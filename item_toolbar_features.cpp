#include "item_toolbar_features.h"

using namespace Presentation;

ItemToolbarFeatures::ItemToolbarFeatures(QToolBar * toolbar) : toolbar(toolbar) {}

void ItemToolbarFeatures::prepareToolbar(const QString & postfix) {
    ADD_FEATURE_ITEM(postfix, LSTR("more"), LSTR("Load more"), ItemToolbarFeatures::loadMoreItem, ISource::stub);

//    features.insert(
//        &openAudioforItem,
//        ItemToolbarFeature(toolbar, ICO_PREFIX % LSTR("more") % postfix, LSTR("Load more"), this, SLOT(ItemToolbarFeatures::openAudioforItem()), &ISource::hasSimillarAudioByAudio)
//    );

//    more_items_btn = toolbar -> addAction(
//        QIcon(ICO_PREFIX % LSTR("more") % postfix),
//        LSTR("Load more"), this, SLOT(loadMoreItem())
//    );

    toolbar -> addSeparator();

//    features.insert(
//        &ItemToolbarFeatures::openAudioforItem,
//        ItemToolbarFeature(ICO_PREFIX % LSTR("audio") % postfix,                     LSTR("Audio recommendations for ") % sentence(),
//            SLOT(openAudioforItem()), &ISource::hasSimillarAudioByAudio)
//    );

//    features.insert(
//        &ItemToolbarFeatures::openAudioAlbumforItem,
//        ItemToolbarFeature(ICO_PREFIX % LSTR("audio_album") % postfix,               LSTR("Audio albums for ") % sentence(),
//            SLOT(openAudioAlbumforItem()), 0)
//    );

//    features.insert(
//        &ItemToolbarFeatures::openAudioPlaylistforItem,
//        ItemToolbarFeature(ICO_PREFIX % LSTR("audio_playlist") % postfix,            LSTR("Audio playlists for ") % sentence(),
//            SLOT(openAudioPlaylistforItem()), 0)
//    );




//        << ItemToolbarFeature(ICO_PREFIX % LSTR("audio_playlist") % postfix,            LSTR("Audio playlists for ") % sentence(),
//                              SLOT(openAudioPlaylistforItem()), 0)



//        << ItemToolbarFeature(ICO_PREFIX % LSTR("video") % postfix,                     LSTR("Video recommendations for ") % sentence(),
//                              SLOT(openVideoforItem()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("video_album") % postfix,               LSTR("Video albums for ") % sentence(),
//                              SLOT(openVideoAlbumforItem()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("video_playlist") % postfix,            LSTR("Video playlists for ") % sentence(),
//                              SLOT(openVideoPlaylistforItem()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist") % postfix,                    LSTR("Recommendations for artists from ") % sentence(),
//                              SLOT(openRecsforItemArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_audio") % postfix,              LSTR("Audio for artists from ") % sentence(),
//                              SLOT(openAudioforArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_sim_audio") % postfix,          LSTR("Simillar audio by artists from ") % sentence(),
//                              SLOT(openSimillarAudiobyArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_album") % postfix,              LSTR("Albums for artists from ") % sentence(),
//                              SLOT(openAudioAlbumsForArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_playlist") % postfix,           LSTR("Playlists for artists from ") % sentence(),
//                              SLOT(openPlaylistsForArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_video") % postfix,              LSTR("Video for artists from ") % sentence(),
//                              SLOT(openVideoforArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_video_album") % postfix,        LSTR("Video albums for artists from ") % sentence(),
//                              SLOT(openVideoAlbumforArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("artist_video_playlist") % postfix,     LSTR("Video playlists for artists from ") % sentence(),
//                              SLOT(openVideoPlaylistforArtist()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner") % postfix,                     LSTR("Recommendations for owner of ") % sentence(),
//                              SLOT(openRecsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_artist") % postfix,              LSTR("Artists for owner of ") % sentence(),
//                              SLOT(openArtistsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_audio") % postfix,               LSTR("Audio for owner of ") % sentence(),
//                              SLOT(openAudioforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_album") % postfix,               LSTR("Audio albums for owner of ") % sentence(),
//                              SLOT(openAudioAlbumsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_playlist") % postfix,            LSTR("Audio playlists for owner of ") % sentence(),
//                              SLOT(openAudioPlaylistsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_video") % postfix,               LSTR("Video for owner of ") % sentence(),
//                              SLOT(openVideoforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_video_album") % postfix,         LSTR("Video albums for owner of ") % sentence(),
//                              SLOT(openVideoAlbumsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("owner_video_playlist") % postfix,      LSTR("Video playlists for owner of ") % sentence(),
//                              SLOT(openVideoPlaylistsforUser()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag") % postfix,                       LSTR("Recommendations for tags of ") % sentence(),
//                              SLOT(openRecForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_artist") % postfix,                LSTR("Artists for tags of ") % sentence(),
//                              SLOT(openArtistForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_audio") % postfix,                 LSTR("Audio by tags of ") % sentence(),
//                              SLOT(openAudioForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_album") % postfix,                 LSTR("Audio albums by tags of ") % sentence(),
//                              SLOT(openAudioAlbumForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_playlist") % postfix,              LSTR("Audio playlists by tags of ") % sentence(),
//                              SLOT(openAudioPlaylistForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_video") % postfix,                 LSTR("Video by tags of ") % sentence(),
//                              SLOT(openVideoForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_video_album") % postfix,           LSTR("Video albums by tags of ") % sentence(),
//                              SLOT(openVideoAlbumForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("tag_video_playlist") % postfix,        LSTR("Video playlists by tags of ") % sentence(),
//                              SLOT(openVideoPlaylistForTag()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label") % postfix,                     LSTR("Recommendations by labels from ") % sentence(),
//                              SLOT(openRecommendationsForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_artist") % postfix,              LSTR("Artists by labels from ") % sentence(),
//                              SLOT(openArtistsForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_audio") % postfix,               LSTR("Audio by labels from ") % sentence(),
//                              SLOT(openAudioForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_album") % postfix,               LSTR("Audio albums by labels from ") % sentence(),
//                              SLOT(openAudioAlbumForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_playlist") % postfix,            LSTR("Audio playlists by labels from ") % sentence(),
//                              SLOT(openAudioPlaylistForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_video") % postfix,               LSTR("Video by labels from ") % sentence(),
//                              SLOT(openVideoForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_video_album") % postfix,         LSTR("Video albums by labels from ") % sentence(),
//                              SLOT(openVideoAlbumForLabel()), 0)
//        << ItemToolbarFeature(ICO_PREFIX % LSTR("label_video_playlist") % postfix,      LSTR("Video playlists by labels from ") % sentence(),
//                              SLOT(openVideoPlaylistForLabel()), 0);
}

