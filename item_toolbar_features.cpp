#include "item_toolbar_features.h"

using namespace Presentation;

ItemToolbarFeatures::ItemToolbarFeatures(QToolBar * toolbar, const QString & postfix) : toolbar(toolbar) {
    pl_more_items_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_more_pl")), LSTR("Load more"), this, SLOT(loadMoreItem()));
    toolbar -> addSeparator();

    pl_item_song_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_audio_pl")), LSTR("Recommendations for played item"), this, SLOT(openRecsforItem()));
    pl_item_singer_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_artist_pl")), LSTR("Recommendations for played item artists"), this, SLOT(openRecsforItemArtist()));
    pl_item_singer_song_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_artist_audio_pl")), LSTR("Tracks from played item artists"), this, SLOT(openTracksforArtist()));
    pl_item_singer_album_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_artist_album_pl")), LSTR("Albums from played item artists"), this, SLOT(openAlbumsForArtist()));
    pl_item_owner_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_owner_pl")), LSTR("Recommendations for played item owner"), this, SLOT(openRecsforItemUser()));
    pl_item_tags_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_tag_pl")), LSTR("Recommendations for played item tags"), this, SLOT(openRecsForItemTags()));
    pl_item_labels_song_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_label_audio_pl")), LSTR("Tracks from played item labels"), this, SLOT(openTracksForLabel()));
    pl_item_labels_album_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_label_album_pl")), LSTR("Albums from played item labels"), this, SLOT(openAlbumsForLabel()));
    pl_item_labels_artist_btn = toolbar -> addAction(QIcon(LSTR(":/item_tools/item_label_artist_pl")), LSTR("Artists from played item labels"), this, SLOT(openArtistsForLabel()));
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
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            view -> runItemCmd(it);
    }
}

void ItemToolbarFeatures::openRecsforItem() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForItem(it);
    }
}
void ItemToolbarFeatures::openRecsforItemUser() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForUser(it);
    }
}
void ItemToolbarFeatures::openRecsforItemArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForArtist(it);
    }
}
void ItemToolbarFeatures::openTracksforArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameArtist(it);
    }
}
void ItemToolbarFeatures::openAlbumsForArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameArtist(it);
    }
}
void ItemToolbarFeatures::openRecsForItemTags() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForTags(it);
    }
}
void ItemToolbarFeatures::openTracksForLabel() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameLabel(it);
    }
}
void ItemToolbarFeatures::openAlbumsForLabel() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameLabel(it);
    }
}
void ItemToolbarFeatures::openArtistsForLabel() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedArtistsFromSameLabel(it);
    }
}
