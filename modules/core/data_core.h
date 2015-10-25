#ifndef DATA_CORE_H
#define DATA_CORE_H

#include <qdebug.h>
#include <qobject.h>
#include <qhash.h>

#include "core_parts_index.h"
#include "modules/core/media/library.h"
#include "modules/core/interfaces/singleton.h"

#include "modules/views/view_interface.h"

#include "settings.h"

namespace Core {
    class DataFactory : public QObject, public QHash<QString, IItem *>, public Singleton<DataFactory> {
        Q_OBJECT

        Views::IView * current_playlist;
        Models::IModel * current_model;
        IItem * current_item;
    public:
        inline DataFactory() : QObject(), current_playlist(0), current_model(0), current_item(0) {}
        ~DataFactory() {}

        inline QModelIndex playedIndex() { return current_model ? current_model -> index(current_item) : QModelIndex(); }
        inline Views::IView * currentPlaylist() const { return current_playlist; }
        inline IItem * playedItem() { return current_item; }
        inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

        void resetPlaying() {
            current_playlist = 0;
            current_item = 0;
        }

        void proceedPlaying(Views::IView * playlist, IItem * item, uint startMili = 0, bool paused = false, int durationMili = 0) {
            current_playlist = playlist;
            current_item = item;
            current_model = item ? (Models::IModel *)playlist -> model() : 0;

//            bool refresh = current_item && new_item == current_item;

            IPlayer * player = Settings::obj().currPlayer();
            player -> stop();
            player -> setMedia(current_item -> toUrl());
            player -> play(startMili, paused, durationMili);
        }

        void proceedStoping() { Settings::obj().currPlayer() -> stop(); }
        void proceedPausing() { Settings::obj().currPlayer() -> pause(); }
        void proceedPauseToggling() { Settings::obj().currPlayer() -> playPause(); }
    public slots:
        void registerSync(QAbstractItemModel * mdl, QMutex * mutex) {
            Library::obj().registerListSync(mdl, mutex);
        }

        void unregisterSync(QAbstractItemModel * mdl) {
            Library::obj().unregisterListSync(mdl);
        }

        void discardSync(QAbstractItemModel * mdl) {
            Library::obj().unregisterListSync(mdl);
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
