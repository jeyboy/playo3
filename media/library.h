#ifndef LIBRARY_H
#define LIBRARY_H

#include <qabstractitemmodel.h>
#include <qfile.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/QtConcurrent>

//#include <QHash>
//#include <QThread>
//#include <QApplication>
//#include <QVector>

//#include "media/media_player/utils/genre.h"
#include "mediainfo.h"
#include "misc/file_utils/filename_conversions.h"
//#include "media/media_player/utils/duration.h"
//#include "misc/func_container.h"
#include "modules/data_struct/model_interface.h"

namespace Playo3 {
    class Library : public QObject {
        Q_OBJECT
    public:
        static Library * instance(QObject * parent = 0);

        void restoreItemState(QModelIndex & ind);
        void declineItemState(QModelIndex & ind);

    //    void clearRemote();
    //    void removeRemoteItem(ModelItem * item);
    //    void initItem(ModelItem * item, const QObject * caller, const char * slot);
    //    bool addItem(ModelItem * item, int state);
    //    void restoreItemState(ModelItem * item);

    //    void setRemoteItemMax(int newMax);

        inline void setWaitListLimit(int newLimit) { waitListLimit = newLimit; }
    signals:
        void updateAttr(const QModelIndex, int attr, QVariant val);

    private slots:
        void initStateRestoring();
        void finishStateRestoring();
        void saveCatalogs();
    //    void startRemoteInfoProc();

    private:
        static Library * self;

        Library(QObject * parent);

        ~Library();

    //    ModelItem * procRemoteInfo(ModelItem * item);
    //    ModelItem * itemsInit(ModelItem * item);

        inline QString libraryPath() { return QCoreApplication::applicationDirPath() + "/library/"; }

        bool proceedItemNames(QStringList & names, int state);
        QChar getCatalogName(QString name);

        QHash<QString, int> * getCatalog(QChar & letter);
        QHash<QString, int> * getCatalog(QString & name);

        void stateRestoring(QModelIndex ind);
        void initItemInfo(IItem * itm);

        QHash<QString, int> * load(const QChar letter);
        void save();
        bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

        QHash<QChar, QHash<QString, int>* > catalogs;
        QHash<QChar, QList<QString> *> catalogs_state;

        QList<QModelIndex> waitOnProc;
        QHash<QModelIndex, QFutureWatcher<void> * > inProc;

    //    QHash<ModelItem *, FuncContainer> remote_collations;
    //    QList<ModelItem *> remote_items;
    //    int remote_items_max;

    //    ModelItem * currRemote;
        QTimer * saveTimer;
    //    QTimer remoteTimer;
        QMutex saveBlock;

        QFuture<void> catsSaveResult;

        int waitListLimit;
    };
}

#endif // LIBRARY_H
