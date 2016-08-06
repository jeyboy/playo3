#include "soundcloud_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void SoundcloudModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    switch(sttngs.rec_type) {
        case rec_song: {
            Soundcloud::Queries::obj().trackRelationsAsync(
                sttngs.uid,
                new Func(this, SLOT(proceedJson(QJsonValue &)))
            );
        break;}

        case rec_set: {
            Soundcloud::Queries::obj().openSetAsync(
                sttngs.uid,
                new Func(this, SLOT(proceedJson(QJsonValue &)))
            );
        break;}

        default:
            Soundcloud::Queries::obj().objectInfoAsync(
                sttngs.uid,
                new Func(this, SLOT(proceedJson(QJsonValue &)))
            );
    }
}

void SoundcloudModel::proceedJson(QJsonValue & hash) {
    QJsonArray items, liked_items, sets;

    if (hash.isArray()) {
        items = hash.toArray();
    } else {
        QJsonObject hash_obj = hash.toObject();
        items = hash_obj.value(block_items).toArray();
        liked_items = hash_obj.value(block_likes).toArray();
        sets = hash_obj.value(block_sets).toArray();
    }

    int total_amount = 0, sets_amount = sets.size(), items_amount = items.size(), likes_amount = liked_items.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + sets_amount + items_amount + likes_amount);
    {
        if (likes_amount > 0) {
            Playlist * playlist = rootItem -> createPlaylist(dt_playlist_sc, QStringLiteral("User Likes"));
            total_amount += proceedScList(liked_items, playlist);
        }

    /////////////////////////////////////////////////////////////////////

        if (sets_amount > 0) {
            for(QJsonArray::Iterator it = sets.begin(); it != sets.end(); it++) {
                QJsonObject set = (*it).toObject();

                QJsonArray set_items = set.value(Soundcloud::tkn_tracks).toArray();
                if (set_items.size() > 0) {
                    Playlist * playlist = rootItem -> createPlaylist(
                        dt_playlist_sc,
                        set.value(Soundcloud::tkn_id).toString(),
                        set.value(Soundcloud::tkn_title).toString()
                    );

                    int playlistSize = proceedScList(set_items, playlist);
                    playlist -> updateItemsCountInBranch(playlistSize);
                    total_amount += playlistSize;
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (items_amount > 0)
            total_amount += proceedScList(items, rootItem);
    }

    rootItem -> updateItemsCountInBranch(total_amount);
    endInsertRows();
    /////////////////////////////////////////////////////////////////////

    emit moveOutProcess();
}
