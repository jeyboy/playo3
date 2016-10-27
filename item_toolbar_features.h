#ifndef ITEM_TOOLBAR_FEATURES_H
#define ITEM_TOOLBAR_FEATURES_H

#include <qtoolbar.h>
#include "dockbars.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/core_parts/item.h"

namespace Presentation {
    class ItemToolbarFeatures : public QObject {
        Q_OBJECT
    protected:
        QToolBar * toolbar;

        QAction * more_items_btn;

        QAction * item_label_btn;
        QAction * item_label_album_btn;
        QAction * item_label_artist_btn;
        QAction * item_label_playlist_btn;
        QAction * item_label_audio_btn;
        QAction * item_label_video_btn;
        QAction * item_label_video_album_btn;
        QAction * item_label_video_playlist_btn;

        QAction * item_owner_btn;
        QAction * item_owner_album_btn;
        QAction * item_owner_artist_btn;
        QAction * item_owner_playlist_btn;
        QAction * item_owner_audio_btn;
        QAction * item_owner_video_btn;
        QAction * item_owner_video_album_btn;
        QAction * item_owner_video_playlist_btn;

        QAction * item_artist_btn;
        QAction * item_artist_album_btn;
        QAction * item_artist_playlist_btn;
        QAction * item_artist_audio_btn;
        QAction * item_artist_video_btn;
        QAction * item_artist_video_album_btn;
        QAction * item_artist_video_playlist_btn;

        QAction * item_audio_btn;

        QAction * item_tag_btn;
        QAction * item_tag_album_btn;
        QAction * item_tag_artist_btn;
        QAction * item_tag_playlist_btn;
        QAction * item_tag_audio_btn;
        QAction * item_tag_video_btn;
        QAction * item_tag_video_album_btn;
        QAction * item_tag_video_playlist_btn;

        virtual Core::IItem * targetItem() = 0;
        virtual Views::IView * targetView() = 0;
    public:
        ItemToolbarFeatures(QToolBar * toolbar, const QString & postfix = QString());

    public slots:
        void hide() { toolbar ->  hide(); }

        void updateToolbar();

        void loadMoreItem();
        void openRecsforItem();
        void openRecsforItemUser();
        void openRecsforItemArtist();
        void openTracksforArtist();
        void openAlbumsForArtist();
        void openRecsForItemTags();
        void openTracksForLabel();
        void openAlbumsForLabel();
        void openArtistsForLabel();
    };

    class PlayedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() { return DataFactory::obj().playedItem(); }
        Views::IView * targetView() { return Dockbars::obj().playedView(); }
    public:
        PlayedItemToolbarFeatures(QToolBar * toolbar) : ItemToolbarFeatures(toolbar, LSTR("_pl")) {}
    };

    class SelectedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() { return targetView() -> selectedItem(); }
        Views::IView * targetView() { return Dockbars::obj().activeView(); }
    };
}

#endif // ITEM_TOOLBAR_FEATURES_H
