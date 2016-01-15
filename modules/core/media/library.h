#ifndef LIBRARY_H
#define LIBRARY_H

#include <qabstractitemmodel.h>
#include <qdebug.h>

#include "modules/core/media/mediainfo.h"
#include "modules/core/misc/format.h"
#include "modules/models/model_interface.h"

#include "library_base.h"

#include "modules/core/interfaces/singleton.h"

#define INPROC_LIMIT 5
#define TIMER_TICK 500
#define WAIT_LIMIT 10

namespace Core {
    namespace Media {
        class MediaInfo;

        class Library : public LibraryBase, public Singleton<Library> {
            Q_OBJECT
        public:
            ~Library();

            void stopProcessing();

            void setItemState(const QModelIndex & ind, int state);

            void directItemStateRestoration(const QModelIndex & ind);
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
        private:
            Library();

            void cancelActiveRestorations();
            IItem * indToItm(const QModelIndex & ind);
            void emitItemAttrChanging(QModelIndex & ind, int state);

            bool proceedItemNames(IItem * itm, int state, bool override = false);

            void stateRestoring(QFutureWatcher<void> * initiator, QModelIndex ind);
            bool remoteInfoRestoring(QFutureWatcher<bool> * initiator, QModelIndex ind);

            void initItemData(IItem * itm);
            void initItemInfo(MediaInfo * info, IItem * itm);
            void initItemTitles(MediaInfo * info, IItem * itm);

            QHash<const QAbstractItemModel *, QMutex *> listSyncs;

            QHash<const QAbstractItemModel *, int > waitListLimit;

            QHash<const QAbstractItemModel *, QList<QModelIndex> > waitOnProc;
            QHash<QModelIndex, QFutureWatcher<void> *> inProc;

            QHash<const QAbstractItemModel *, QList<QModelIndex> > waitRemoteOnProc;
            QHash<QModelIndex, QFutureWatcher<bool> *> inRemoteProc;

            int inProcLimit;
        };
    }
}

#endif // LIBRARY_H
