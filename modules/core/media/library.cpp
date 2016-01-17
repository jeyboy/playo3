#include "library.h"

using namespace Core;
using namespace Media;
using namespace Models;

Library::~Library() { declineItemStateRestoring(); }

//void Library::stopProcessing() {
//    waitOnProc.clear();
//    waitRemoteOnProc.clear();

//    cancelActiveRestorations();
//}

void Library::setItemState(const QModelIndex & ind, int state) {
    bool override = state < 0;
    if (state & ItemFields::listened || state == -ItemFields::liked)
        state = 0;
    else if (state & ItemFields::liked)
        state = 1;
    else return;

    IItem * itm = indToItm(ind);
    initItemData(itm);

    proceedItemTitles(itm, state, override);
}

void Library::restoreItemState(const QModelIndex & ind) {
    stateRestoring(ind);
}

void Library::restoreItemStateAsync(const QModelIndex & ind, bool is_remote) {
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("RestoreItem"), ind.data().toString());

    QHash<ItemsListType, QHash<QModelIndex, bool> > & list = waitOnProc[ind.model()];
    QHash<QModelIndex, bool> all = list[all_items];

    if (!all.contains(ind)) {
        all.insert(ind, is_remote);

        int current_amount = all.size(), limit = waitListLimit.value(ind.model(), WAIT_LIMIT);

        if (current_amount > limit) {
            for(QHash<QModelIndex, bool>::Iterator item = all.begin(); item != all.end();) {
                IItem * itm = indToItm(item.key());
                itm -> unset(ItemFields::proceeded);

                list[item.value() ? remote_items : local_items].remove(item.key());
            }
        }

        list[is_remote ? remote_items : local_items].insert(ind, true);
    }

    if (!is_remote && inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}

void Library::declineItemStateRestoring(const QModelIndex & ind) {
    QFutureWatcherBase * watcher = 0;

    if (inProc.contains(ind)) watcher = inProc.value(ind);
    if (inRemoteProc.contains(ind)) watcher = inRemoteProc.value(ind);

    if (watcher) {
        watcher -> cancel();
        watcher -> waitForFinished();
        return;
    }

    QHash<ItemsListType, QHash<QModelIndex, bool> > & list = waitOnProc[ind.model()];
    QHash<QModelIndex, bool> all = list[all_items];
    if (all.contains(ind))
        list[all.take(ind) ? remote_items : local_items].remove(ind);
}

void Library::declineItemStateRestoring(const QAbstractItemModel * model) {
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("DeclineItemRestoration"));
    QHash<QModelIndex, bool> items = waitOnProc.take(model)[all_items];

    for(QHash<QModelIndex, bool>::iterator item = items.begin(); item != items.end(); item = items.erase(item))
        indToItm(item.key()) -> unset(ItemFields::proceeded);

    cancelActiveRestorations();
}

void Library::declineItemStateRestoring() {
    QList<const QAbstractItemModel *> keys = waitOnProc.keys();
    for(QList<const QAbstractItemModel *>::ConstIterator key = keys.constBegin(); key != keys.constEnd(); key++)
        declineItemStateRestoring(*key);
}


bool Library::nextProcItem(ItemsListType iType, QModelIndex & ind) {
    if (!waitOnProc.isEmpty()) {
        QHash<QModelIndex, bool> list;

        QList<const QAbstractItemModel *> keys = waitOnProc.keys();
        QList<const QAbstractItemModel *>::Iterator it = keys.end();
        while(it != keys.begin()) {
            --it;

            list = waitOnProc[*it][iType];

            if (list.isEmpty() && waitOnProc[*it][all_items].isEmpty())
                waitOnProc.remove(*it);
            else if (listSyncs[*it] -> tryLock()) {
                list.take(ind = list.keys().last());
                waitOnProc[*it][all_items].remove(ind);
                return true;
            }
        }
    }

    return false;
}


void Library::initStateRestoring() {
    QModelIndex ind;
    if (nextProcItem(local_items, ind)) {
        Logger::obj().write(QStringLiteral("Library"), QStringLiteral("InitInfo"), ind.data().toString());
        QFutureWatcher<void> * initiator = new QFutureWatcher<void>();
        inProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishStateRestoring()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::stateRestoring, ind, initiator));
    }
}

