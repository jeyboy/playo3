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

    void DataFactory::playNext(bool onFail) {
        setState(-ItemState::proccessing); // extra call for item clearing states!
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
        setState(ItemState::not_exist);
        playNext(true);
    }

    void DataFactory::proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili, PlayerInitState state, int durationMili) {
        IPlayer * player = currPlayer();
        player -> closeMedia();

        if (current_playlist && current_item && item && current_playlist != playlist)
            spoil();

        current_playlist = playlist;
        current_item = item;

        if (item) {
            player -> setMedia(
                current_item -> toUrl(),
                startMili,
                durationMili
            );

            if ((init_state_flag = state) != initiated)
                player -> play(init_state_flag == paused);

            emit likeChanged(current_item -> is(ItemState::liked));
        }
        else player -> setMedia(QUrl());
    }

    void DataFactory::playerStatusChanged(const PlayerStatus & status) {
        switch(status) {
            case InitMedia: {
                qDebug() << "INIT MEDIA";
                setState(ItemState::played);
            break;}

            case PlaingMedia: {
                attempts = 0;
                int add_state = 0;
                qDebug() << "PLAING MEDIA";
                if (current_item -> isRemote() && Settings::obj().isInitiateOnPlaying()) {
                    Library::obj().initItemData(current_item, false);
                    add_state = ItemFields::proceeded;
                }
                setState(ItemState::listened | add_state);
            break;}

            case CloseMedia: {
                qDebug() << "CLOSE MEDIA";
                setState(-(ItemState::proccessing | ItemState::played));
            break;}

            case LoadingMedia: {
                qDebug() << "LOADING MEDIA";
                setState(ItemState::proccessing);
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
                setState(ItemState::not_proccessing | ItemState::exist | ItemState::supported);
            break;}

            case InvalidMedia: {
                qDebug() << "INVALID MEDIA";
                setState(ItemState::not_supported);
                playNext(true);
            break;}

            case NoMedia: {
                qDebug() << "NO MEDIA";

                if (current_item) {
                    if (current_item -> isRemote()) {
                        restoreOrNext();
                    } else {
                        setState(ItemState::not_exist);
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

        if (!node -> is(ItemState::proceeded)) {
            node -> set(ItemState::proceeded);
            Library::obj().restoreItemStateAsync(ind, node -> isRemote());
        }
    }
}
