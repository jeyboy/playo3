#ifndef LIBRARY_H
#define LIBRARY_H

#include <qstringbuilder.h>
#include <qabstractitemmodel.h>
#include <qfile.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/QtConcurrent>
#include <qdebug.h>

#include "mediainfo.h"
#include "misc/file_utils/filename_conversions.h"
#include "media/format.h"
#include "media/duration.h"
#include "modules/data_struct/model_interface.h"

#define INPROC_LIMIT 5
#define TIMER_TICK 500
#define WAIT_LIMIT 10

class MediaInfo;

namespace Playo3 {
    class Library : public QObject {
        Q_OBJECT
    public:
        static Library * instance(QObject * parent = 0);
        inline static void close() { delete self; }

        void setItemState(const QModelIndex & ind, int state);

        void restoreItemState(const QModelIndex & ind);
        void declineItemStateRestoring(const QModelIndex & ind);
        void declineAllItemsRestoration(const QAbstractItemModel * model);

        inline void setWaitListLimit(QAbstractItemModel * model, int newLimit) { waitListLimit[model] = newLimit; }

        inline void registerListSync(const QAbstractItemModel * model, QMutex * sync) { listSyncs[model] = sync; }
        void unregisterListSync(const QAbstractItemModel * model) {
            declineAllItemsRestoration(model);
            listSyncs.remove(model);
        }
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

        inline QString libraryPath() { return QCoreApplication::applicationDirPath() % QStringLiteral("/library/"); }

        bool proceedItemNames(QStringList names, int state);
        QChar getCatalogName(QString name);

        QHash<QString, int> * getCatalog(QChar & letter);
        QHash<QString, int> * getCatalog(QString & name);

        void stateRestoring(QFutureWatcher<void> * initiator, QModelIndex ind);
        bool remoteInfoRestoring(QFutureWatcher<bool> * initiator, QModelIndex ind);

        void initItemData(IItem * itm);
        void initItemInfo(MediaInfo * info, IItem * itm);
        void initItemTitles(MediaInfo * info, IItem * itm);

        QHash<QString, int> * load(const QChar letter);
        void save();
        bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

        QHash<QChar, QHash<QString, int>* > catalogs;
        QHash<QChar, QList<QString> *> catalogs_state;

        QHash<const QAbstractItemModel *, QMutex *> listSyncs;

        QHash<const QAbstractItemModel *, int > waitListLimit;

        QHash<const QAbstractItemModel *, QList<QModelIndex> > waitOnProc;
        QHash<QModelIndex, QFutureWatcher<void> *> inProc;

        QHash<const QAbstractItemModel *, QList<QModelIndex> > waitRemoteOnProc;
        QHash<QModelIndex, QFutureWatcher<bool> *> inRemoteProc;

        QTimer * saveTimer;
        QMutex saveBlock;

        QFuture<void> catsSaveResult;

        int inProcLimit;
        int timeAmount;
    };
}

#endif // LIBRARY_H
