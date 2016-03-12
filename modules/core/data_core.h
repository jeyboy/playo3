#ifndef DATA_CORE_H
#define DATA_CORE_H

#include <qobject.h>

#include "data_factory.h"
#include "core_parts/item.h"
#include "player/player_index.h"
#include "modules/core/media/library.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/interfaces/iplaylistable.h"

#define MAX_ATTEMPTS 1

namespace Core {
    class DataCore : public QObject, public DataFactory, public Singleton<DataCore> {
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
        bool useNextSource();
        void playNewSource() {
            qDebug() << "FIND SOURCE OR RESTORE";
            IPlayer * player = currPlayer();
            player -> updateMedia(current_item -> toUrl());
            player -> play(init_state_flag == paused);
        }
    public:
        inline DataCore() : QObject(), current_playlist(0), current_item(0), attempts(0) {
            PlayerFactory::obj().registerCallback(out, this, SIGNAL(statusChanged(PlayerStatus)), SLOT(playerStatusChanged(PlayerStatus)));
        }

        inline IPlayer * currPlayer() { return PlayerFactory::obj().currPlayer(); }

        inline QModelIndex playedIndex() { return current_playlist ? current_playlist -> index(current_item) : QModelIndex(); }
        inline IPlaylistable * currentPlaylist() const { return current_playlist; }
        inline IItem * playedItem() { return current_item; }
        inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

        void resetPlaying() { proceedPlaying(0, 0); }

        void proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili = 0, PlayerInitState state = played, bool fixSourceLimit = false);

        void proceedStoping() { currPlayer() -> stop(); }
        void proceedPausing() { currPlayer() -> pause(); }
        void proceedPauseToggling() { currPlayer() -> playPause(); }
    signals:
        void newPlaylistNeed();
        void likeChanged(bool);

    public slots:
        void changeLikeStatus(bool is_liked) { setState(is_liked ? flag_liked : flag_not_liked); }

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
