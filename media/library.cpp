#include "library.h"
#include <qdebug.h>

using namespace Playo3;

Library * Library::self = 0;

Library * Library::instance(QObject * parent) {
    if(!self)
        self = new Library(parent);
    return self;
}

Library::Library(QObject * parent) : QObject(parent), waitListLimit(10), ticksAmount(0) {
    catsSaveResult = QFuture<void>();

    saveTimer = new QTimer();
    QObject::connect(saveTimer, SIGNAL(timeout()), this, SLOT(clockTick()));
    saveTimer -> start(1000);

    QDir dir(libraryPath());
    if (!dir.exists())
        dir.mkpath(".");
}

Library::~Library() {
    waitOnProc.clear();
    waitRemoteOnProc.clear();

    foreach(QFutureWatcher<void> * feature, inProc.values())
        if (feature -> isRunning())
            feature -> cancel();

    foreach(QFutureWatcher<void> * feature, inRemoteProc.values())
        if (feature -> isRunning())
            feature -> cancel();

    saveTimer -> stop();
    save();

    delete saveTimer;
}

void Library::setItemState(const QModelIndex & ind, int state) {
    if (state & ItemFields::liked)
        state = 1;
    else if (state & ItemFields::listened)
        state = 0;
    else return;

    IItem * itm = indToItm(ind);
    initItemData(itm);

    proceedItemNames(itm -> titlesCache().toStringList(), state);
}

void Library::restoreItemState(const QModelIndex & ind) {
    if (!waitOnProc.contains(ind)) {
        waitOnProc.append(ind);
        while(waitOnProc.size() > waitListLimit) {
            IItem * itm = indToItm(waitOnProc.takeFirst());
            itm -> unset(ItemFields::proceeded);
        }

        if (ind.data(IREMOTE).toBool()) {
            waitRemoteOnProc.append(ind);
            while(waitRemoteOnProc.size() > waitListLimit)
                waitRemoteOnProc.removeFirst();
        }
    }

    if (inProc.size() < INPROC_LIMIT)
        initStateRestoring();
}

void Library::declineItemStateRestoring(const QModelIndex & ind) {
    if (inProc.contains(ind))
        inProc.value(ind) -> cancel();
    else waitOnProc.removeAll(ind);
}

void Library::initRemoteItemInfo() {
    if (!waitRemoteOnProc.isEmpty()) {
        QFutureWatcher<void> * initiator = new QFutureWatcher<void>();
        connect(initiator, SIGNAL(finished()), this, SLOT(finishRemoteItemInfoInit()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::remoteInfoRestoring, waitRemoteOnProc.takeLast()));
    }
}

void Library::finishRemoteItemInfoInit() {
    QFutureWatcher<void> * initiator = (QFutureWatcher<void> *)sender();
    QModelIndex ind = inRemoteProc.key(initiator);
    inRemoteProc.remove(ind);
    delete initiator;
}

void Library::initStateRestoring() {
    if (!waitOnProc.isEmpty()) {
        QFutureWatcher<void> * initiator = new QFutureWatcher<void>();
        QModelIndex ind = waitOnProc.takeLast();
        inProc.insert(ind, initiator);
        connect(initiator, SIGNAL(finished()), this, SLOT(finishStateRestoring()));
        initiator -> setFuture(QtConcurrent::run(this, &Library::stateRestoring, ind));
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

void Library::stateRestoring(QModelIndex ind) {
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
                emitItemAttrChanging(ind, ItemState::liked);
                return;
            }

            isListened |= (state == 0);
        }
    }

    if (isListened)
        emitItemAttrChanging(ind, ItemState::listened);
    else
        emitItemAttrChanging(ind, ItemState::new_item);
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

void Library::remoteInfoRestoring(QModelIndex ind) {
    IItem * itm = indToItm(ind);
    qDebug() << "REMOTE RESTORING " << itm -> title();
    bool has_info = itm -> hasInfo();

    if (has_info) return;

    MediaInfo m(itm -> fullPath(), has_info);

    initItemInfo(m, itm);
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
    if (++ticksAmount > 999999999) ticksAmount = 0;

    int saveLibDelay = Settings::instance() -> saveLibDelay();

    if (saveLibDelay != 0 && ticksAmount % saveLibDelay == 0)
        saveCatalogs();


    if (ticksAmount % Settings::instance() -> remoteItemsProcDelay() == 0)
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
            if (catalog_state_has_item) {
                saveList = catalogs_state.value(letter);
//                if (saveList != 0)
//                    qDebug() << "LLL: " << letter << " Libb: " << (*saveList);
            }

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

////QList<QString> * Library::getNamesForObject(QString path, QString name) {
////    QList<QString> * res = new QList<QString>();

////    res -> append(name);
////    QString temp = prepareName(name, true);
////    if (temp != name)
////        res -> append(temp);

////    MediaInfo m(path);

////    QString temp2 = prepareName(m.getArtist() + m.getTitle());
////    if (!temp2.isEmpty() && temp2 != name && temp2 != temp)
////        res -> append(temp2);

////    return res;
////}

void Library::initItemData(IItem * itm) {
    bool has_titles = itm -> titlesCache().isValid();
    bool has_info = itm -> hasInfo();

    if (has_titles && has_info) return;

    MediaInfo m(itm -> toUrl(), has_info);

    if (!has_titles) initItemTitles(m, itm);
    if (!has_info) initItemInfo(m, itm);
}

void Library::initItemInfo(MediaInfo & info, IItem * itm) {
    itm -> setSize(info.getSize());
    qDebug() << itm -> title() << " ||| " << info.getSize() << " : " << info.getBitrate() << " : " << info.getSampleRate() << " : " << info.getChannels();
    itm -> setInfo(Format::toInfo(Format::toUnits(info.getSize()), info.getBitrate(), info.getSampleRate(), info.getChannels()));
    itm -> setDuration(Duration::fromSeconds(info.getDuration()));
    itm -> setGenre(MusicGenres::instance() -> toInt(info.getGenre()));
}

void Library::initItemTitles(MediaInfo & info, IItem * itm) {
    QStringList list;
    QString title = cacheTitleFilter(itm -> title().toString());
    list.append(title);

    QString temp = forwardNumberFilter(title);
    if (temp != title)
        list.append(temp);

    QString tagTitle = cacheTitleFilter(info.getArtist() + info.getTitle());
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
    if (saveBlock.tryLock(-1)) {
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
