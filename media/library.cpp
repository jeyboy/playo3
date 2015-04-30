#include "library.h"
#include "misc/logger.h"

using namespace Playo3;

Library * Library::self = 0;

Library * Library::instance(QObject * parent) {
    if(!self)
        self = new Library(parent);
    return self;
}

Library::Library(QObject * parent) : QObject(parent), timeAmount(0) {
    catsSaveResult = QFuture<void>();

    saveTimer = new QTimer();
    QObject::connect(saveTimer, SIGNAL(timeout()), this, SLOT(clockTick()));
    saveTimer -> start(TIMER_TICK);

    QDir dir(libraryPath());
    if (!dir.exists())
        dir.mkpath(".");
}

Library::~Library() {   
    waitOnProc.clear();
    waitRemoteOnProc.clear();

    cancelActiveRestorations();

    saveTimer -> stop();
    save();

    delete saveTimer;
}

void Library::setItemState(const QModelIndex & ind, int state) {
    if (state & ItemFields::liked)
        state = 1;
    else if (state & ItemFields::listened || state == -ItemFields::liked)
        state = 0;
    else return;

    IItem * itm = indToItm(ind);
    initItemData(itm);

    proceedItemNames(itm -> titlesCache().toStringList(), state);
}

void Library::restoreItemState(const QModelIndex & ind) {
//    if (!listSyncs.value(ind.model(), 0)) return;
    Logger::instance() -> write("Library", "RestoreItem", ind.data().toString());

    QList<QModelIndex> & list = waitOnProc[ind.model()];

    if (!list.contains(ind)) {
        QList<QModelIndex> & remote_list = waitRemoteOnProc[ind.model()];
        list.append(ind);

        while(list.size() > waitListLimit.value(ind.model(), WAIT_LIMIT)) {
            QModelIndex i = list.takeFirst();
            if (i.data(IREMOTE).toBool())
                remote_list.removeOne(i);
            IItem * itm = indToItm(i);
            itm -> unset(ItemFields::proceeded);
        }

        if (Settings::instance() -> isUsedDelayForRemote())
            if (ind.data(IREMOTE).toBool()) {
                remote_list.append(ind);

                while(remote_list.size() > waitListLimit.value(ind.model(), WAIT_LIMIT)) {
                    IItem * itm = indToItm(remote_list.takeFirst());
                    itm -> unset(ItemFields::proceeded);
                }
            }
    }

    if (inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}

void Library::declineItemStateRestoring(const QModelIndex & ind) {
    if (inProc.contains(ind))
        inProc.value(ind) -> cancel();
    else waitOnProc[ind.model()].removeAll(ind);
}

void Library::declineAllItemsRestoration(const QAbstractItemModel * model) {
    Logger::instance() -> write("Library", "DeclineItemRestoration", model -> objectName());
    QList<QModelIndex> items = waitOnProc.take(model);
    QList<QModelIndex> remote_items = waitRemoteOnProc.take(model);

    while(!items.isEmpty()) {
        QModelIndex i = items.takeFirst();
        if (i.data(IREMOTE).toBool())
            remote_items.removeOne(i);
        IItem * itm = indToItm(i);
        itm -> unset(ItemFields::proceeded);
    }

    while(!remote_items.isEmpty()) {
        QModelIndex i = remote_items.takeFirst();
        IItem * itm = indToItm(i);
        itm -> unset(ItemFields::proceeded);
    }

    cancelActiveRestorations();
}

void Library::initRemoteItemInfo() {
    if (!waitRemoteOnProc.isEmpty()) {

        const QAbstractItemModel * key = 0;

        QList<const QAbstractItemModel *> keys = waitRemoteOnProc.keys();
        QList<const QAbstractItemModel *>::Iterator it = keys.begin();

        for(; it != keys.end(); it++) {
            if (listSyncs[(*it)] -> tryLock()) {
                qDebug() << "L LOCK";
                key = (*it);
                break;
            }
        }

        if (!key) return;

        QList<QModelIndex> & list = waitRemoteOnProc[key];

        if (list.isEmpty()) {
            waitRemoteOnProc.remove(key);
            listSyncs[key] -> unlock();
            return;
        }

        QModelIndex ind = list.takeLast();
        Logger::instance() -> write("Library", "InitRemoteInfo", ind.data().toString());
        if (list.isEmpty()) waitRemoteOnProc.remove(key);

        QFutureWatcher<bool> * initiator = new QFutureWatcher<bool>();
        inRemoteProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishRemoteItemInfoInit()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::remoteInfoRestoring, initiator, ind));
    }
}

