#include "vk_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    if (sttngs.rec_type == Core::rec_none) {
        Vk::Queries::obj().userInfoAsync(
            sttngs.uid,
            new Func(this, SLOT(proceedJson(QJsonValue &)))
        );
    } else {
        Vk::Queries::obj().trackRecommendationsAsync(
            sttngs.uid,
            sttngs.rec_type == Core::rec_user,
            true,
            new Func(this, SLOT(proceedJson(QJsonValue &)))
        );
    }
}

void VkModel::proceedJson(QJsonValue & hash) {
    QJsonArray audio_albums, audios, groups, friends, videos, video_albums;

    if (hash.isArray()) {
        // reserved
    } else {
        QJsonObject hash_obj = hash.toObject();
        audio_albums = hash_obj.value(block_sets_audio).toArray();

        audios =  hash_obj.value(block_items_audio).toArray();

        groups = hash_obj.value(block_groups).toArray();
        friends = hash_obj.value(block_friends).toArray();

        videos = hash_obj.value(block_items_video).toArray();
        video_albums = hash_obj.value(block_sets_video).toArray();
    }

    int itemsAmount = 0, albums_count = audio_albums.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count() + videos.count() + video_albums.count());
    {
        if (albums_count > 0) {
            Playlist * folder;
            QJsonObject album;

            int pos = 0;
            for(QJsonArray::Iterator album_obj = audio_albums.begin(); album_obj != audio_albums.end(); album_obj++, pos++) {
                album = (*album_obj).toObject();
                QString album_id = album.value(Vk::tkn_id).toString();

                if (album.contains(Vk::tkn_items)) {
                    QJsonArray albumItems = album.value(Vk::tkn_items).toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createPlaylist(
                            dt_playlist_vk,
                            album_id,
                            album.value(Vk::tkn_title).toString(),
                            pos
                        );

                        int folderItemsAmount = proceedVkList(albumItems, folder, dmt_audio);
                        folder -> updateItemsCountInBranch(folderItemsAmount);
                        itemsAmount += folderItemsAmount;
                    }
                }
                else {
                    rootItem -> createLoadablePlaylist(
                        {
                            {tkn_media_type, dmt_audio},
                            {JSON_TYPE_ITEM_TYPE, dt_site_vk},
                            {tkn_grab_refresh, album_id}
                        },
                        album.value(Vk::tkn_title).toString(),
                        album_id
                    );
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (video_albums.count() > 0) {
            Playlist * folder;
            QJsonObject album;

            int pos = 0;
            for(QJsonArray::Iterator album_obj = video_albums.begin(); album_obj != video_albums.end(); album_obj++, pos++) {
                album = (*album_obj).toObject();
                QString album_id = QString::number(album.value(Vk::tkn_id).toInt());

                if (album.contains(Vk::tkn_items)) {
                    QJsonArray albumItems = album.value(Vk::tkn_items).toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createPlaylist(
                            dt_playlist_vk,
                            album_id,
                            album.value(Vk::tkn_title).toString(),
                            pos
                        );

                        int folderItemsAmount = proceedVkList(albumItems, folder, dmt_video);
                        folder -> updateItemsCountInBranch(folderItemsAmount);
                        itemsAmount += folderItemsAmount;
                    }
                }
                else {
                    folder = rootItem -> createLoadablePlaylist(
                        {
                            {tkn_media_type, dmt_video},
                            {JSON_TYPE_ITEM_TYPE, dt_site_vk},
                            {tkn_grab_refresh, album_id}
                        },
                        album.value(Vk::tkn_title).toString(),
                        album_id
                    );
                }

                folder -> setArtPath(album.value(Vk::tkn_video_art).toString());
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (videos.count() > 0)
            itemsAmount += proceedVkList(videos, rootItem, dmt_video);

    /////////////////////////////////////////////////////////////////////

        if (audios.count() > 0)
            itemsAmount += proceedVkList(audios, rootItem, dmt_audio);
    }

    rootItem -> updateItemsCountInBranch(itemsAmount);
    endInsertRows();
    /////////////////////////////////////////////////////////////////////
    {
        QJsonObject group;

        for(QJsonArray::Iterator it = groups.begin(); it != groups.end(); it++) {
            group = (*it).toObject();

            Vk::Queries::obj().addGroup(
                Linkable(
                    QString::number(group.value(Vk::tkn_id).toInt()),
                    group.value(Vk::tkn_title).toString(),
                    group.value(Vk::tkn_screen_name).toString(),
                    group.value(Vk::tkn_photo).toString()
                )
            );
        }
    }
/////////////////////////////////////////////////////////////////////
    {
        QJsonObject frend;

        for(QJsonArray::Iterator it = friends.begin(); it != friends.end(); it++) {
            frend = (*it).toObject();

            Vk::Queries::obj().addFriend(
                Linkable(
                    QString::number(frend.value(Vk::tkn_id).toInt()),
                    frend.value(Vk::tkn_title).toString(),
                    frend.value(Vk::tkn_screen_name).toString(),
                    frend.value(Vk::tkn_photo).toString()
                )
            );
        }
    }

    emit moveOutProcess();
}
