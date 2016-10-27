#include "item_toolbar_features.h"

using namespace Presentation;

ItemToolbarFeatures::ItemToolbarFeatures(QToolBar * toolbar, const QString & postfix) : toolbar(toolbar) {
    QString prefix = LSTR(":/item_tools/item_");

    more_items_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("more") % postfix),
        LSTR("Load more"), this, SLOT(loadMoreItem())
    );

    toolbar -> addSeparator();

    item_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("album") % postfix),
        LSTR("Albums for ") % sentence(),
        this, SLOT(openAlbumforItem())
    );

    item_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("playlist") % postfix),
        LSTR("Playlists for ") % sentence(),
        this, SLOT(openPlaylistforItem())
    );


    item_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("audio") % postfix),
        LSTR("Audio recommendations for ") % sentence(),
        this, SLOT(openAudioforItem())
    );

    item_audio_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("audio_album") % postfix),
        LSTR("Audio albums for ") % sentence(),
        this, SLOT(openAudioAlbumforItem())
    );

    item_audio_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("audio_playlist") % postfix),
        LSTR("Audio playlists for ") % sentence(),
        this, SLOT(openAudioPlaylistforItem())
    );

    item_video_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("video") % postfix),
        LSTR("Video recommendations for ") % sentence(),
        this, SLOT(openVideoforItem())
    );

    item_video_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("video_album") % postfix),
        LSTR("Video albums for ") % sentence(),
        this, SLOT(openVideoAlbumforItem())
    );

    item_video_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("video_playlis") % postfix),
        LSTR("Video playlists for ") % sentence(),
        this, SLOT(openVideoPlaylistforItem())
    );



    item_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist") % postfix),
        LSTR("Recommendations for artists from ") % sentence(),
        this, SLOT(openRecsforItemArtist())
    );

    item_artist_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_audio") % postfix),
        LSTR("Audio for artists from ") % sentence(),
        this, SLOT(openAudioforArtist())
    );

    item_artist_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_album") % postfix),
        LSTR("Albums for artists from ") % sentence(),
        this, SLOT(openAudioAlbumsForArtist())
    );

    item_artist_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_playlist") % postfix),
        LSTR("Playlists for artists from ") % sentence(),
        this, SLOT(openPlaylistsForArtist())
    );

    item_artist_video_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_video") % postfix),
        LSTR("Video for artists from ") % sentence(),
        this, SLOT(openVideoforArtist())
    );

    item_artist_video_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_video_album") % postfix),
        LSTR("Video albums for artists from ") % sentence(),
        this, SLOT(openVideoAlbumforArtist())
    );

    item_artist_video_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("artist_video_playlist") % postfix),
        LSTR("Video playlists for artists from ") % sentence(),
        this, SLOT(openVideoPlaylistforArtist())
    );

    item_owner_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner") % postfix),
        LSTR("Recommendations for owner of") % sentence(),
        this, SLOT(openRecsforUser())
    );
    item_owner_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_artist") % postfix),
        LSTR("Artists for owner of") % sentence(),
        this, SLOT(openArtistsforUser())
    );
    item_owner_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_audio") % postfix),
        LSTR("Audio for owner of") % sentence(),
        this, SLOT(openAudioforUser())
    );
    item_owner_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_album") % postfix),
        LSTR("Audio albums for owner of") % sentence(),
        this, SLOT(openAudioAlbumsforUser())
    );
    item_owner_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_playlist") % postfix),
        LSTR("Audio playlists for owner of") % sentence(),
        this, SLOT(openAudioPlaylistsforUser())
    );

    item_owner_video_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_video") % postfix),
        LSTR("Video for owner of") % sentence(),
        this, SLOT(openVideoforUser())
    );
    item_owner_video_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_video_album") % postfix),
        LSTR("Video albums for owner of") % sentence(),
        this, SLOT(openVideoAlbumsforUser())
    );
    item_owner_video_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("owner_video_playlist") % postfix),
        LSTR("Video playlists for owner of") % sentence(),
        this, SLOT(openVideoPlaylistsforUser())
    );

    item_tag_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag") % postfix),
        LSTR("Recommendations for tags of ") % sentence(),
        this, SLOT(openRecForTag())
    );
    item_tag_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_artist") % postfix),
        LSTR("Artists for tags of ") % sentence(),
        this, SLOT(openArtistForTag())
    );
    item_tag_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_audio") % postfix),
        LSTR("Audio by tags of ") % sentence(),
        this, SLOT(openAudioForTag())
    );
    item_tag_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_album") % postfix),
        LSTR("Audio albums by tags of ") % sentence(),
        this, SLOT(openAudioAlbumForTag())
    );
    item_tag_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_playlist") % postfix),
        LSTR("Audio playlists by tags of ") % sentence(),
        this, SLOT(openAudioPlaylistForTag())
    );

    item_tag_video_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_video") % postfix),
        LSTR("Video by tags of ") % sentence(),
        this, SLOT(openVideoForTag())
    );
    item_tag_video_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_video_album") % postfix),
        LSTR("Video albums by tags of ") % sentence(),
        this, SLOT(openVideoAlbumForTag())
    );
    item_tag_video_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("tag_video_playlist") % postfix),
        LSTR("Video playlists by tags of ") % sentence(),
        this, SLOT(openVideoPlaylistForTag())
    );


    item_label_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label") % postfix),
        LSTR("Recommendations by labels from ") % sentence(),
        this, SLOT(openRecommendationsForLabel())
    );
    item_label_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_artist") % postfix),
        LSTR("Artists by labels from ") % sentence(),
        this, SLOT(openArtistsForLabel())
    );
    item_label_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_audio") % postfix),
        LSTR("Audio by labels from ") % sentence(),
        this, SLOT(openAudioForLabel())
    );
    item_label_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_album") % postfix),
        LSTR("Audio albums by labels from ") % sentence(),
        this, SLOT(openAudioAlbumForLabel())
    );
    item_label_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_playlist") % postfix),
        LSTR("Audio playlists by labels from ") % sentence(),
        this, SLOT(openAudioPlaylistForLabel())
    );


    item_label_video_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_video") % postfix),
        LSTR("Video by labels from ") % sentence(),
        this, SLOT(openVideoForLabel())
    );
    item_label_video_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_video_album") % postfix),
        LSTR("Video albums by labels from ") % sentence(),
        this, SLOT(openVideoAlbumForLabel())
    );
    item_label_video_playlist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("label_video_playlist") % postfix),
        LSTR("Video playlists by labels from ") % sentence(),
        this, SLOT(openVideoPlaylistForLabel())
    );
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

            bool has_tags_recs = item -> isAudio() ? source -> hasSimillarAudioByTag() : source -> hasSimillarVideoByTag();
            item_tag_btn -> setVisible(has_tags_recs);

            bool has_label_song_recs = source -> hasAudioFromSameLabel();
            item_label_audio_btn -> setVisible(has_label_song_recs);

            bool has_label_album_recs = source -> hasAlbumsFromSameLabel();
            item_label_album_btn -> setVisible(has_label_album_recs);

            bool has_label_artist_recs = source -> hasArtistsFromSameLabel();
            item_label_artist_btn -> setVisible(has_label_artist_recs);

            show |= has_item_recs || has_owner_recs || has_label_artist_recs ||
                    has_artist_recs || has_tags_recs || has_label_song_recs ||
                    has_artist_albums || has_artist_songs || has_label_album_recs;
        }
    }

    toolbar -> setHidden(!show);
}

