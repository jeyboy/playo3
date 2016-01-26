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
        void spoil() {
            if (!current_playlist) {
                qDebug() << "PLAYLIST IS UNDEFINED";
                return;
            }

            if (!current_playlist -> spoil(playedIndex()))
                qDebug() << "STATE IS NOT CHANGED";
        }
        void setState(int state) {
            if (!current_playlist) {
                qDebug() << "PLAYLIST IS UNDEFINED";
                return;
            }

            qDebug() << "STATE" << state;
            if (!current_playlist -> setState(playedIndex(), state))
                qDebug() << "STATE IS NOT CHANGED";
        }

        void playNext(bool onFail = false) {
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

        void restoreOrNext() { // need to prevent form loop
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

        void proceedStalledState() {
            setState(ItemState::not_exist);
            playNext(true);
        }
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

        void proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili = 0, PlayerInitState state = played, int durationMili = 0) {
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

        void proceedStoping() { currPlayer() -> stop(); }
        void proceedPausing() { currPlayer() -> pause(); }
        void proceedPauseToggling() { currPlayer() -> playPause(); }

    signals:
        void newPlaylistNeed();
        void likeChanged(bool);

    public slots:
//        inline void itemNotExist(QModelIndex node) { setData(node, ItemState::not_exist, ISTATE); }
//        inline void itemNotSupported(QModelIndex node) {
//            setData(node, ItemState::not_supported, ISTATE);
//            QDesktopServices::openUrl(node.data(IURL).toUrl());
//        }
//        inline void itemError(QModelIndex node) { setData(node, ItemState::not_supported | ItemState::not_exist, ISTATE); }

        void changeLikeStatus(bool is_liked) { setState((is_liked ? 1 : -1) * ItemState::liked); }

        void playerStatusChanged(const PlayerStatus & status) {
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

        void proceedInfoAsync(const QModelIndex & ind) {
            IItem * node = static_cast<IItem *>(ind.internalPointer());
            if (node -> isContainer()) return;

            if (!node -> is(ItemState::proceeded)) {
                node -> set(ItemState::proceeded);
                Library::obj().restoreItemStateAsync(ind, node -> isRemote());
            }
        }
    };
}

#endif // DATA_CORE_H