void Library::finishRemoteItemInfoInit() {
    QFutureWatcher<bool> * initiator = (QFutureWatcher<bool> *)sender();
    bool result = initiator -> isCanceled() || initiator -> result();
    QModelIndex ind = inRemoteProc.key(initiator);
    inRemoteProc.remove(ind);
    delete initiator;

    if (!result) // call new item initiation if prev item initiation is finished without remote call for info
        initRemoteItemInfo();
}

void Library::initStateRestoring() {
    if (!waitOnProc.isEmpty()) {
        const QAbstractItemModel * key = 0;

        QList<const QAbstractItemModel *> keys = waitOnProc.keys();
        QList<const QAbstractItemModel *>::Iterator it = keys.begin();

        for(; it != keys.end(); it++) {
            if (listSyncs[(*it)] -> tryLock()) {
                qDebug() << "L LOCK";
                key = (*it);
                break;
            }
        }

        if (!key) return;

        QList<QModelIndex> & list = waitOnProc[key];

        if (list.isEmpty()) {
            waitOnProc.remove(key);
            listSyncs[key] -> unlock();
            return;
        }

        QModelIndex ind = list.takeLast();
        Logger::instance() -> write("Library", "InitInfo", ind.data().toString());

        if (list.isEmpty()) waitOnProc.remove(key);

        QFutureWatcher<void> * initiator = new QFutureWatcher<void>();
        inProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishStateRestoring()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::stateRestoring, initiator, ind));
    }
}