void ItemToolbarFeatures::loadMoreItem() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            view -> runItemCmd(it);
    }
}

void ItemToolbarFeatures::openAlbumforItem();
void ItemToolbarFeatures::openPlaylistforItem();

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

void ItemToolbarFeatures::openAudioAlbumforItem();
void ItemToolbarFeatures::openAudioPlaylistforItem();

void ItemToolbarFeatures::openVideoforItem();
void ItemToolbarFeatures::openVideoAlbumforItem();
void ItemToolbarFeatures::openVideoPlaylistforItem();

void ItemToolbarFeatures::openRecsforItemArtist();
void ItemToolbarFeatures::openSimillarAudioforArtist() {
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
void ItemToolbarFeatures::openPlaylistsForArtist();
void ItemToolbarFeatures::openVideoforArtist();
void ItemToolbarFeatures::openVideoAlbumforArtist();
void ItemToolbarFeatures::openVideoPlaylistforArtist();

void ItemToolbarFeatures::openRecsforUser();
void ItemToolbarFeatures::openArtistsforUser();
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
void ItemToolbarFeatures::openAudioAlbumsforUser();
void ItemToolbarFeatures::openAudioPlaylistsforUser();

void ItemToolbarFeatures::openVideoforUser();
void ItemToolbarFeatures::openVideoAlbumsforUser();
void ItemToolbarFeatures::openVideoPlaylistsforUser();

void ItemToolbarFeatures::openRecForTag();
void ItemToolbarFeatures::openArtistForTag();
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
void ItemToolbarFeatures::openAudioAlbumForTag();
void ItemToolbarFeatures::openAudioPlaylistForTag();

void ItemToolbarFeatures::openVideoForTag();
void ItemToolbarFeatures::openVideoAlbumForTag();
void ItemToolbarFeatures::openVideoPlaylistForTag();

void ItemToolbarFeatures::openRecommendationsForLabel();
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

void ItemToolbarFeatures::openAudioPlaylistForLabel();

void ItemToolbarFeatures::openVideoForLabel();
void ItemToolbarFeatures::openVideoAlbumForLabel();
void ItemToolbarFeatures::openVideoPlaylistForLabel();
