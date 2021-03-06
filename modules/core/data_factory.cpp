#include "data_factory.h"

namespace Core {
    DataFactory::DataFactory() : QObject(), current_playlist(0), current_item(0), attempts(0), restore_attempts(0) {
        PlayerFactory::obj().registerCallback(answer_from_player, this, SIGNAL(statusChanged(QString, PlayerStatus)), SLOT(playerStatusChanged(QString, PlayerStatus)));
    }

    void DataFactory::spoil() {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> spoil(playedIndex()))
            qCritical() << "STATE IS NOT CHANGED";
    }
    void DataFactory::setState(const QString & name, const int & state) {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        qDebug() << "STATE" << state << ':' << name;
        if (!current_playlist -> setState(playedIndex(), state))
            qCritical() << "STATE IS NOT CHANGED";
    }

    void DataFactory::setError(const ItemErrors & error) {
        if (!current_playlist) {
            qCritical() << "PLAYLIST IS UNDEFINED";
            return;
        }

        if (!current_playlist -> setError(playedIndex(), error))
            qCritical() << "ERROR IS NOT ATTACHED";
    }

    void DataFactory::playNext(const bool & onFail) {
        setState(currPlayer() -> title(), IItem::flag_not_proccessing); // extra call for item clearing states!
        if (!current_playlist) {
            qCritical() << "NEXT: PLAYLIST IS UNDEFINED";
            return;
        }
        if (current_playlist -> isAutoPlayNext()) {
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

        if (++restore_attempts > MAX_RESTORE_ATTEMPTS)
            proceedStalledState();

        if (!current_playlist -> restoreItem(current_item)) {
            qCritical() << "RESTORE: FAILED";
            proceedStalledState();
        } else {
            qDebug() << "RESTORE: SUCCESS";
            IPlayer * player = currPlayer();
            player -> updateMedia(current_item -> toUrl(), current_item -> title().toString());
            player -> play(init_state_flag == IPlayer::init_paused);
        }
    }

    void DataFactory::proceedStalledState() {
        setError(current_item -> isRemote() ? ItemErrors::warn_not_accessable : ItemErrors::warn_not_supported);
        playNext(true);
    }

    void DataFactory::proceedPlaying(IPlaylistable * playlist, IItem * item, const uint & startMili, const IPlayer::InitializationState & state) {
//        if (item == current_item && playlist == currentPlaylist()) return;

        if (item)
            qDebug() << "********** PROCEED PLAYING:" << item -> title().toString();

        IPlayer * player = currPlayer();

        QUrl curr_url = player -> mediaUrl();
        bool continue_playing = item && !curr_url.isEmpty() && item -> toUrl() == curr_url;

        if (!continue_playing)
            player -> closeMedia();
        else {
            player -> pause();
            playerStatusChanged(player -> title(), CloseMedia);
        }

        if (current_playlist && current_item && item && current_playlist != playlist)
            spoil();

        QString prev_item_title = current_item ? current_item -> title().toString() : QString();

        current_playlist = playlist;
        current_item = item;
        restore_attempts = 0;

        if (item) {
            if (item -> error().toInt() != ItemErrors::warn_not_permitted) {
                emit playedItemChanged(prev_item_title, item -> title().toString());

                if (continue_playing) {
                    player -> updateMedia(
                        item -> startPosMillis(),
                        item -> durationMillis()
                    );
                    playerStatusChanged(player -> title(), InitMedia);
                    playerStatusChanged(player -> title(), PlaingMedia);
                } else {
                    player -> setMedia(
                        current_item -> toUrl(),
                        current_item -> title().toString(),
                        item -> startPosMillis(),
                        item -> durationMillis(),
                        startMili
                    );
                }

                if ((init_state_flag = state) != IPlayer::init_actiated)
                    player -> play(init_state_flag == IPlayer::init_paused);

                emit likeChanged(current_item -> is(IItem::flag_liked));
            } else playNext(false);
        }
        else player -> setMedia(QUrl(), QString());

        emit itemFeaturesChanged();
    }

    void DataFactory::playerStatusChanged(const QString & name, const PlayerStatus & status) {
        switch(status) {
            case InitMedia: {
                qDebug() << "INIT MEDIA" << name;
                setState(name, IItem::flag_played | IItem::flag_proccessing);
            break;}

            case PlaingMedia: {
                setError(ItemErrors::err_none);

                attempts = 0;
                int add_state = 0;
                qDebug() << "PLAING MEDIA" << name;
                if (current_item -> isRemote() && Settings::obj().isInitiateOnPlaying()) {
                    Library::obj().initItemData(current_item, false);
                    add_state = IItem::flag_proceeded;
                }

                setState(name, IItem::flag_not_proccessing | IItem::flag_listened | add_state); // IItem::flag_not_proccessing is duplicated for cue items
            break;}

            case CloseMedia: {
                qDebug() << "CLOSE MEDIA" << name;
                setState(name, IItem::flag_not_proccessing | IItem::flag_not_played);
            break;}

            case LoadingMedia: {
                qDebug() << "LOADING MEDIA" << name;
//                setState(name, ItemState::proccessing);
            break;}

            case UnknownMediaStatus: {
                qCritical() << "UNKNOOW STATUS MEDIA" << name;
                setError(ItemErrors::err_unknow);
                playNext(true);
            break;}

            case StalledMedia: {
                qCritical() << "STALLED MEDIA" << name;

                if (current_item -> isRemote())
                    restoreOrNext();
                else proceedStalledState();
            break;}

            case EndPlaingMedia: {
                qDebug() << "END PLAYING MEDIA" << name;
                playNext();
            break;}

            case LoadedMedia: {
                qDebug() << "LOADED MEDIA" << name;
                setState(name, IItem::flag_not_proccessing);
            break;}

            case InvalidMedia: {
                qCritical() << "INVALID MEDIA" << name;

                if (current_item -> isRemote())
                    restoreOrNext();
                else {
                    setError(ItemErrors::warn_not_supported);
                    playNext(true);
                }
            break;}

            case NoMedia: {
                qCritical() << "NO MEDIA" << name;

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
            default: { qCritical() << "WTF MEDIA" << name; }
        }
    }
}
