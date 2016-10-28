#ifndef ITEM_TOOLBAR_FEATURES_H
#define ITEM_TOOLBAR_FEATURES_H

#include <qtoolbar.h>
#include "dockbars.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/core_parts/item.h"

#define PERM_ERROR(name) "Permissions required" << name << "required on some additional permissions or this service not respondable to this action";

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
        QAction * item_artist_sim_audio_btn;
        QAction * item_artist_video_btn;
        QAction * item_artist_video_album_btn;
        QAction * item_artist_video_playlist_btn;

        QAction * item_audio_btn;
        QAction * item_audio_album_btn;
        QAction * item_audio_playlist_btn;
        QAction * item_video_btn;
        QAction * item_video_album_btn;
        QAction * item_video_playlist_btn;

        QAction * item_album_btn;
        QAction * item_playlist_btn;

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
        virtual QString sentence() = 0;

        void prepareToolbar(const QString & postfix = QString());
    public:
        ItemToolbarFeatures(QToolBar * toolbar);

        QToolBar * targetToolbar() { return toolbar; }
    public slots:
        void hide() { toolbar ->  hide(); }
        void updateToolbar();


        void loadMoreItem();

//        void openAlbumforItem();
//        void openPlaylistforItem();

        void openAudioforItem();
        void openAudioAlbumforItem();
        void openAudioPlaylistforItem();

        void openVideoforItem();
        void openVideoAlbumforItem();
        void openVideoPlaylistforItem();

        void openRecsforItemArtist();
        void openSimillarAudiobyArtist();
        void openAudioforArtist();

        void openAudioAlbumsForArtist();
        void openPlaylistsForArtist();
        void openVideoforArtist();
        void openVideoAlbumforArtist();
        void openVideoPlaylistforArtist();

        void openRecsforUser();
        void openArtistsforUser();
        void openAudioforUser();
        void openAudioAlbumsforUser();
        void openAudioPlaylistsforUser();

        void openVideoforUser();
        void openVideoAlbumsforUser();
        void openVideoPlaylistsforUser();

        void openRecForTag();
        void openArtistForTag();
        void openAudioForTag();
        void openAudioAlbumForTag();
        void openAudioPlaylistForTag();

        void openVideoForTag();
        void openVideoAlbumForTag();
        void openVideoPlaylistForTag();

        void openRecommendationsForLabel();
        void openArtistsForLabel();

        void openAudioForLabel();
        void openAudioAlbumForLabel();
        void openAudioPlaylistForLabel();

        void openVideoForLabel();
        void openVideoAlbumForLabel();
        void openVideoPlaylistForLabel();
    };

    class PlayedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() { return DataFactory::obj().playedItem(); }
        Views::IView * targetView() { return Dockbars::obj().playedView(); }
        QString sentence() { return LSTR("played item"); }
    public:
        PlayedItemToolbarFeatures(QToolBar * toolbar) : ItemToolbarFeatures(toolbar) { prepareToolbar(LSTR("_pl")); }
    };

    class SelectedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() { return targetView() -> selectedItem(); }
        Views::IView * targetView() { return Dockbars::obj().activeView(); }
        QString sentence() { return LSTR("selected item"); }
    public:
        SelectedItemToolbarFeatures(QToolBar * toolbar) : ItemToolbarFeatures(toolbar) { prepareToolbar(); }
    };
}

#endif // ITEM_TOOLBAR_FEATURES_H
