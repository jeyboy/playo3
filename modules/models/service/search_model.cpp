#include "search_model.h"

using namespace Models;
using namespace Core::Web;

SearchModel::~SearchModel() {
    if (initiator && initiator -> isRunning()) {
        initiator -> cancel();
        initiator -> waitForFinished();
    }

    delete initiator;

    beginResetModel();
    endResetModel();
}

void SearchModel::startSearch(bool continues) {
    initiator = new QFutureWatcher<void>();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, (
            request.limit(DEFAULT_LIMIT_AMOUNT) == 1 && (continues || !request.predicates.isEmpty()) ? &SearchModel::searchSingleRoutine : &SearchModel::searchRoutine
        ), initiator));
    emit updateRemovingBlockation(true);
}

void SearchModel::initiateSearch(const SearchSettings & params) {
    request = params;
    startSearch();
}

void SearchModel::declineSearch() {
    if (initiator && initiator -> isRunning()) {
        initiator -> cancel();
        initiator -> waitForFinished();
        requests.clear();
        search_reglament.clear();
    }
}
void SearchModel::suspendSearch(QJsonObject & obj) {
    if (initiator && initiator -> isRunning()) {
        initiator -> cancel();
        initiator -> waitForFinished();

        if (!requests.isEmpty()) {
            QJsonArray res;
            for(QList<SearchRequest>::Iterator request = requests.begin(); request != requests.end(); request++)
                (*request).save(res);

            obj.insert(SEARCH_JSON_KEY, res);
            obj.insert(SEARCH_SET_JSON_KEY, request.toJson());
            obj.insert(SEARCH_REGLAMENT_JSON_KEY, QJsonObject::fromVariantHash(search_reglament));
        }
    } else { // wtf - why this keys apppers in json ?
        obj.remove(SEARCH_JSON_KEY);
        obj.remove(SEARCH_SET_JSON_KEY);
        obj.remove(SEARCH_REGLAMENT_JSON_KEY);
    }
}

void SearchModel::resumeSearch(const QJsonObject & obj) {
    QJsonArray res = obj.value(SEARCH_JSON_KEY).toArray();

    if (res.isEmpty()) return;

    request.fromJson(obj.value(SEARCH_SET_JSON_KEY).toObject());
    SearchRequest::fromJson(res, requests);
    search_reglament = obj.value(SEARCH_REGLAMENT_JSON_KEY).toObject().toVariantHash();

    startSearch(true);
}

int SearchModel::proceedTabs(SearchRequest & params, Playlist * parent) {
    return ((IModel *) params.search_interface) -> initiateSearch(params, parent);
}

int SearchModel::proceedMyComputer(SearchRequest & params, Playlist * parent) {
    int amount = 0;
    QStringList filters;

    if (params.spredicate.isEmpty()) {
        filters = Extensions::obj().filterList("music");
    } else {
        QStringList res = Extensions::obj().filterList("music");
        for(QStringList::Iterator it = res.begin(); it != res.end(); it++)
            filters.append("*" + params.spredicate + (*it));
    }

    //System.Music.Artist:(Beethoven OR Mozart)

//    int genre_id = MusicGenres::instance() -> toInt();
    QDirIterator dir_it(*(QString *)(params.search_interface), filters,  (QDir::Filter)FILE_FILTERS, QDirIterator::Subdirectories);

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        bool valid = true; //params.sgenre_id == -1;

//        if (!valid) {
//            MediaInfo m(QUrl::fromLocalFile(path), true, true);
//            valid = m.getGenre() == params.sgenre_id;
//        }

        if (valid) {
            new File(path, dir_it.fileName(), parent);
            amount++;
        }
    }

    params.clearInterface();
    return amount;
}

void SearchModel::searchFinished() {
    if (!initiator -> isCanceled()) {
        search_reglament.clear();
        requests.clear();
    }

    emit updateRemovingBlockation(false);
    delete initiator;
    initiator = 0;
}

void SearchModel::searchRoutine(QFutureWatcher<void> * watcher) {
    Playlist * res = rootItem;

    emit moveInBackgroundProcess();
    if (requests.isEmpty())
        prepareRequests(requests);

    ISearchable::SearchLimit limitation((ISearchable::PredicateType)request.type, request.limit(DEFAULT_LIMIT_AMOUNT));

    int offset = res -> childCount();
    float total = requests.size() / 100.0;
    while(!requests.isEmpty()) {
        if (watcher -> isCanceled())
            break;

        SearchRequest r = requests.takeFirst();

        Playlist * parent = res -> createPlaylist(r.token());
        int propagate_count = 0;

        switch(r.search_type) {
            case SearchRequest::local: {
//                qDebug() << "SO COMP";
                propagate_count = proceedMyComputer(r, parent);
            break;}

            case SearchRequest::inner: {
                qDebug() << "SO INNER";
                propagate_count = ((IModel *) r.search_interface) -> initiateSearch(r, parent);
            break;}

            case SearchRequest::remote: {
                ISearchable * iface = (ISearchable *) r.search_interface;
                qDebug() << "SO START" << iface -> siteType();
                QJsonArray items = iface -> search(r.spredicate, r.sgenre, limitation);

                switch (iface -> siteType()) {
                    case site_vk: { propagate_count = proceedVkList(items, parent); break; }
                    case site_sc: { propagate_count = proceedScList(items, parent); break;}
                    case site_od: { propagate_count = proceedOdList(items, parent); break;}
                    case site_yandex: { propagate_count = proceedYandexList(items, parent); break;}
                    default: propagate_count = proceedGrabberList(iface -> siteType(), items, parent);
                }

                qDebug() << "SOSOSO" << iface -> siteType() << propagate_count;
            break;}

            default:;
        }

        if (propagate_count > 0) {
            // hack for view updating
            beginInsertRows(QModelIndex(), offset, offset);
            endInsertRows();
            parent -> backPropagateItemsCountInBranch(propagate_count);
//            emit collapseNeeded(index(parent));
        }
        offset += 1;
        emit setBackgroundProgress(requests.size() / total);
    }

    qDebug() << "SO END";
    emit moveOutBackgroundProcess();
}