void ItemToolbarFeatures::updateToolbar() {
    bool show = false;

    Core::IItem * item = targetItem();

    if (item) {
        show = item -> parent() -> hasMoreItems()/* || it -> hasMoreItems()*/;
        more_items_btn -> setVisible(show );

        Core::ISource * source = Web::Apis::source(item -> dataType());

        if (source) {
            bool has_item_recs = source -> hasSimillarAudioByAudio();
            item_audio_btn -> setVisible(has_item_recs);

            bool has_owner_recs = source -> hasSimillarAudioByUser();
            item_owner_btn -> setVisible(has_owner_recs);

            bool has_artist_recs = source -> hasSimillarAudioByArtist();
            item_artist_btn -> setVisible(has_artist_recs);

            bool has_artist_songs = source -> hasAudioFromSameArtist();
            item_artist_audio_btn -> setVisible(has_artist_songs);

            bool has_artist_albums = source -> hasAlbumsFromSameArtist();
            item_artist_album_btn -> setVisible(has_artist_albums);

            bool has_tags_audio_recs = item -> source -> hasSimillarAudioByTag();
            item_tag_audio_btn -> setVisible(has_tags_audio_recs);

            bool has_tags_video_recs = item -> source -> hasSimillarVideoByTag();
            item_tag_video_btn -> setVisible(has_tags_video_recs);

            bool has_label_song_recs = source -> hasAudioFromSameLabel();
            item_label_audio_btn -> setVisible(has_label_song_recs);

            bool has_label_album_recs = source -> hasAlbumsFromSameLabel();
            item_label_album_btn -> setVisible(has_label_album_recs);

            bool has_label_artist_recs = source -> hasArtistsFromSameLabel();
            item_label_artist_btn -> setVisible(has_label_artist_recs);

            show |= has_item_recs || has_owner_recs || has_label_artist_recs || has_tags_video_recs ||
                    has_artist_recs || has_tags_audio_recs || has_label_song_recs ||
                    has_artist_albums || has_artist_songs || has_label_album_recs;
        }
    }

    toolbar -> setHidden(!show);
}

