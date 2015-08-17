#include "search_model.h"

using namespace Playo3;

SearchModel::~SearchModel() {
    if (initiator)
        initiator -> cancel();

    delete initiator;
}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    initiator = new QFutureWatcher<FolderItem *>();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, &SearchModel::searchRoutine, initiator));
}

void SearchModel::initiateSearch(QStringList & predicates) {
    SearchSettings settings(true, true, true);
    settings.predicates = predicates;
    settings.onlyOne = true;
    initiateSearch(settings);
}

int SearchModel::proceedTabs(SearchRequest & params, FolderItem * parent) {
    return ((IModel *) params.search_interface) -> initiateSearch(params, parent);
}

int SearchModel::proceedMyComputer(SearchRequest & params, FolderItem * parent) {
    int amount = 0;
    QStringList filters;

    if (params.spredicate.isEmpty()) {
        filters = Extensions::instance() -> filterList("music");
    } else {
        QStringList res = Extensions::instance() -> filterList("music");
        for(QStringList::Iterator it = res.begin(); it != res.end(); it++)
            filters.append("*" + params.spredicate + (*it));
    }

    //System.Music.Artist:(Beethoven OR Mozart)

    QDirIterator dir_it(*params.search_interface, filters,  QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        bool valid = params.sgenre_id == -1;

        if (!valid) {
            MediaInfo m(QUrl::fromLocalFile(path), true, true);
            valid = m.getGenre() == params.sgenre_id;
        }

        if (valid) {
            QFileInfo file = dir_it.fileInfo();
            new FileItem(file.path(), file.fileName(), parent);
            amount++;
        }
    }

    return amount;
}

void SearchModel::searchFinished() {
    if (!initiator -> isCanceled()) {
//        FolderItem * folder = initiator -> result();
//        QList<FolderItem *> children = folder -> folderChildren();

//        beginInsertRows(QModelIndex(), 0, children.count());
//            for(QList<FolderItem *>::Iterator it = children.begin(); it != children.end(); it++)
//                rootItem -> linkNode(*it);
//        endInsertRows();
    }

    beginResetModel();
    endResetModel();
    emit moveOutProcess();

    delete initiator;
    initiator = 0;
}

FolderItem * SearchModel::searchRoutine(QFutureWatcher<FolderItem *> * watcher) {
    emit moveInProcess();
    QList<SearchRequest> requests;
    FolderItem * res = rootItem;

    emit moveInBackgroundProcess();
    prepareRequests(requests);

    float total = requests.size() / 100.0;
    while(!requests.isEmpty()) {
        if (watcher -> isCanceled())
            return res;

        SearchRequest r = requests.takeFirst();

        emit setProgress(requests.size() / total);
        FolderItem * parent = res -> createFolder(r.token());
        int propagate_count = 0;

        switch(r.search_type) {
            case SearchRequest::local: {
                propagate_count = proceedMyComputer(r, parent);
            break;}

            case SearchRequest::inner: {
                propagate_count = ((IModel *) r.search_interface) -> initiateSearch(r, parent);
            break;}

            case SearchRequest::remote: {
                ISearchable * iface = (ISearchable *) r.search_interface;

                switch (r.search_interface -> siteType()) { // unificate vk and sc search requests
                    case Playo3::vk_site: {
                        QJsonArray items;
                        if (r.spredicate.isEmpty() && r.popular) {
                            items = Vk::Api::instance() -> audioPopular(true, r.sgenre_id);
                        } else
                            items = Vk::Api::instance() -> audioSearch(
                                r.spredicate, request.type == artist, request.search_in_own, r.popular, request.limit(DEFAULT_LIMIT_AMOUNT)
                            );
                        propagate_count = proceedVkList(items, parent);
                    break;}
                    case Playo3::sc_site: {
                        QJsonArray items = Soundcloud::Api::instance() -> audioSearch(r.spredicate, r.sgenre, r.popular, request.limit(DEFAULT_LIMIT_AMOUNT));
                        if (Soundcloud::Api::extractCount(items) > 0)
                            parent -> backPropagateItemsCountInBranch(proceedScList(items, parent));
                    break;}

                    default: {
                        QJsonArray items = iface -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
                        propagate_count = proceedGrabberList(iface -> siteType(), items, parent);
                    }
                }
            break;}

            default:;
//            case SearchRequest::request_vk: {
//                QJsonArray items;
//                if (r.spredicate.isEmpty() && r.popular) {
//                    items = Vk::Api::instance() -> audioPopular(true, r.sgenre_id);
//                } else
//                    items = Vk::Api::instance() -> audioSearch(
//                        r.spredicate, request.type == artist, request.search_in_own, r.popular, request.limit(DEFAULT_LIMIT_AMOUNT)
//                    );

//                parent -> backPropagateItemsCountInBranch(proceedVkList(items, parent));
//            break;}
//            case SearchRequest::request_4shared: {
//                QJsonArray items = Fourshared::Api::instance() -> audioSearch(r.spredicate, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::fourshared, items, parent));
//            break;}
//            case SearchRequest::request_sc: {
//                QJsonArray items = Soundcloud::Api::instance() -> audioSearch(r.spredicate, r.sgenre, r.popular, request.limit(DEFAULT_LIMIT_AMOUNT));
//                if (Soundcloud::Api::extractCount(items) > 0)
//                    parent -> backPropagateItemsCountInBranch(proceedScList(items, parent));
//            break;}
//            case SearchRequest::request_computer: {
//                parent -> backPropagateItemsCountInBranch(proceedMyComputer(r, parent));
//            break;}
//            case SearchRequest::request_tabs: {
//                parent -> backPropagateItemsCountInBranch(proceedTabs(r, parent));
//            break;}
//            case SearchRequest::request_other: {
//                QJsonArray items = Grabber::MyzukaAlbum::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::myzuka, items, parent));

//                items = Grabber::Zaycev::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::zaycev, items, parent));

//                items = Grabber::Mp3Base::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::mp3base, items, parent));

//                items = Grabber::PromoDj::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::promodj, items, parent));

//                items = Grabber::Mp3cc::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::mp3cc, items, parent));

//                items = Grabber::Mp3pm::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::mp3pm, items, parent));

//                items = Grabber::Shmidt::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::shmidt, items, parent));

//                items = Grabber::Jetune::instance() -> search(r.spredicate, r.sgenre, r.sgenre_id, r.popular, request.type == artist, request.type == song, request.limit(DEFAULT_LIMIT_AMOUNT));
//                parent -> backPropagateItemsCountInBranch(proceedGrabberList(Playo3::jetune, items, parent));
//            break;}
        }

        parent -> backPropagateItemsCountInBranch(propagate_count);
    }

    emit moveOutBackgroundProcess();
    return res;
}

