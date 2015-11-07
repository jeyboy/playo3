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

#include "settings.h"

namespace Core {
    class DataFactory : public QObject, public QHash<QString, IItem *>, public Singleton<DataFactory> {
        Q_OBJECT

        IPlaylistable * current_playlist;
        IItem * current_item;
    protected:
        void setState(int state) {
            if (!current_playlist) {
                qDebug() << "PLAYLIST IS UNDEFINED";
                return;
            }

            if (!current_playlist -> setState(playedIndex(), state))
                qDebug() << "STATE IS NOT CHANGED";
        }

        void playNext() {
            setState(-ItemState::proccessing); // extra call for item clearing states!
            if (!current_playlist) {
                qDebug() << "NEXT: PLAYLIST IS UNDEFINED";
                return;
            }
            current_playlist -> execNextIndex();
        }
    public:
        inline DataFactory() : QObject(), current_playlist(0), current_item(0) {
            PlayerFactory::obj().registerCallback(out, this, SIGNAL(statusChanged(PlayerStatus)), SLOT(playerStatusChanged(PlayerStatus)));
        }
        ~DataFactory() {}

        inline IPlayer * currPlayer() { return PlayerFactory::obj().currPlayer(); }

        inline QModelIndex playedIndex() { return current_playlist ? current_playlist -> index(current_item) : QModelIndex(); }
        inline IPlaylistable * currentPlaylist() const { return current_playlist; }
        inline IItem * playedItem() { return current_item; }
        inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

        void resetPlaying() {
            qDebug() << "RESET PLAYING";
            proceedPlaying(0, 0);
        }

        void proceedPlaying(IPlaylistable * playlist, IItem * item, uint startMili = 0, bool paused = false, int durationMili = 0) {
//            bool refresh = current_item && new_item == current_item;

            IPlayer * player = currPlayer();
            player -> closeMedia();

            current_playlist = playlist;
            current_item = item;

            if (item) {
                player -> setMedia(current_item -> toUrl());
                player -> play(startMili, paused, durationMili);
            }
            else player -> setMedia(QUrl());
        }

        void proceedStoping() { currPlayer() -> stop(); }
        void proceedPausing() { currPlayer() -> pause(); }
        void proceedPauseToggling() { currPlayer() -> playPause(); }


    public slots:
//        inline void startProccessing() { setItemState(ItemState::proccessing); }
//        inline void endProccessing() { setItemState(-(ItemState::proccessing | ItemState::not_exist | ItemState::not_supported)); }

//        inline void itemNotExist(QModelIndex node) { setData(node, ItemState::not_exist, ISTATE); }
//        inline void itemNotSupported(QModelIndex node) {
//            setData(node, ItemState::not_supported, ISTATE);
//            QDesktopServices::openUrl(node.data(IURL).toUrl());
//        }
//        inline void itemError(QModelIndex node) { setData(node, ItemState::not_supported | ItemState::not_exist, ISTATE); }


        void playerStatusChanged(const PlayerStatus & status) {
            switch(status) {
                case InitMedia: {
                    qDebug() << "INIT MEDIA";
                    setState(ItemState::played);
                break;}

                case PlaingMedia: {
                    qDebug() << "PLAING MEDIA";
                    setState(ItemState::listened);
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
                    playNext();
                break;}

                case StalledMedia: {
                    qDebug() << "STALLED MEDIA";

                    if (current_item -> isRemote()) {
                        //REFRESH NEEDED
                    } else {
                        //MARK ELEM AS INVALID
                        playNext();
                    }
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
                    //MARK ELEM AS NOT SUPPORTED
                    playNext();
                break;}

                case NoRemoteMedia: {

                }

                case NoMedia: {
                    qDebug() << "NO MEDIA";

                    if (current_item -> isRemote()) {
                        //REFRESH NEEDED
                    } else {
                        //MARK ELEM AS NOT EXISTED
                        playNext();
                    }
                break;}
                default: {  }
            }
        }

        void registerSync(QAbstractItemModel * mdl, QMutex * mutex) {
            Library::obj().registerListSync(mdl, mutex);
        }

        void unregisterSync(QAbstractItemModel * mdl) {
            Library::obj().unregisterListSync(mdl);
        }

        void discardSync(QAbstractItemModel * mdl) {
            Library::obj().declineAllItemsRestoration(mdl);
        }


        void changeCadrSize(QAbstractItemModel * mdl, int newCadrSize) {
            Library::obj().setWaitListLimit(mdl, newCadrSize);
        }

        void proceedInfo(const QModelIndex & ind) {
            Library::obj().directItemStateRestoration(ind);
        }

        void proceedInfoAsync(const QModelIndex & ind) {
            IItem * node = static_cast<IItem *>(ind.internalPointer());

            bool is_interactive = Settings::obj().isInteractiveProc();
            bool is_proceeded = node -> is(ItemState::proceeded);

            if (!is_proceeded) {
                if (!is_interactive)
                    node -> set(ItemState::proceeded);
                if (!node -> isContainer())
                    Library::obj().restoreItemState(ind);
            }
        }
    };
}

#endif // DATA_CORE_H
