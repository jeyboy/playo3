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

            enum ItemsListType {
                all_items = 0,
                local_items,
                remote_items
            };

            friend class Singleton<Library>;
            inline Library() {
                remoteProcTimer = new QTimer();
                QObject::connect(remoteProcTimer, SIGNAL(timeout()), this, SLOT(remoteItemInfo()));
                remoteProcTimer -> start(Settings::obj().remoteItemsProcDelay());
            }

            bool nextProcItem(ItemsListType iType, QModelIndex & ind);

            void cancelActiveRestorations();

            void stateRestoring(const QModelIndex & ind, QFutureWatcher<void> * initiator = 0);
            bool remoteInfoRestoring(const QModelIndex & ind, QFutureWatcher<bool> * initiator);

            IItem * indToItm(const QModelIndex & ind);
            void emitItemAttrChanging(const QModelIndex & ind, int state);

            void initItemData(IItem * itm);
            void initItemInfo(MediaInfo * info, IItem * itm);
            void initItemTitles(MediaInfo * info, IItem * itm);

            bool proceedItemTitles(IItem * itm, int state, bool override = false);

            QHash<const QAbstractItemModel *, QMutex *> listSyncs;
            QHash<const QAbstractItemModel *, int> waitListLimit;

            QHash<const QAbstractItemModel *, QHash<ItemsListType, QHash<QModelIndex, bool> > > waitOnProc;
            QHash<QModelIndex, QFutureWatcher<void> *> inProc;
            QHash<QModelIndex, QFutureWatcher<bool> *> inRemoteProc;

            QTimer * remoteProcTimer;
        public:
            ~Library();

            void setItemState(const QModelIndex & ind, int state);

            void restoreItemState(const QModelIndex & ind);
            void restoreItemStateAsync(const QModelIndex & ind, bool is_remote);
            void declineItemStateRestoring(const QModelIndex & ind);
            void declineItemStateRestoring(const QAbstractItemModel * model);
            void declineItemStateRestoring();

            inline void setWaitListLimit(QAbstractItemModel * model, int newLimit) { waitListLimit[model] = newLimit; }

            inline void registerListSync(const QAbstractItemModel * model, QMutex * sync) { listSyncs[model] = sync; }
            void unregisterListSync(const QAbstractItemModel * model) {
                declineItemStateRestoring(model);
                listSyncs.remove(model);
            }
        signals:
            void updateAttr(const QModelIndex &, int attr, const QVariant & val);
        private slots:
            void initStateRestoring();
            void finishStateRestoring();
            void remoteItemInfo();
            void finishRemoteItemInfo();
        };
    }
}

#endif // LIBRARY_H
