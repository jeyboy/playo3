#include "data_core.h"
#include "modules/models/service/search_model.h"

namespace Core {
    void DataCore::spoil() {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> spoil(playedIndex()))
            qDebug() << "STATE IS NOT CHANGED";
    }
    void DataCore::setState(int state) {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        qDebug() << "STATE" << state;
        if (!current_playlist -> setState(playedIndex(), state))
            qDebug() << "STATE IS NOT CHANGED";
    }

    void DataCore::setError(ItemErrors error) {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> setError(playedIndex(), error))
            qDebug() << "ERROR IS NOT ATTACHED";
    }

    void DataCore::playNext(bool onFail) {
        setState(flag_not_proccessing); // extra call for item clearing states!
        if (!current_playlist) {
            qDebug() << "NEXT: PLAYLIST IS UNDEFINED";
            return;
        }
        if (current_playlist -> isPlaylist()) {
            if (onFail && Settings::obj().isFindValid())
                if (++attempts > MAX_ATTEMPTS) {
                    attempts = 0;
                    return;
                }

            current_playlist -> execNextIndex();
        }
    }

    void DataCore::restoreOrNext() { // need to prevent form loop
        if (!current_playlist) {
            qDebug() << "RESTORE: PLAYLIST IS UNDEFINED";
            return;
        }

        bool item_updated = current_playlist -> restoreItem(current_item);
        if (!item_updated) {
            qDebug() << "RESTORE: FAILED";
            if (!(item_updated = current_item -> useNextSource())) {
                if (Settings::obj().isFindOtherSource())
                    item_updated = Models::SearchModel::findNewSource(current_item);
            }
        }

        if (!item_updated) {
            proceedStalledState();
            return;
        }

        qDebug() << "FIND SOURCE OR RESTORE";
        IPlayer * player = currPlayer();
        player -> updateMedia(current_item -> toUrl());
        player -> play(init_state_flag == paused);
    }

    void DataCore::proceedStalledState() {
        if (current_item)
            setError(current_item -> isRemote() ? ItemErrors::warn_not_accessable : ItemErrors::err_not_existed);
        playNext(true);
    }

    void DataCore::proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili, PlayerInitState state, bool fixSourceLimit) {
        IPlayer * player = currPlayer();

        bool continuePlaying = item && !player -> media().isEmpty() && item -> toUrl() == player -> media();

        if (!continuePlaying)
            player -> closeMedia();
        else {
            player -> pause();
            playerStatusChanged(CloseMedia);
        }

        if (current_playlist && current_item && item && current_playlist != playlist)
            spoil();

        current_playlist = playlist;
        current_item = item;

        if (item) {
            if (item -> error().toInt() != ItemErrors::warn_not_permitted) {
                if (fixSourceLimit)
                    item -> fixSourceLimit();

                if (continuePlaying) {
                    player -> updateMedia(
                        item -> startPosMillis(),
                        item -> durationMillis()
                    );
                    playerStatusChanged(InitMedia);
                    playerStatusChanged(PlaingMedia);
                } else {                   
                    player -> setMedia(
                        current_item -> toUrl(),
                        item -> startPosMillis(),
                        item -> durationMillis(),
                        startMili
                    );
                }

                if ((init_state_flag = state) != initiated)
                    player -> play(init_state_flag == paused);

                emit likeChanged(current_item -> has(flag_liked));
            } else playNext(false);
        }
        else player -> setMedia(QUrl());
    }

    void DataCore::playerStatusChanged(const PlayerStatus & status) {
        switch(status) {
            case InitMedia: {
                qDebug() << "INIT MEDIA";
                setState(flag_played | flag_proccessing);
            break;}

            case PlaingMedia: {
                attempts = 0;
                int add_state = 0;
                qDebug() << "PLAING MEDIA";
                if (current_item -> isRemote() && Settings::obj().isInitiateOnPlaying()) {
                    Library::obj().initItemData(current_item, false);
                    add_state = flag_proceeded;
                }
                setState(flag_listened | add_state);
            break;}

            case CloseMedia: {
                qDebug() << "CLOSE MEDIA";
                setState(flag_not_proccessing | flag_not_played);
            break;}

            case LoadingMedia: {
                qDebug() << "LOADING MEDIA";
//                setState(ItemState::proccessing);
            break;}

            case UnknownMediaStatus: {
                qDebug() << "UNKNOOW STATUS MEDIA";
                playNext(true);
            break;}

            case StalledMedia: {
                qDebug() << "STALLED MEDIA";

                if (current_item -> isRemote())
                    restoreOrNext();
                else proceedStalledState();
            break;}

            case EndPlaingMedia: {
                qDebug() << "END PLAYING MEDIA";
                playNext();
            break;}

            case LoadedMedia: {
                setState(flag_not_proccessing);
                setError(err_none);
            break;}

            case InvalidMedia: {
                qDebug() << "INVALID MEDIA";
                setError(warn_not_supported);
                playNext(true);
            break;}

            case NoMedia: {
                qDebug() << "NO MEDIA";

                if (current_item) {
                    if (current_item -> isRemote()) {
                        restoreOrNext();
                    } else {
                        setError(err_not_existed);
                        playNext(true);
                    }
                }
                else emit newPlaylistNeed();
            break;}
            default: { qDebug() << "WTF MEDIA"; }
        }
    }

    void DataCore::proceedInfoAsync(const QModelIndex & ind) {
        IItem * node = static_cast<IItem *>(ind.internalPointer());
        if (node -> isContainer()) return;

        if (!node -> has(flag_proceeded)) {
            node -> set(flag_proceeded);
            Library::obj().restoreItemStateAsync(ind, node -> isRemote());
        }
    }
}