void Library::stateRestoring(const QModelIndex & ind, QFutureWatcher<void> * watcher) {
    IItem * itm = indToItm(ind);
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("StateRestoring"), itm -> title().toString());
    initItemData(itm);

    QHash<QString, int> * cat;
    bool isListened = false;
    int state;
    QList<QString>::iterator i;
    QStringList titles = itm -> titlesCache().toStringList();

    for (i = titles.begin(); i != titles.end(); ++i) {
        if (!(cat = getCatalog(*i))) continue;

        state = cat -> value(*i, -1);

        if (state != -1) {
            if (state == 1) {
                if (!watcher || (watcher && watcher -> isCanceled()))
                    itm -> set(ItemState::liked);
                else
                    emitItemAttrChanging(ind, ItemState::liked);

                if (watcher) listSyncs[ind.model()] -> unlock();
                return;
            }

            isListened |= (state == 0);
        }
    }

    bool canceled = !watcher || (watcher && watcher -> isCanceled());
    if (isListened) {
        if (canceled)
            itm -> set(ItemState::listened);
        else
            emitItemAttrChanging(ind, ItemState::listened);
    } else
        if (!canceled)
            emitItemAttrChanging(ind, ItemState::new_item);

    if (watcher) listSyncs[ind.model()] -> unlock();
}

void Library::finishStateRestoring() {
    QFutureWatcher<void> * initiator = (QFutureWatcher<void> *)sender();
    QModelIndex ind = inProc.key(initiator);
    inProc.remove(ind);
    delete initiator;

    if (inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}





IItem * Library::indToItm(const QModelIndex & ind) {
    return static_cast<IItem *>(ind.internalPointer());
//    return (qobject_cast<const IModel *>(ind.model())) -> item(ind);
}

void Library::emitItemAttrChanging(const QModelIndex & ind, int state) {
    QMetaObject::invokeMethod(
        const_cast<QAbstractItemModel *>(ind.model()),
        "onUpdateAttr",
        (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection),
        Q_ARG(const QModelIndex &, ind),
        Q_ARG(int, ISTATERESTORE),
        Q_ARG(const QVariant &, state)
    );
//    connect(
//        this, SIGNAL(updateAttr(QModelIndex,int,QVariant)),
//        ind.model(), SLOT(onUpdateAttr(const QModelIndex,int,QVariant)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection)
//    );
//    emit updateAttr(ind, ISTATERESTORE, state);
//    disconnect(this, SIGNAL(updateAttr(QModelIndex,int,QVariant)), ind.model(), SLOT(onUpdateAttr(const QModelIndex,int,QVariant)));
}










void Library::remoteItemInfo() {
    QModelIndex ind;
    if (nextProcItem(remote_items, ind)) {
        Logger::obj().write(QStringLiteral("Library"), QStringLiteral("InitRemoteInfo"), ind.data().toString());
        QFutureWatcher<bool> * initiator = new QFutureWatcher<bool>();
        inRemoteProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishRemoteItemInfoInit()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::remoteInfoRestoring, initiator, ind));
    }

    remoteProcTimer -> setInterval(Settings::obj().remoteItemsProcDelay());
}

void Library::finishRemoteItemInfo() {
    QFutureWatcher<bool> * initiator = (QFutureWatcher<bool> *)sender();
    bool result = initiator -> isCanceled() || initiator -> result();
    QModelIndex ind = inRemoteProc.key(initiator);
    inRemoteProc.remove(ind);
    delete initiator;

    if (!result) // call new item initiation if prev item initiation is finished without remote call for info
        remoteItemInfo();
}

void Library::cancelActiveRestorations() {
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("CancelActiveRestorations"));
    {
        QList<QFutureWatcher<void> *> inProcList = inProc.values();
        for(QList<QFutureWatcher<void> *>::Iterator it = inProcList.begin(); it != inProcList.end(); it++)
//            if ((*it) -> isRunning())
                (*it) -> cancel();
    }

    QList<QFutureWatcher<bool> *> inProcRemoteList = inRemoteProc.values();


    for(QList<QFutureWatcher<bool> *>::Iterator it = inProcRemoteList.begin(); it != inProcRemoteList.end(); it++)