void SearchModel::prepareRequests(QList<SearchRequest> & requests) {
    bool web_predicable = !(request.predicates.isEmpty() && request.genres.isEmpty());

    if (request.predicates.isEmpty()) request.predicates << QString();
    if (request.genres.isEmpty()) request.genres << QString();

    for(QStringList::Iterator it = request.predicates.begin(); it != request.predicates.end(); it++) {
        QString predicate = (*it).replace(QRegularExpression("'\""), " ");

        for(QList<QString>::Iterator genre_it = request.genres.begin(); genre_it != request.genres.end(); genre_it++) {
            if (web_predicable && request.inSites)
                for(QList<void *>::Iterator search_interface = request.sites.begin(); search_interface != request.sites.end(); search_interface++)
                    requests.append(SearchRequest(SearchRequest::remote, (*search_interface), predicate, (*genre_it), request.popular));

            if (request.inTabs)
                for(QList<void *>::Iterator tab = request.tabs.begin(); tab != request.tabs.end(); tab++)
                    requests.append(SearchRequest(SearchRequest::inner, (*tab), predicate, (*genre_it), request.popular));

            if (request.inComputer)
                for(QList<void *>::Iterator drive = request.drives.begin(); drive != request.drives.end(); drive++)
                    requests.append(SearchRequest(SearchRequest::local, drive, predicate, (*genre_it), request.popular));
        }
    }

//    if (!request.predicates.isEmpty()) { // search by predicates
//        for(QStringList::Iterator it = request.predicates.begin(); it != request.predicates.end(); it++) {
//            QString predicate = (*it).replace(QRegularExpression("'\""), " ");

//            if (request.inFourshared) requests.append(SearchRequest(SearchRequest::request_4shared, predicate, QString(), 0, request.popular));

//            if (!request.genres.isEmpty()) {
//                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, predicate, QString(), 0, request.popular));

//                for(QHash<int, QString>::Iterator genre_it = request.genres.begin(); genre_it != request.genres.end(); genre_it++) {
//                    if (request.inSites)
//                        for(QList<void *>::Iterator search_interface = request.sites.begin(); search_interface != request.sites.end(); search_interface++)
//                            requests.append(SearchRequest(SearchRequest::remote, (*search_interface), predicate, genre_it.key(), request.popular));

//                    if (request.inTabs)
//                        for(QList<void *>::Iterator tab = request.tabs.begin(); tab != request.tabs.end(); tab++)
//                            requests.append(SearchRequest(SearchRequest::inner, (*tab), predicate, genre_it.key(), request.popular));

//                    if (request.inComputer)
//                        for(QList<void *>::Iterator drive = request.drives.begin(); drive != request.drives.end(); drive++)
//                            requests.append(SearchRequest(SearchRequest::local, drive, predicate, genre_it.key(), request.popular));
//                }
//            } else {
//                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, predicate, QString(), -1, request.popular));
//                if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, predicate, QString(), -1, request.popular));
//                if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, predicate, QString(), -1, request.popular));
//                if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, predicate, QString(), -1, request.popular));
//                if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, predicate, QString(), -1, request.popular));
//            }
//        }
//    } else if (!request.genres.isEmpty()) {
//        for(QHash<int, QString>::Iterator genre_it = request.genres.begin(); genre_it != request.genres.end(); genre_it++) {
//            if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, QString(), QString(), VkGenres::instance() -> fromStandartId(genre_it.key()), request.popular));
//            if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, QString(), genre_it.value(), genre_it.key(), request.popular));
//            if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, QString(), genre_it.value(), genre_it.key(), request.popular));
//            if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, QString(), genre_it.value(), genre_it.key(), request.popular));
//            if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, QString(), genre_it.value(), genre_it.key(), request.popular));
//        }
//    } else if (request.popular) {
//        if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk));
//        if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc));
//        if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other));
//        if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs));
//        if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer));
//    } else {
//        if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs));
//        if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer));
//    }
}
