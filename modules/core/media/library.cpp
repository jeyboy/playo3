#include "library.h"

using namespace Core;
using namespace Media;
using namespace Models;

Library::~Library() { declineStateRestoring(); }

void Library::setItemState(const QModelIndex & ind, int state) {
    bool override = state == flag_not_liked;
    if (state & flag_listened || override)
        state = 0;
    else if (state & flag_liked)
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

    ModelCell & cell = waitOnProc[ind.model()];
    cell.append(ind, is_remote, waitListLimit.value(ind.model(), WAIT_LIMIT));

    if (inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}

void Library::declineStateRestoring(const QModelIndex & ind) {
    QFutureWatcherBase * watcher = 0;

    if (inProc.contains(ind)) watcher = inProc.value(ind);
    if (!watcher && inRemoteProc.contains(ind)) watcher = inRemoteProc.value(ind);

    if (watcher) {
        watcher -> cancel();
        watcher -> waitForFinished();
        return;
    }

    waitOnProc[ind.model()].remove(ind);
}

void Library::declineStateRestoring(const QAbstractItemModel * model) {
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("DeclineItemRestoration"));
    waitOnProc.take(model).clear();
//    QList<QModelIndex> items = waitOnProc.take(model).order;

//    for(QList<QModelIndex>::iterator item = items.begin(); item != items.end(); item = items.erase(item))
//        indToItm(item.key()) -> unset(ItemFields::proceeded);

    cancelActiveRestorations();
}

void Library::declineStateRestoring() {
    QList<const QAbstractItemModel *> keys = waitOnProc.keys();
    for(QList<const QAbstractItemModel *>::ConstIterator key = keys.constBegin(); key != keys.constEnd(); key++)
        declineStateRestoring(*key);
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


bool Library::nextProcItem(ItemsListType iType, QModelIndex & ind) { // need to think about priority for current visible lists
    if (!waitOnProc.isEmpty()) {

        QList<const QAbstractItemModel *> keys = waitOnProc.keys();
        for(QList<const QAbstractItemModel *>::Iterator it = keys.begin(); it != keys.end(); it++) {
            ModelCell & cell = waitOnProc[*it];
            QList<QModelIndex> & list = cell.waitLists[iType];

            if (cell.isEmpty())
                waitOnProc.remove(*it);
            else if (!list.isEmpty() && listSyncs[*it] -> tryLock()) {
                ind = list.takeLast();
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
                    itm -> set(flag_liked);
                else
                    emitItemAttrChanging(ind, flag_liked);

                if (watcher) listSyncs[ind.model()] -> unlock();
                return;
            }

            isListened |= (state == 0);
        }
    }

    bool canceled = !watcher || (watcher && watcher -> isCanceled());
    if (isListened) {
        if (canceled)
            itm -> set(flag_listened);
        else
            emitItemAttrChanging(ind, flag_listened);
    } else
        if (!canceled)
            emitItemAttrChanging(ind, flag_new_item);

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
}



void Library::remoteItemInfo() {
    QModelIndex ind;
    if (nextProcItem(remote_items, ind)) {
        Logger::obj().write(QStringLiteral("Library"), QStringLiteral("InitRemoteInfo"), ind.data().toString());
        QFutureWatcher<bool> * initiator = new QFutureWatcher<bool>();
        inRemoteProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishRemoteItemInfo()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::remoteInfoRestoring, ind, initiator));
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

bool Library::remoteInfoRestoring(const QModelIndex & ind, QFutureWatcher<bool> * watcher) {
    IItem * itm = indToItm(ind);
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("RemoteInfoRestoring"), itm -> title().toString());
    bool has_info = itm -> hasInfo();

    if (has_info) {
        listSyncs[ind.model()] -> unlock();
        return false;
    }

    QUrl item_url = itm -> toUrl();
    if (item_url.isEmpty()) // some elements did not contains valid urls on moment of call
        if (IModel::restoreUrl(itm))
            item_url = itm -> toUrl();

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

void Library::initItemData(IItem * itm, bool dont_force_remote) {
    bool has_titles = itm -> titlesCache().isValid();
    bool has_info = itm -> hasInfo() || (dont_force_remote ? (itm -> isRemote() && Settings::obj().isUsedDelayForRemote()) : false);

    if (has_titles && has_info) return;

    MediaInfo m(itm -> toUrl(), itm -> extension(), has_info);
    if (m.hasError()) return;
    if (!has_titles) initItemTitles(&m, itm);
    if (!has_info) initItemInfo(&m, itm);
}

void Library::initItemInfo(MediaInfo * info, IItem * itm) {
    qint64 iSize = info -> getSize();

    qint64 duration = info -> getDuration();
    if (!itm -> duration().isValid() && duration > 0)
        itm -> setDuration(Duration::fromMillis(duration - itm -> startPos().toLongLong()));

    if (itm -> isPartial()) { //TODO: need to calculate size of parts
        iSize = 0;
    }

    itm -> setSize(iSize);

    if (info -> isReaded())
        itm -> setInfo(Info::str(Info::toUnits(iSize), info -> getExtension(), info -> getBitrate(), info -> getSampleRate(), info -> getChannels()));
    else
        itm -> setInfo(Info::str(Info::toUnits(iSize), info -> getExtension()));

    itm -> setGenreID(info -> getGenre());

    if (!info -> getExtension().isEmpty())
        itm -> setExtension(info -> getExtension());
}

void Library::initItemTitles(MediaInfo * info, IItem * itm) {
    QString title = itm -> title().toString();
    title = FilenameConversions::extraSymbolsFilter(title);
    itm -> setTitle(title);

    QStringList list;
    title = FilenameConversions::cacheTitleFilter(title);
    list.append(title);

    QString temp = FilenameConversions::forwardNumberFilter(title);
    if (temp != title)
        list.append(temp);

    if (!itm -> isPartial()) {
        QString tagTitle = FilenameConversions::cacheTitleFilter(info -> getArtist() + info -> getTitle());
        if (!tagTitle.isEmpty() && tagTitle != title && tagTitle != temp)
            list.append(tagTitle);
    }

    itm -> setTitlesCache(list);
}
