#ifndef DATA_CORE_H
#define DATA_CORE_H

#include <qdebug.h>
#include <qobject.h>
#include <qhash.h>

#include "core_parts_index.h"
#include "player/player_index.h"
#include "modules/core/media/library.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/interfaces/iplaylistable.h"

#define MAX_ATTEMPTS 1

namespace Core {
    class DataFactory : public QObject, public QHash<QString, IItem *>, public Singleton<DataFactory> {
        Q_OBJECT

        IPlaylistable * current_playlist;
        IItem * current_item;

        PlayerInitState init_state_flag;
        int attempts;
    protected:
        void spoil();
        void setState(int state);
        void setError(ItemErrors error);

        void playNext(bool onFail = false);
        void restoreOrNext();
        void proceedStalledState();
    public:
        inline DataFactory() : QObject(), current_playlist(0), current_item(0), attempts(0) {
            PlayerFactory::obj().registerCallback(out, this, SIGNAL(statusChanged(PlayerStatus)), SLOT(playerStatusChanged(PlayerStatus)));
        }
        ~DataFactory() {}

        inline IPlayer * currPlayer() { return PlayerFactory::obj().currPlayer(); }

        inline QModelIndex playedIndex() { return current_playlist ? current_playlist -> index(current_item) : QModelIndex(); }
        inline IPlaylistable * currentPlaylist() const { return current_playlist; }
        inline IItem * playedItem() { return current_item; }
        inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

        void resetPlaying() { proceedPlaying(0, 0); }

        void proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili = 0, PlayerInitState state = played);

        void proceedStoping() { currPlayer() -> stop(); }
        void proceedPausing() { currPlayer() -> pause(); }
        void proceedPauseToggling() { currPlayer() -> playPause(); }

    signals:
        void newPlaylistNeed();
        void likeChanged(bool);

    public slots:
        void changeLikeStatus(bool is_liked) { setState((is_liked ? 1 : -1) * ItemState::liked); }

        void playerStatusChanged(const PlayerStatus & status);

        void registerSync(QAbstractItemModel * mdl, QMutex * mutex) {
            Library::obj().registerListSync(mdl, mutex);
        }

        void unregisterSync(QAbstractItemModel * mdl) {
            Library::obj().unregisterListSync(mdl);
        }

        void discardSync(QAbstractItemModel * mdl) {
            Library::obj().declineStateRestoring(mdl);
        }


        void changeCadrSize(QAbstractItemModel * mdl, int newCadrSize) {
            Library::obj().setWaitListLimit(mdl, newCadrSize);
        }

        void proceedInfo(const QModelIndex & ind) {
            Library::obj().restoreItemState(ind);
        }

        void proceedInfoAsync(const QModelIndex & ind);
    };
}

#endif // DATA_CORE_H