void Library::finishStateRestoring() {
    QFutureWatcher<void> * initiator = (QFutureWatcher<void> *)sender();
    QModelIndex ind = inProc.key(initiator);
    inProc.remove(ind);
    delete initiator;

    if (inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}

void Library::stateRestoring(QFutureWatcher<void> * watcher, QModelIndex ind) {
    IItem * itm = indToItm(ind);
    qDebug() << "RESTORING " << itm -> title();
    initItemData(itm);

    QHash<QString, int> * cat;
    bool isListened = false;
    int state;
    QList<QString>::iterator i;
    QStringList titles = itm -> titlesCache().toStringList();

    for (i = titles.begin(); i != titles.end(); ++i) {
        if (!(cat = getCatalog((*i)))) continue;

        state = cat -> value((*i), -1);

        if (state != -1) {
            if (state == 1) {
                if (watcher -> isCanceled())
                    itm -> set(ItemState::liked);
                else
                    emitItemAttrChanging(ind, ItemState::liked);
                return;
            }

            isListened |= (state == 0);
        }
    }

    bool canceled = watcher -> isCanceled();
    if (isListened) {
        if (canceled)
            itm -> set(ItemState::listened);
        else
            emitItemAttrChanging(ind, ItemState::listened);
    } else
        if (!canceled)
            emitItemAttrChanging(ind, ItemState::new_item);

    qDebug() << "L UNLOCK";
    listSyncs[ind.model()] -> unlock();
}

void Library::cancelActiveRestorations() {
    {
        QList<QFutureWatcher<void> *> inProcList = inProc.values();
        QList<QFutureWatcher<void> *>::Iterator it = inProcList.begin();

        for(; it != inProcList.end(); it++)
            if ((*it) -> isRunning())
                (*it) -> cancel();
    }

    QList<QFutureWatcher<bool> *> inProcRemoteList = inRemoteProc.values();
    QList<QFutureWatcher<bool> *>::Iterator it = inProcRemoteList.begin();

    for(; it != inProcRemoteList.end(); it++)
        if ((*it) -> isRunning())
            (*it) -> cancel();
}

IItem * Library::indToItm(const QModelIndex & ind) {
    return (qobject_cast<const IModel *>(ind.model())) -> item(ind);
}

void Library::emitItemAttrChanging(QModelIndex & ind, int state) {
    connect(
        this, SIGNAL(updateAttr(QModelIndex,int,QVariant)),
        ind.model(), SLOT(onUpdateAttr(const QModelIndex,int,QVariant)), (Qt::ConnectionType)(Qt::BlockingQueuedConnection | Qt::UniqueConnection)
    );
    emit updateAttr(ind, ISTATERESTORE, state);
    disconnect(this, SIGNAL(updateAttr(QModelIndex,int,QVariant)), ind.model(), SLOT(onUpdateAttr(const QModelIndex,int,QVariant)));
}

bool Library::remoteInfoRestoring(QFutureWatcher<bool> * watcher, QModelIndex ind) {
    IItem * itm = indToItm(ind);
    qDebug() << "REMOTE RESTORING " << itm -> title();
    bool has_info = itm -> hasInfo();

    if (has_info) {
        qDebug() << "L UNLOCK";
        listSyncs[ind.model()] -> unlock();
        return false;
    }

    MediaInfo m(itm -> fullPath(), itm -> extension().isValid(), has_info);
    initItemInfo(&m, itm);

    if (!watcher -> isCanceled())
        emitItemAttrChanging(ind, ind.data(ISTATE).toInt());

    qDebug() << "L UNLOCK";
    listSyncs[ind.model()] -> unlock();
    return true;
}

//void Library::clearRemote() {
//    currRemote = 0;
//    remote_items.clear();
//    remote_collations.clear();
//}

//void Library::removeRemoteItem(ModelItem * item) {
//    if (item == currRemote) {
//        currRemote = 0;
//    } else {
//        remote_items.removeAll(item);
//        remote_collations.take(item);
//    }
//}

//void Library::initItem(ModelItem * item, const QObject * caller, const char * slot) {
//    QFutureWatcher<ModelItem *> * initiator = new QFutureWatcher<ModelItem *>();
//    connect(initiator, SIGNAL(finished()), caller, slot);
//    initiator -> setFuture(QtConcurrent::run(this, &Library::itemsInit, item));
//    if (item -> isRemote() && !item -> hasInfo()) {
//        remote_items.append(item);
//        remote_collations.insert(item, FuncContainer(caller, slot));

//        if (remote_items.count() > remote_items_max) {
//            ModelItem * i = remote_items.takeFirst();
//            remote_collations.take(i);
//            i -> getState() -> unsetProceed();
//        }
//    }
//}

//bool Library::addItem(ModelItem * item, int state) {
//    initItemInfo(item);

//    if (state == STATE_LIKED)
//        state = 1;
//    else state = 0;

//    return proceedItemNames(item -> getTitlesCache(), state);
//}

//void Library::setRemoteItemMax(int newMax) {
//    remote_items_max = newMax;
//}

////void Library::setItemState(const QString filename, int state) {
////    QHash<QString, int> cat = getCatalog(filename);
////    cat.insert(filename, state);
////    instance() -> catalogs_state.append(getCatalogName(filename.at(0)));
////}


////////////////////////////////////////////////////////////////////////
///// privates
////////////////////////////////////////////////////////////////////////
void Library::clockTick() {
    if ((timeAmount += TIMER_TICK) > 999999999) timeAmount = 0;

    int saveLibDelay = Settings::instance() -> saveLibDelay();

    if (saveLibDelay != 0 && timeAmount % saveLibDelay == 0)
        saveCatalogs();

    int procDelay = Settings::instance() -> remoteItemsProcDelay();
    if (procDelay == 0 || timeAmount % procDelay == 0)
        initRemoteItemInfo();
}

void Library::saveCatalogs() {
    if (!catsSaveResult.isRunning())
        catsSaveResult = QtConcurrent::run(this, &Library::save);
}

bool Library::proceedItemNames(QStringList names, int state) {
    QHash<QString, int> * cat;
    QChar letter;
    bool catState = false, catalog_has_item, catalog_state_has_item;
    QList<QString> * saveList;
    QList<QString>::iterator i;

    for (i = names.begin(); i != names.end(); ++i) {
        saveList = 0;
        letter = getCatalogName((*i));
        cat = getCatalog(letter);

        catalog_has_item = cat -> contains((*i));

        if (!catalog_has_item || (catalog_has_item && cat -> value((*i)) < state)) {
            cat -> insert((*i), state);

            catalog_state_has_item = catalogs_state.contains(letter);
            if (catalog_state_has_item)
                saveList = catalogs_state.value(letter);

            if (catalog_has_item) {
                if (saveList)
                    delete saveList;
                catalogs_state.insert(letter, 0);
            } else {
                if (!catalog_state_has_item) {
                    if (saveList == 0)
                        saveList = new QList<QString>();
                }

                saveList -> append((*i));
                catalogs_state.insert(letter, saveList);
            }
        }
    }

    return catState;
}

QChar Library::getCatalogName(QString name) {
    if (name.length() == 0) return '_';
    return name.at(0);
}

QHash<QString, int> * Library::getCatalog(QChar & letter) {
    if (catalogs.contains(letter)) {
        return catalogs.value(letter);
    } else {
        QHash<QString, int> * res = load(letter);
        catalogs.insert(letter, res);
        return res;
    }
}

QHash<QString, int> * Library::getCatalog(QString & name) {
    if (name.length() == 0) return 0;

    QChar c = getCatalogName(name);
    return getCatalog(c);
}

void Library::initItemData(IItem * itm) {
    bool has_titles = itm -> titlesCache().isValid();
    bool has_info = (itm -> isRemote() && Settings::instance() -> isUsedDelayForRemote()) || itm -> hasInfo();

    if (has_titles && has_info) return;

    MediaInfo m(itm -> toUrl(), itm -> extension().isValid(), has_info);

    if (!has_titles) initItemTitles(&m, itm);
    if (!has_info) initItemInfo(&m, itm);
}

void Library::initItemInfo(MediaInfo * info, IItem * itm) {
    itm -> setSize(info -> getSize());
    if (info -> isReaded())
        itm -> setInfo(Format::toInfo(Format::toUnits(info -> getSize()), info -> getBitrate(), info -> getSampleRate(), info -> getChannels()));
    else
        itm -> setInfo(Format::toUnits(info -> getSize()));
    if (info -> getDuration() > 0)
        itm -> setDuration(Duration::fromSeconds(info -> getDuration()));
    itm -> setGenre(info -> getGenre());

    if (!info -> getExtension().isEmpty())
        itm -> setExtension(info -> getExtension());
}

void Library::initItemTitles(MediaInfo * info, IItem * itm) {
    QStringList list;
    QString title = cacheTitleFilter(itm -> title().toString());
    list.append(title);

    QString temp = forwardNumberFilter(title);
    if (temp != title)
        list.append(temp);

    QString tagTitle = cacheTitleFilter(info -> getArtist() + info -> getTitle());
    if (!tagTitle.isEmpty() && tagTitle != title && tagTitle != temp)
        list.append(tagTitle);

    itm -> setTitlesCache(list);
}

QHash<QString, int> * Library::load(const QChar letter) {
    QHash<QString, int> * res = new QHash<QString, int>();

    QFile f(libraryPath() + "cat_" + letter);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar;
        QString name;
        int state;

        while(!f.atEnd()) {
            ar = f.readLine();
            state = ar.mid(0, 1).toInt();
            name = QString::fromUtf8(ar.mid(1, ar.length() - 3));
            res -> insert(name, state);
        }

        f.close();
    }

    return res;
}

