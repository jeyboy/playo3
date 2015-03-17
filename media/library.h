#ifndef LIBRARY_H
#define LIBRARY_H

#include <qabstractitemmodel.h>
#include <qfile.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfuturewatcher.h>

//#include <QHash>
//#include <QThread>
//#include <QtConcurrent/QtConcurrent>
//#include <QApplication>
//#include <QVector>

//#include "media/media_player/utils/genre.h"
#include "mediainfo.h"
//#include "media/media_player/utils/duration.h"
//#include "misc/func_container.h"

class Library : public QObject {
    Q_OBJECT
public:
    static Library * instance(QObject parent = 0);

//    void clearRemote();
//    void removeRemoteItem(ModelItem * item);
//    void initItem(ModelItem * item, const QObject * caller, const char * slot);
//    bool addItem(ModelItem * item, int state);
//    void restoreItemState(ModelItem * item);

//    void setRemoteItemMax(int newMax);

private slots:
    void saveCatalogs();
//    void startRemoteInfoProc();

private:
    static Library * self;

    Library(QObject * parent) : QObject(parent), remote_items_max(10) {
        catalogs = new QHash<QChar,  QHash<QString, int>* >();
        catalogs_state = QHash<QChar, QList<QString> *>();
        catsSaveResult = QFuture<void>();

        saveTimer = new QTimer();
        QObject::connect(saveTimer, SIGNAL(timeout()), this, SLOT(saveCatalogs()));
        saveTimer -> start(10000);

        QObject::connect(&remoteTimer, SIGNAL(timeout()), this, SLOT(startRemoteInfoProc()));
        remoteTimer.start(2000);

        QDir dir(libraryPath());
        if (!dir.exists())
            dir.mkpath(".");
    }

    ~Library() {
        timer -> stop();
        save();

        delete catalogs;
        delete timer;
    }

//    ModelItem * procRemoteInfo(ModelItem * item);
//    ModelItem * itemsInit(ModelItem * item);

    QString libraryPath();

    QString prepareName(QString gipoTitle, bool additional = false);
    bool proceedItemNames(QList<QString> * names, int state);

    QChar getCatalogChar(QString name);

    QHash<QString, int> * getCatalog(QChar letter);
    QHash<QString, int> * getCatalog(QString name);

//    void initItemInfo(ModelItem * item);

    QHash<QString, int> * load(const QChar letter);
    void save();
    bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

    QHash<QChar, QHash<QString, int>* > * catalogs;
    QHash<QChar, QList<QString> *> catalogs_state;
//    QHash<ModelItem *, FuncContainer> remote_collations;
//    QList<ModelItem *> remote_items;
//    int remote_items_max;

//    ModelItem * currRemote;
    QTimer * saveTimer;
    QTimer remoteTimer;
    QMutex saveBlock;

    QFuture<void> catsSaveResult;
};

#endif // LIBRARY_H
