#include "search_model.h"

using namespace Models;
using namespace Core::Web;

SearchModel::~SearchModel() {
    if (initiator)
        initiator -> cancel();

    delete initiator;
}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    initiator = new QFutureWatcher<Playlist *>();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, &SearchModel::searchRoutine, initiator));
}

void SearchModel::initiateSearch(QStringList & predicates) {
    SearchSettings settings(true, true, false/*true*/);
    settings.predicates = predicates;
    settings.onlyOne = true;
    initiateSearch(settings);
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
    QDirIterator dir_it(*(QString *)(params.search_interface), filters,  QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        bool valid = true; //params.sgenre_id == -1;

//        if (!valid) {
//            MediaInfo m(QUrl::fromLocalFile(path), true, true);
//            valid = m.getGenre() == params.sgenre_id;
//        }

        if (valid) {
            QFileInfo file = dir_it.fileInfo();
            new File(file.path(), file.fileName(), parent);
            amount++;
        }
    }

    delete (QString *)params.search_interface;
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

Playlist * SearchModel::searchRoutine(QFutureWatcher<Playlist *> * watcher) {
    emit moveInProcess();
    QList<SearchRequest> requests;
    Playlist * res = rootItem;

    emit moveInBackgroundProcess();
    prepareRequests(requests);
    ISearchable::SearchLimit limitation((ISearchable::PredicateType)request.type, request.limit(DEFAULT_LIMIT_AMOUNT));

    float total = requests.size() / 100.0;
    while(!requests.isEmpty()) {
        if (watcher -> isCanceled())
            return res;

        SearchRequest r = requests.takeFirst();

        emit setProgress(requests.size() / total);
        Playlist * parent = res -> createPlaylist(r.token());
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
                QJsonArray items = iface -> search(r.spredicate, r.sgenre, limitation);

                switch (iface -> siteType()) {
                    case vk_site: { propagate_count = proceedVkList(items, parent); break; }
                    case sc_site: {
                        if (items.size() > 0)
                            propagate_count = proceedScList(items, parent);
                    break;}
                    case od_site: {
                        if (items.size() > 0)
                            propagate_count = proceedOdList(items, parent);
                    break;}
                    default: propagate_count = proceedGrabberList(iface -> siteType(), items, parent);
                }

                qDebug() << "SOSOSO" << iface -> siteType() << propagate_count;
            break;}

            default:;
        }

        if (propagate_count > 0)
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
                for(QStringList::Iterator drive = request.drives.begin(); drive != request.drives.end(); drive++)
                    requests.append(SearchRequest(SearchRequest::local, new QString(*drive), predicate, (*genre_it), request.popular));
        }
    }
}
