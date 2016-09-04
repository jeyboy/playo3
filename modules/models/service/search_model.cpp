#include "search_model.h"
#include "dockbars.h"
#include "modules/core/web/web_apis.h"

using namespace Models;
using namespace Core::Web;

SearchModel::~SearchModel() {
    if (inProccess()) {
        STOP_SEARCH();
    }

    delete initiator;

    beginResetModel();
    endResetModel();
}

void SearchModel::start() {
    initiator = new QFutureWatcher<void>();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, &SearchModel::searchRoutine, initiator));
    emit updateRemovingBlockation(true);
}

void SearchModel::initiate(const SearchLimitLayers & params) {
    layers = params;
    start();
}

void SearchModel::decline() {
    if (inProccess()) {
        STOP_SEARCH();
        requests.clear();
        search_reglament.clear();
    }
}

void SearchModel::load(const QJsonObject & obj) {
    QJsonArray res = obj.value(SEARCH_REQUESTS_JSON_KEY).toArray();

    if (res.isEmpty()) return;

    SearchLimitLayer::fromJson(res, requests);
    search_reglament = obj.value(SEARCH_REGLAMENT_JSON_KEY).toObject().toVariantHash();

    start();
}

void SearchModel::save(QJsonObject & obj) {
    if (inProccess()) {
        STOP_SEARCH();

        if (!requests.isEmpty()) {
            QJsonArray res;
            for(QList<SearchLimitLayer>::Iterator request = requests.begin(); request != requests.end(); request++)
                (*request).save(res);

            obj.insert(SEARCH_REQUESTS_JSON_KEY, res);
            obj.insert(SEARCH_REGLAMENT_JSON_KEY, QJsonObject::fromVariantHash(search_reglament));
        }
    }
}