void ItemToolbarFeatures::appendToMenu(QMenu * menu) {
    Core::IItem * item = targetItem();

    if (item) {
        Core::ISource * source = Web::Apis::source(item -> dataType());

        if (source) {
            QString prefix = LSTR(":/item_tools/item_");


            bool has_item_recs = source -> hasSimillarAudioByAudio();
            item_audio_btn -> setVisible(has_item_recs);

            bool has_owner_recs = source -> hasSimillarAudioByUser();
            item_owner_btn -> setVisible(has_owner_recs);

            bool has_artist_recs = source -> hasSimillarAudioByArtist();
            item_artist_btn -> setVisible(has_artist_recs);

            bool has_artist_songs = source -> hasAudioFromSameArtist();
            item_artist_audio_btn -> setVisible(has_artist_songs);

            bool has_artist_albums = source -> hasAlbumsFromSameArtist();
            item_artist_album_btn -> setVisible(has_artist_albums);

            bool has_tags_audio_recs = item -> source -> hasSimillarAudioByTag();
            item_tag_audio_btn -> setVisible(has_tags_audio_recs);

            bool has_tags_video_recs = item -> source -> hasSimillarVideoByTag();
            item_tag_video_btn -> setVisible(has_tags_video_recs);

            bool has_label_song_recs = source -> hasAudioFromSameLabel();
            item_label_audio_btn -> setVisible(has_label_song_recs);

            bool has_label_album_recs = source -> hasAlbumsFromSameLabel();
            item_label_album_btn -> setVisible(has_label_album_recs);

            bool has_label_artist_recs = source -> hasArtistsFromSameLabel();
            item_label_artist_btn -> setVisible(has_label_artist_recs);
        }
    }

    //        src = Web::Apis::source((DataSubType)ind.data(ITYPE).toInt());

    //        if (src) {
    //            if (src -> hasSimillarAudioByUser())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Recommendations for item owner"), this, SLOT(openRecsForItemUser()));

    //            if (src -> hasSimillarAudioByAudio())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Recommendations by item"), this, SLOT(openRecsForItem()));

    //            if (src -> hasSimillarAudioByArtist())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Recommendations by item artist"), this, SLOT(openRecsForArtist()));

    //            if (src -> hasSimillarAudioByTag())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Recommendations by item tags"), this, SLOT(openRecsForTag()));

    //            if (src -> hasAudioFromSameArtist())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Tracks from same artist"), this, SLOT(openTracksFromSameArtist()));

    //            if (src -> hasAlbumsFromSameArtist())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Albums from same artist"), this, SLOT(openAlbumsFromSameArtist()));


    //            if (src -> hasAudioFromSameLabel())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Tracks from same label"), this, SLOT(openTracksFromSameLabel()));

    //            if (src -> hasAlbumsFromSameLabel())
    //                menu.addAction(QIcon(/*":/active_tab"*/), LSTR("Albums from same label"), this, SLOT(openAlbumsFromSameLabel()));

    //            menu.addSeparator();
    //        }
}

void ItemToolbarFeatures::loadMoreItem() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            view -> runItemCmd(it);
    }
}

void ItemToolbarFeatures::openAudioforItem() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasSimillarAudioByAudio()) {
            if (it -> id().isValid()) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, it -> id().toString(), rec_audio);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Rec for song ") % it -> title().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioforItem"));
    }
}

void ItemToolbarFeatures::openAudioAlbumforItem() {}
void ItemToolbarFeatures::openAudioPlaylistforItem() {}

void ItemToolbarFeatures::openVideoforItem() {}
void ItemToolbarFeatures::openVideoAlbumforItem() {}
void ItemToolbarFeatures::openVideoPlaylistforItem() {}

void ItemToolbarFeatures::openRecsforItemArtist() {}
void ItemToolbarFeatures::openSimillarAudiobyArtist() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasSimillarAudioByArtist()) {
            QVariantMap artists = it -> artistsList();

            for(QVariantMap::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, artist.key(), rec_artist);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Rec for artist ") % artist.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openSimillarAudioforArtist"));
    }
}
void ItemToolbarFeatures::openAudioforArtist() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasAudioFromSameArtist()) {
            QVariantMap artists = it -> artistsList();

            if (artists.isEmpty()) {
                QString var_artist = it -> title().toString().split('-').first();
                artists.insert(var_artist, var_artist);
            }

            for(QVariantMap::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, artist.key(), rec_artist_audio);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Tracks from artist ") % artist.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioforArtist"));
    }
}

