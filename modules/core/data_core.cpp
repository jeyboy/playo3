#include "data_core.h"

namespace Core {
    void DataFactory::spoil() {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> spoil(playedIndex()))
            qDebug() << "STATE IS NOT CHANGED";
    }
    void DataFactory::setState(int state) {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        qDebug() << "STATE" << state;
        if (!current_playlist -> setState(playedIndex(), state))
            qDebug() << "STATE IS NOT CHANGED";
    }

    void DataFactory::setError(ItemErrors error) {
        if (!current_playlist) {
            qDebug() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> setError(playedIndex(), error))
            qDebug() << "ERROR IS NOT ATTACHED";
    }

    void DataFactory::playNext(bool onFail) {
        setState(IItem::flag_not_proccessing); // extra call for item clearing states!
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

    void DataFactory::restoreOrNext() { // need to prevent form loop
        if (!current_playlist) {
            qDebug() << "RESTORE: PLAYLIST IS UNDEFINED";
            return;
        }
        if (!current_playlist -> restoreItem(current_item)) {
            qDebug() << "RESTORE: FAILED";
            proceedStalledState();
        } else {
            qDebug() << "RESTORE: SUCCESS";
            IPlayer * player = currPlayer();
            player -> updateMedia(current_item -> toUrl());
            player -> play(init_state_flag == paused);
        }
    }

    void DataFactory::proceedStalledState() {
        setError(ItemErrors::err_not_existed);
        playNext(true);
    }

    void DataFactory::proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili, PlayerInitState state) {
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

                emit likeChanged(current_item -> is(IItem::flag_liked));
            } else playNext(false);
        }
        else player -> setMedia(QUrl());
    }

    void DataFactory::playerStatusChanged(const PlayerStatus & status) {
        switch(status) {
            case InitMedia: {
                qDebug() << "INIT MEDIA";
                setState(IItem::flag_played | IItem::flag_proccessing);
            break;}

            case PlaingMedia: {
                attempts = 0;
                int add_state = 0;
                qDebug() << "PLAING MEDIA";
                if (current_item -> isRemote() && Settings::obj().isInitiateOnPlaying()) {
                    Library::obj().initItemData(current_item, false);
                    add_state = IItem::flag_proceeded;
                }
                setState(IItem::flag_listened | add_state);
            break;}

            case CloseMedia: {
                qDebug() << "CLOSE MEDIA";
                setState(IItem::flag_not_proccessing | IItem::flag_not_played);
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
                setState(IItem::flag_not_proccessing);
                setError(ItemErrors::err_none);
            break;}

            case InvalidMedia: {
                qDebug() << "INVALID MEDIA";
                setError(ItemErrors::warn_not_supported);
                playNext(true);
            break;}

            case NoMedia: {
                qDebug() << "NO MEDIA";

                if (current_item) {
                    if (current_item -> isRemote()) {
                        restoreOrNext();
                    } else {
                        setError(ItemErrors::err_not_existed);
                        playNext(true);
                    }
                }
                else emit newPlaylistNeed();
            break;}
            default: { qDebug() << "WTF MEDIA"; }
        }
    }

    void DataFactory::proceedInfoAsync(const QModelIndex & ind) {
        IItem * node = static_cast<IItem *>(ind.internalPointer());
        if (node -> isContainer()) return;

        if (!node -> is(IItem::flag_proceeded)) {
            node -> set(IItem::flag_proceeded);
            Library::obj().restoreItemStateAsync(ind, node -> isRemote());
        }
    }
}
