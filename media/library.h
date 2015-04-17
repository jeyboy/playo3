#ifndef LIBRARY_H
#define LIBRARY_H

#include <qabstractitemmodel.h>
#include <qfile.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/QtConcurrent>

#include "mediainfo.h"
#include "misc/file_utils/filename_conversions.h"
#include "media/format.h"
#include "media/duration.h"
#include "modules/data_struct/model_interface.h"

#define INPROC_LIMIT 5
#define TIMER_TICK 500

namespace Playo3 {
    class Library : public QObject {
        Q_OBJECT
    public:
        static Library * instance(QObject * parent = 0);

        void setItemState(const QModelIndex & ind, int state);

        void restoreItemState(const QModelIndex & ind);
        void declineItemStateRestoring(const QModelIndex & ind);
        void declineAllItemsRestoration();

        inline void setWaitListLimit(int newLimit) { waitListLimit = newLimit; }
    signals:
        void updateAttr(const QModelIndex, int attr, QVariant val);
    private slots:
        void initStateRestoring();
        void finishStateRestoring();
        void initRemoteItemInfo();
        void finishRemoteItemInfoInit();

        void clockTick();
        void saveCatalogs();
    private:
        void cancelActiveRestorations();
        IItem * indToItm(const QModelIndex & ind);
        void emitItemAttrChanging(QModelIndex & ind, int state);
        static Library * self;

        Library(QObject * parent);

        ~Library();

        inline QString libraryPath() { return QCoreApplication::applicationDirPath() + "/library/"; }

        bool proceedItemNames(QStringList names, int state);
        QChar getCatalogName(QString name);

        QHash<QString, int> * getCatalog(QChar & letter);
        QHash<QString, int> * getCatalog(QString & name);

        void stateRestoring(QFutureWatcher<void> * initiator, QModelIndex ind);
        bool remoteInfoRestoring(QFutureWatcher<bool> * initiator, QModelIndex ind);

        void initItemData(IItem * itm);
        void initItemInfo(MediaInfo & info, IItem * itm);
        void initItemTitles(MediaInfo & info, IItem * itm);

        QHash<QString, int> * load(const QChar letter);
        void save();
        bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

        QHash<QChar, QHash<QString, int>* > catalogs;
        QHash<QChar, QList<QString> *> catalogs_state;

//        QHash<QAbstractItemModel *, QList<QModelIndex> > waitOnProc;
        QList<QModelIndex> waitOnProc;
        QHash<QModelIndex, QFutureWatcher<void> * > inProc;

        QList<QModelIndex> waitRemoteOnProc;
        QHash<QModelIndex, QFutureWatcher<bool> * > inRemoteProc;

        QTimer * saveTimer;
        QMutex saveBlock, itmAttrsBlock;

        QFuture<void> catsSaveResult;

        int inProcLimit;
        int waitListLimit;
        int timeAmount;
    };
}

#endif // LIBRARY_H