int SearchModel::proceedMyComputer(SearchLimitLayer & params, Playlist * parent) {
    int amount = 0;
    QStringList filters;

    if (params.predicate.isEmpty()) {
        filters = Extensions::obj().filterList("music");
    } else {
        QStringList res = Extensions::obj().filterList("music");
        for(QStringList::Iterator it = res.begin(); it != res.end(); it++)
            filters.append("*" + params.predicate + (*it));
    }

    //    int genre_id = MusicGenres::instance() -> toInt();
    QDirIterator dir_it(params.context.toString(), filters, (QDir::Filter)FILE_FILTERS, QDirIterator::Subdirectories);

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        bool valid = true; //params.sgenre_id == -1;

//        if (!valid) {
//            MediaInfo m(QUrl::fromLocalFile(path), true, true);
//            valid = m.getGenre() == params.sgenre_id;
//        }

        if (valid) {
            new IItem(parent, LOCAL_ITEM_ATTRS(path, dir_it.fileName()));
            amount++;
        }
    }

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
    Playlist * res = root_item;

    emit moveInBackgroundProcess();
    if (requests.isEmpty())
        layers.prepareLayers(requests);

    QHash<QString, QAbstractItemModel *> inners;
    QList<Controls::DockBar *> bars = Presentation::Dockbars::obj().dockbars();

    for(QList<Controls::DockBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
        Views::IView * v = Presentation::Dockbars::obj().view(*bar);
        if (v) inners.insert((*bar) -> objectName(), v -> model());
    }

    bool singular = requests.first().items_limit == 1;
    int offset = res -> childCount();
    float total = requests.size() / 100.0;

    while(!requests.isEmpty()) {
        if (watcher -> isCanceled()) break;

        SearchLimitLayer r = requests.takeFirst();
        Playlist * parent = res -> createPlaylist(dt_playlist, r.token());
        int propagate_count = 0;

        switch(r.req_type) {
            case sr_local: {
//                qDebug() << "SO COMP";
                propagate_count = proceedMyComputer(r, parent);
            break;}

            case sr_inner: {
                IModel * _mdl = ((IModel *) inners.value(r.context.toString(), 0));
                qDebug() << "SO INNER" << r.context << _mdl;
                propagate_count = _mdl -> search(r, parent);
            break;}

            case sr_remote: {
                ISource * iface = Web::Apis::source((DataSubType)r.context.toInt());
                qDebug() << "SO START" << iface -> sourceType();
                QJsonValue json = iface -> search(r);
                propagate_count = proceedBlocks(json.toArray(), parent);
                qDebug() << "SOSOSO" << iface -> sourceType() << propagate_count;
            break;}

            default:;
        }

        if (singular) {
            if (propagate_count > 0) {
                int taked_amount = search(r.token(), res, parent, 1); // need to rework algo - at this time taked first compatible item - not better
                parent -> removeYouself();

                if (taked_amount > 0) {
                    search_reglament.insert(r.token(), true);

                    while (true) {
                        if (requests.isEmpty()) break;
                        if (requests.first().predicate == r.token())
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
        } else {
            if (propagate_count > 0) {
                // hack for view updating
                beginInsertRows(QModelIndex(), offset, offset);
                endInsertRows();
//                parent -> backPropagateItemsCountInBranch(propagate_count);
    //            emit collapseNeeded(index(parent));
            }
            offset += 1;
        }

        emit setBackgroundProgress(requests.size() / total);
    }

    if (singular) {
        int not_finded = 0;
        for(QVariantHash::Iterator it = search_reglament.begin(); it != search_reglament.end(); it++)
            if (!it.value().toBool()) {
                new IItem(res, NO_SOURCE_ITEM_ATTRS(it.key()));
                not_finded++;
            }

        if (not_finded > 0) {
            beginInsertRows(QModelIndex(), offset, offset + (not_finded - 1));
            endInsertRows();
            res -> backPropagateItemsCountInBranch(not_finded);
        }
    }

    qDebug() << "SO END";
    emit moveOutBackgroundProcess();
}

//void SearchModel::searchSingleRoutine(QFutureWatcher<void> * watcher) {
//    Playlist * res = rootItem;

//    emit moveInBackgroundProcess();
//    if (requests.isEmpty())
//        prepareRequests(requests);

//    ISearchable::SearchLimit limitation(ISearchable::sc_all, (ISearchable::SearchPredicateType)request.type, QString(), QString(), 1);

//    int offset = res -> childCount();
//    float total = requests.size() / 100.0;
//    while(!requests.isEmpty()) {
//        if (watcher -> isCanceled())
//            break;

//        SearchRequest r = requests.takeFirst();

//        Playlist * parent = new Playlist();
//        search_reglament.insert(r.token(), false);
//        int propagate_count = 0;

//        switch(r.search_type) {
//            case SearchRequest::local: {
//                qDebug() << "SO LOCAL";
//                propagate_count = proceedMyComputer(r, parent);
//            break;}

//            case SearchRequest::inner: {
//                qDebug() << "SO INNER";
//                propagate_count = ((IModel *) r.search_interface) -> initiateSearch(r, parent);
//            break;}

//            case SearchRequest::remote: {
//                ISearchable * iface = (ISearchable *) r.search_interface;
//                qDebug() << "SO START" << iface -> siteType();
//                QJsonArray items = iface -> search(*limitation.updatePredicates(r.spredicate, r.sgenre));

//                switch (iface -> siteType()) {
//                    case dt_site_vk: { propagate_count = proceedVkList(items, parent); break; }
//                    case dt_site_sc: { propagate_count = proceedScList(items, parent); break;}
//                    case dt_site_od: { propagate_count = proceedOdList(items, parent); break;}
//                    default: propagate_count = proceedGrabberList(iface -> siteType(), items, parent);
//                }

//                qDebug() << "SOSOSO" << iface -> siteType() << propagate_count;
//            break;}

//            default:;
//        }

//        if (propagate_count > 0) {
//            int taked_amount = innerSearch(r.token(), res, parent, 1); // need to rework algo - at this time taked first compatible item - not better
//            parent -> removeYouself();

//            if (taked_amount > 0) {
//                search_reglament.insert(r.token(), true);

//                while (true) {
//                    if (requests.isEmpty()) break;
//                    if (requests.first().spredicate == r.token())
//                        requests.removeFirst();
//                    else break;
//                }

//                beginInsertRows(QModelIndex(), offset, offset);
//                endInsertRows();
//                res -> backPropagateItemsCountInBranch(taked_amount);
//                offset += taked_amount;
//                QThread::msleep(250); //delay for getting away from captches
//            }
//        }
//        emit setBackgroundProgress(requests.size() / total);
//    }

//    int not_finded = 0;
//    for(QVariantHash::Iterator it = search_reglament.begin(); it != search_reglament.end(); it++)
//        if (!it.value().toBool()) {
//            new IItem(res, NO_SOURCE_ITEM_ATTRS(it.key()));
//            not_finded++;
//        }

//    if (not_finded > 0) {
//        beginInsertRows(QModelIndex(), offset, offset + (not_finded - 1));
//        endInsertRows();
//        res -> backPropagateItemsCountInBranch(not_finded);
//    }


//    qDebug() << "SO END";
//    emit moveOutBackgroundProcess();
//}
