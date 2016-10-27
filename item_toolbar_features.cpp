#include "item_toolbar_features.h"

using namespace Presentation;

ItemToolbarFeatures::ItemToolbarFeatures(QToolBar * toolbar, const QString & postfix) : toolbar(toolbar) {
    QString prefix = LSTR(":/item_tools/item");

    more_items_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_more") % postfix), LSTR("Load more"), this, SLOT(loadMoreItem())
    );
    toolbar -> addSeparator();

    item_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_audio") % postfix), LSTR("Recommendations for played item"), this, SLOT(openRecsforItem())
    );

    item_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_artist") % postfix), LSTR("Recommendations for played item artists"), this, SLOT(openRecsforItemArtist())
    );

    item_artist_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_artist_audio") % postfix), LSTR("Tracks from played item artists"), this, SLOT(openTracksforArtist())
    );

    item_artist_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_artist_album") % postfix), LSTR("Albums from played item artists"), this, SLOT(openAlbumsForArtist())
    );

    item_owner_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_owner") % postfix), LSTR("Recommendations for played item owner"), this, SLOT(openRecsforItemUser())
    );

    item_tag_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_tag") % postfix), LSTR("Recommendations for played item tags"), this, SLOT(openRecsForItemTags())
    );

    item_label_audio_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_label_audio") % postfix), LSTR("Tracks from played item labels"), this, SLOT(openTracksForLabel())
    );

    item_label_album_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_label_album") % postfix), LSTR("Albums from played item labels"), this, SLOT(openAlbumsForLabel())
    );

    item_label_artist_btn = toolbar -> addAction(
        QIcon(prefix % LSTR("_label_artist") % postfix), LSTR("Artists from played item labels"), this, SLOT(openArtistsForLabel())
    );
}

void ItemToolbarFeatures::updateToolbar() {
    bool show = false;

    Core::IItem * item = targetItem();

    if (item) {
        show = item -> parent() -> hasMoreItems()/* || it -> hasMoreItems()*/;
        pl_more_items_btn -> setVisible(show );

        Core::ISource * source = Web::Apis::source(item -> dataType());

        if (source) {
            bool has_item_recs = source -> hasSimillarAudioByAudio();
            pl_item_song_btn -> setVisible(has_item_recs);

            bool has_owner_recs = source -> hasSimillarAudioByUser();
            pl_item_owner_btn -> setVisible(has_owner_recs);

            bool has_artist_recs = source -> hasSimillarAudioByArtist();
            pl_item_singer_btn -> setVisible(has_artist_recs);

            bool has_artist_songs = source -> hasAudioFromSameArtist();
            pl_item_singer_song_btn -> setVisible(has_artist_songs);

            bool has_artist_albums = source -> hasAlbumsFromSameArtist();
            pl_item_singer_album_btn -> setVisible(has_artist_albums);

            bool has_tags_recs = item -> isAudio() ? source -> hasSimillarAudioByTag() : source -> hasSimillarVideoByTag();
            pl_item_tags_btn -> setVisible(has_tags_recs);

            bool has_label_song_recs = source -> hasAudioFromSameLabel();
            pl_item_labels_song_btn -> setVisible(has_label_song_recs);

            bool has_label_album_recs = source -> hasAlbumsFromSameLabel();
            pl_item_labels_album_btn -> setVisible(has_label_album_recs);

            bool has_label_artist_recs = source -> hasArtistsFromSameLabel();
            pl_item_labels_artist_btn -> setVisible(has_label_artist_recs);

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

void ItemToolbarFeatures::openRecsforItem() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForItem(it);
    }
}
void ItemToolbarFeatures::openRecsforItemUser() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForUser(it);
    }
}
void ItemToolbarFeatures::openRecsforItemArtist() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForArtist(it);
    }
}
void ItemToolbarFeatures::openTracksforArtist() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameArtist(it);
    }
}
void ItemToolbarFeatures::openAlbumsForArtist() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameArtist(it);
    }
}
void ItemToolbarFeatures::openRecsForItemTags() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForTags(it);
    }
}
void ItemToolbarFeatures::openTracksForLabel() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameLabel(it);
    }
}
void ItemToolbarFeatures::openAlbumsForLabel() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameLabel(it);
    }
}
void ItemToolbarFeatures::openArtistsForLabel() {
    IView * view = targetView();
    if (view) {
        IItem * it = targetItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedArtistsFromSameLabel(it);
    }
}