//        if ((*it) -> isRunning())
            (*it) -> cancel();
}


bool Library::remoteInfoRestoring(const QModelIndex & ind, QFutureWatcher<bool> * watcher) {
    IItem * itm = indToItm(ind);
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("RemoteInfoRestoring"), itm -> title().toString());
    bool has_info = itm -> hasInfo();

    if (has_info) {
        listSyncs[ind.model()] -> unlock();
        return false;
    }

    MediaInfo m(itm -> toUrl(), itm -> extension(), has_info);
    if (m.isRemote() && m.hasError()) {
        if (IModel::restoreUrl(itm))
            m = MediaInfo(itm -> toUrl(), itm -> extension(), has_info);
    }

    initItemInfo(&m, itm);

    if (!watcher -> isCanceled())
        emitItemAttrChanging(ind, ind.data(ISTATE).toInt());

    listSyncs[ind.model()] -> unlock();
    return true;
}

////////////////////////////////////////////////////////////////////////
///// privates
////////////////////////////////////////////////////////////////////////
bool Library::proceedItemTitles(IItem * itm, int state, bool override) {
    QHash<QString, int> * cat;
    QChar letter;
    bool catState = false, catalog_has_item, catalog_state_has_item;
    QList<QString> * saveList;
    QStringList names = itm -> titlesCache().toStringList();

    for (QList<QString>::iterator i = names.begin(); i != names.end(); ++i) {
        saveList = 0;
        letter = getCatalogName(*i);
        cat = getCatalog(letter);

        catalog_has_item = cat -> contains(*i);

        if (override || !catalog_has_item || (catalog_has_item && cat -> value(*i) < state)) {
            cat -> insert(*i, state);

            catalog_state_has_item = catalogs_state.contains(letter);
            if (catalog_state_has_item)
                saveList = catalogs_state.value(letter);

            if (catalog_has_item) {
                if (saveList)
                    delete saveList;
                catalogs_state.insert(letter, 0);
            } else {
                if (saveList == 0)
                    saveList = new QList<QString>();

                saveList -> append(*i);
                catalogs_state.insert(letter, saveList);
            }
        }
    }

    return catState;
}

void Library::initItemData(IItem * itm) {
    bool has_titles = itm -> titlesCache().isValid();
    bool has_info = (itm -> isRemote() && Settings::obj().isUsedDelayForRemote()) || itm -> hasInfo();

    if (has_titles && has_info) return;

    MediaInfo m(itm -> toUrl(), itm -> extension(), has_info);
    if (m.hasError()) return;
    if (!has_titles) initItemTitles(&m, itm);
    if (!has_info) initItemInfo(&m, itm);
}

void Library::initItemInfo(MediaInfo * info, IItem * itm) {
    itm -> setSize(info -> getSize());
    if (info -> isReaded())
        itm -> setInfo(Info::str(Info::toUnits(info -> getSize()), info -> getExtension(), info -> getBitrate(), info -> getSampleRate(), info -> getChannels()));
    else
        itm -> setInfo(Info::str(Info::toUnits(info -> getSize()), info -> getExtension()));
    if (info -> getDuration() > 0)
        itm -> setDuration(Duration::fromSeconds(info -> getDuration()));
    itm -> setGenre(info -> getGenre());

    if (!info -> getExtension().isEmpty())
        itm -> setExtension(info -> getExtension());
}

void Library::initItemTitles(MediaInfo * info, IItem * itm) {
    QString title = itm -> title().toString();
    if (itm -> isRemote()) //FIXME local files broke at this time by clearing // need to separately storing of display title for tree
        title = FilenameConversions::extraSymbolsFilter(title);
    itm -> setTitle(title);

    QStringList list;
    title = FilenameConversions::cacheTitleFilter(title);
    list.append(title);

    QString temp = FilenameConversions::forwardNumberFilter(title);
    if (temp != title)
        list.append(temp);

    QString tagTitle = FilenameConversions::cacheTitleFilter(info -> getArtist() + info -> getTitle());
    if (!tagTitle.isEmpty() && tagTitle != title && tagTitle != temp)
        list.append(tagTitle);

    itm -> setTitlesCache(list);
}
