#include "soundcloud_model.h"
#include "media/player.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////

void SoundcloudModel::refresh(bool retryPlaing) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();
    Soundcloud::Api::instance() -> objectInfo(tab_uid, Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
}

void SoundcloudModel::proceedAudioList(Json::Obj & hash) {
    QJsonArray albums = hash.value(Soundcloud::playlist_key).toArray();
    QJsonArray audios = hash.value(Soundcloud::audio_list_key).toArray();
    int itemsAmount = 0, albums_count = Soundcloud::Api::extractCount(albums), audios_count = Soundcloud::Api::extractCount(audios);

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudFolder * folder;
            Json::Obj album;

            for(Json::Arr::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                Json::Arr part_arr = (*album_part).toArray();
                for(Json::Arr::Iterator it = part_arr.begin(); it != part_arr.end(); it++) {
                    album = (*it).toObject();

                    Json::Arr albumItems = album.arr(Soundcloud::tracks_key);
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createFolder<SoundcloudFolder>(
                            album.str(Soundcloud::id_key),
                            album.str(Soundcloud::title_key)
                        );

                        int folderItemsAmount = proceedScList(albumItems, folder);
                        folder -> updateItemsCountInBranch(folderItemsAmount);
                        itemsAmount += folderItemsAmount;
                    }
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (audios_count > 0)
            itemsAmount += proceedScList(audios, rootItem);
    }
    rootItem -> updateItemsCountInBranch(itemsAmount);
    endInsertRows();
    /////////////////////////////////////////////////////////////////////

    {
        Json::Obj group;
        Json::Arr groups = hash.arr(Soundcloud::groups_key);

        for(Json::Arr::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
            Json::Arr group_part = (*group_it).toArray();
            for(Json::Arr::Iterator it = group_part.begin(); it != group_part.end(); it++) {
                group = (*it).toObject();

                Soundcloud::Api::instance() -> addGroup(
                    group.numStr(Soundcloud::id_key),
                    group.str(Soundcloud::name_key)
                );
            }
        }
    }
    /////////////////////////////////////////////////////////////////////
    Json::Obj frend;
    QString name;

    {        
        Json::Arr friends = hash.arr(Soundcloud::followings_key);

        for(Json::Arr::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            Json::Arr friend_part = (*friend_it).toArray();
            for(Json::Arr::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.str(Soundcloud::full_name_key);
                if (name.isEmpty())
                    name = frend.str(Soundcloud::username_key);

                Soundcloud::Api::instance() -> addFriend(
                    frend.numStr(Soundcloud::id_key),
                    name
                );
            }
        }
    }

    {
        Json::Arr friends = hash.arr(Soundcloud::followers_key);

        for(Json::Arr::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            Json::Arr friend_part = (*friend_it).toArray();
            for(Json::Arr::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.str(Soundcloud::full_name_key);
                if (name.isEmpty())
                    name = frend.str(Soundcloud::username_key);

                Soundcloud::Api::instance() -> addFriend(
                    frend.numStr(Soundcloud::id_key),
                    name
                );
            }
        }
    }

    emit moveOutProcess();
}

void SoundcloudModel::proceedAudioListAndRetry(Json::Obj & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