void SearchModel::searchSingleRoutine(QFutureWatcher<void> * watcher) {
    Playlist * res = rootItem;

    emit moveInBackgroundProcess();
    if (requests.isEmpty())
        prepareRequests(requests);

    ISearchable::SearchLimit limitation((ISearchable::PredicateType)request.type, 1);

    int offset = res -> childCount();
    float total = requests.size() / 100.0;
    while(!requests.isEmpty()) {
        if (watcher -> isCanceled())
            break;

        SearchRequest r = requests.takeFirst();

        Playlist * parent = new Playlist();
        search_reglament.insert(r.token(), false);
        int propagate_count = 0;

        switch(r.search_type) {
            case SearchRequest::local: {
                qDebug() << "SO LOCAL";
                propagate_count = proceedMyComputer(r, parent);
            break;}

            case SearchRequest::inner: {
                qDebug() << "SO INNER";
                propagate_count = ((IModel *) r.search_interface) -> initiateSearch(r, parent);
            break;}

            case SearchRequest::remote: {
                ISearchable * iface = (ISearchable *) r.search_interface;
                qDebug() << "SO START" << iface -> siteType();
                QJsonArray items = iface -> search(r.spredicate, r.sgenre, limitation);

                switch (iface -> siteType()) {
                    case site_vk: { propagate_count = proceedVkList(items, parent); break; }
                    case site_sc: { propagate_count = proceedScList(items, parent); break;}
                    case site_od: { propagate_count = proceedOdList(items, parent); break;}
                    default: propagate_count = proceedGrabberList(iface -> siteType(), items, parent);
                }

                qDebug() << "SOSOSO" << iface -> siteType() << propagate_count;
            break;}

            default:;
        }

        if (propagate_count > 0) {
            int taked_amount = innerSearch(r.token(), res, parent, 1); // need to rework algo - at this time taked first compatible item - not better
            parent -> removeYouself();

            if (taked_amount > 0) {
                search_reglament.insert(r.token(), true);

                while (true) {
                    if (requests.isEmpty()) break;
                    if (requests.first().spredicate == r.token())
                        requests.removeFirst();
                    else break;
                }

                beginInsertRows(QModelIndex(), offset, offset);
                endInsertRows();
                res -> backPropagateItemsCountInBranch(taked_amount);
                offset += taked_amount;
                QThread::msleep(250); //delay for getting away from captches
            }
        }
        emit setBackgroundProgress(requests.size() / total);
    }

    int not_finded = 0;
    for(QVariantHash::Iterator it = search_reglament.begin(); it != search_reglament.end(); it++)
        if (!it.value().toBool()) {
            WebFile * file = new WebFile(QVariant(), QString(), it.key(), res);
            file -> set(ItemState::not_exist);
            not_finded++;
        }

    if (not_finded > 0) {
        beginInsertRows(QModelIndex(), offset, offset + (not_finded - 1));
        endInsertRows();
        res -> backPropagateItemsCountInBranch(not_finded);
    }


    qDebug() << "SO END";
    emit moveOutBackgroundProcess();
}

void SearchModel::prepareRequests(QList<SearchRequest> & requests) {
    bool web_predicable = !(request.predicates.isEmpty() && request.genres.isEmpty()) || request.popular;

    if (request.predicates.isEmpty()) request.predicates << QString();
    if (request.genres.isEmpty()) request.genres << QString();

    for(QStringList::Iterator it = request.predicates.begin(); it != request.predicates.end(); it++) {
        QString predicate = (*it).replace(QRegularExpression("['\"]"), " ");

        for(QList<QString>::Iterator genre_it = request.genres.begin(); genre_it != request.genres.end(); genre_it++) {
            if (web_predicable && request.inSites)
                for(QList<void *>::Iterator search_interface = request.sites.begin(); search_interface != request.sites.end(); search_interface++)
                    requests.append(SearchRequest(SearchRequest::remote, (*search_interface), predicate, (*genre_it), request.popular));

            if (request.inTabs) {
                for(QList<void *>::Iterator tab = request.tabs.begin(); tab != request.tabs.end(); tab++)
                    requests.append(SearchRequest(SearchRequest::inner, (*tab), predicate, (*genre_it), request.popular));
            }

            if (request.inComputer)
                for(QStringList::Iterator drive = request.drives.begin(); drive != request.drives.end(); drive++)
                    requests.append(SearchRequest(SearchRequest::local, new QString(*drive), predicate, (*genre_it), request.popular));
        }
    }
}