void ItemToolbarFeatures::openAudioAlbumsForArtist() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasAlbumsFromSameArtist()) {
            QVariantMap artists = it -> artistsList();

            for(QVariantMap::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, artist.key(), rec_artist_album);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Albums from artist ") % artist.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioAlbumsForArtist"));
    }
}
void ItemToolbarFeatures::openPlaylistsForArtist() {}
void ItemToolbarFeatures::openVideoforArtist() {}
void ItemToolbarFeatures::openVideoAlbumforArtist() {}
void ItemToolbarFeatures::openVideoPlaylistforArtist() {}

void ItemToolbarFeatures::openRecsforUser() {}
void ItemToolbarFeatures::openArtistsforUser() {}
void ItemToolbarFeatures::openAudioforUser() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasSimillarAudioByUser()) {
            QVariant owner_id = it -> ownerId();
            if (owner_id.isValid()) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, owner_id.toString(), rec_user_audio);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Rec for user ") % owner_id.toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioforUser"));
    }
}
void ItemToolbarFeatures::openAudioAlbumsforUser() {}
void ItemToolbarFeatures::openAudioPlaylistsforUser() {}

void ItemToolbarFeatures::openVideoforUser() {}
void ItemToolbarFeatures::openVideoAlbumsforUser() {}
void ItemToolbarFeatures::openVideoPlaylistsforUser() {}

void ItemToolbarFeatures::openRecForTag() {}
void ItemToolbarFeatures::openArtistForTag() {}
void ItemToolbarFeatures::openAudioForTag() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        bool is_audio = it -> isAudio();

        if (src -> hasSimillarAudioByTag()) {
            if (it -> id().isValid()) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, it -> tagsList().join(','), is_audio ? rec_tag_audio : rec_tag_video);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Rec for ") % (is_audio ? LSTR("song ") : LSTR("video ")) % it -> title().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioForTag"));
    }
}
void ItemToolbarFeatures::openAudioAlbumForTag() {}
void ItemToolbarFeatures::openAudioPlaylistForTag() {}

void ItemToolbarFeatures::openVideoForTag() {}
void ItemToolbarFeatures::openVideoAlbumForTag() {}
void ItemToolbarFeatures::openVideoPlaylistForTag() {}

void ItemToolbarFeatures::openRecommendationsForLabel() {}
void ItemToolbarFeatures::openArtistsForLabel() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasArtistsFromSameLabel()) {
            QVariantMap labels = it -> labelsList();

            for(QVariantMap::Iterator label = labels.begin(); label != labels.end(); label++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, label.key(), rec_label_album);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Albums for label ") % label.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openArtistsForLabel"));
    }
}

void ItemToolbarFeatures::openAudioForLabel() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasAudioFromSameLabel()) {
            QVariantMap labels = it -> labelsList();

            for(QVariantMap::Iterator label = labels.begin(); label != labels.end(); label++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, label.key(), rec_label_audio);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Audio for label ") % label.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioForLabel"));
    }
}
void ItemToolbarFeatures::openAudioAlbumForLabel() {
    IItem * it = targetItem();
    if (it) {
        ISource * src = Web::Apis::source(it -> dataType());

        if (src -> hasAlbumsFromSameLabel()) {
            QVariantMap labels = it -> labelsList();

            for(QVariantMap::Iterator label = labels.begin(); label != labels.end(); label++) {
                Params bar_settings(it -> dataType(), mpf_auto_play_next, label.key(), rec_label_album);
                Presentation::Dockbars::obj().createDocBar(Presentation::BarCreationNames(LSTR("Albums for label ") % label.value().toString()), bar_settings, 0, true, true);
            }
        } else qCritical() << PERM_ERROR(LSTR("openAudioAlbumForLabel"));
    }
}

void ItemToolbarFeatures::openAudioPlaylistForLabel() {}

void ItemToolbarFeatures::openVideoForLabel() {}
void ItemToolbarFeatures::openVideoAlbumForLabel() {}
void ItemToolbarFeatures::openVideoPlaylistForLabel() {}