void Library::save() {
    if (saveBlock.tryLock()) {
        QHash<QString, int> * res;
        QHash<QChar, QList<QString> *>::iterator i = catalogs_state.begin();

        bool result;

        while(i != catalogs_state.end()) {
            res = catalogs.value(i.key());

            if (i.value()) {
                result = fileDump(i.key(), *i.value(), QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

                if(result) {
                    delete catalogs_state.value(i.key());
                    catalogs_state.insert(i.key(), 0);
                }
            } else {
                QList<QString> keys = res -> keys();
                result = fileDump(i.key(), keys, QIODevice::WriteOnly | QIODevice::Text);
            }

            if (result)
                i = catalogs_state.erase(i);
            else
                i++;
        }

        saveBlock.unlock();
    }
}

bool Library::fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags) {
    QList<QString>::const_iterator cat_i = keysList.cbegin();
    QHash<QString, int> * res = catalogs.value(key);

    QFile f(libraryPath() + "cat_" + key);
    if (f.open(openFlags)) {
        QTextStream out(&f);
        out.setCodec("UTF-8");

        while(cat_i != keysList.cend()) {
            out << QString::number(res -> value(*cat_i)) << (*cat_i).toUtf8() << "\n";
            cat_i++;
        }

        f.close();

        return true;
    }

    return false;
}
