#include "data_factory.h"

namespace Core {
    void DataFactory::spoil() {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> spoil(playedIndex()))
            qCritical() << "STATE IS NOT CHANGED";
    }
    void DataFactory::setState(int state) {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        qDebug() << "STATE" << state;
        if (!current_playlist -> setState(playedIndex(), state))
            qCritical() << "STATE IS NOT CHANGED";
    }

    void DataFactory::setError(ItemErrors error) {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> setError(playedIndex(), error))
            qCritical() << "ERROR IS NOT ATTACHED";
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

    void DataFactory::restoreOrNext() { // need to prevent from loop
        if (!current_playlist) {
            qCritical() << "RESTORE: PLAYLIST IS UNDEFINED";
            return;
        }

        if (++attempts > MAX_ATTEMPTS) {
            attempts = 0;
            proceedStalledState();
        }

        if (!current_playlist -> restoreItem(current_item)) {
            qCritical() << "RESTORE: FAILED";
            proceedStalledState();
        } else {
            qDebug() << "RESTORE: SUCCESS";
            IPlayer * player = currPlayer();
            player -> updateMedia(current_item -> toUrl());
            player -> play(init_state_flag == paused);
        }
    }

    void DataFactory::proceedStalledState() {
        setError(current_item -> isRemote() ? ItemErrors::warn_not_accessable : ItemErrors::err_not_existed);
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

        QString prev_item_title = current_item ? current_item -> title().toString() : QString();

        current_playlist = playlist;
        current_item = item;
        attempts = 0;

        if (item) {
            if (item -> error().toInt() != ItemErrors::warn_not_permitted) {
                emit playedItemChanged(prev_item_title, item -> title().toString());

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
                int add_state = 0;
                qDebug() << "PLAING MEDIA";
                if (current_item -> isRemote() && Settings::obj().isInitiateOnPlaying()) {
                    Library::obj().initItemData(current_item, false);
                    add_state = IItem::flag_proceeded;
                }

                setState(IItem::flag_not_proccessing | IItem::flag_listened | add_state); // IItem::flag_not_proccessing is duplicated for cue items
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
                qCritical() << "UNKNOOW STATUS MEDIA";
                playNext(true);
            break;}

            case StalledMedia: {
                qCritical() << "STALLED MEDIA";

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
                qCritical() << "INVALID MEDIA";

                if (current_item -> isRemote())
                    restoreOrNext();
                else {
                    setError(ItemErrors::warn_not_supported);
                    playNext(true);
                }
//                playNext(true);
            break;}

            case NoMedia: {
                qCritical() << "NO MEDIA";

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
            default: { qCritical() << "WTF MEDIA"; }
        }
    }
}
