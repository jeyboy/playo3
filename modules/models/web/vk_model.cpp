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
    QJsonArray albums, audios, groups, friends;

    if (hash.isArray()) {
        // reserved
    } else {
        QJsonObject hash_obj = hash.toObject();
        albums = hash_obj.value(Vk::tkn_albums).toArray();
        audios = hash_obj.value(Vk::tkn_audio_list).toObject().value(Vk::tkn_items).toArray();
        groups = hash_obj.value(Vk::tkn_groups).toArray();
        friends = hash_obj.value(Vk::tkn_friends).toArray();
    }

    int itemsAmount = 0, albums_count = albums.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count());
    {
        if (albums_count > 0) {
            Playlist * folder;
            QJsonObject album;

            int pos = 0;
            for(QJsonArray::Iterator album_obj = albums.begin(); album_obj != albums.end(); album_obj++, pos++) {
                album = (*album_obj).toObject();

                if (album.contains(Vk::tkn_items)) {
                    QJsonArray albumItems = album.value(Vk::tkn_items).toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createPlaylist(
                            dt_playlist_vk,
                            album.value(Vk::tkn_id).toString(),
                            album.value(Vk::tkn_title).toString(),
                            pos
                        );

                        int folderItemsAmount = proceedVkList(albumItems, folder);
                        folder -> updateItemsCountInBranch(folderItemsAmount);
                        itemsAmount += folderItemsAmount;
                    }
                }
                else {
                    rootItem -> createLoadablePlaylist(
                        {
                            {JSON_TYPE_ITEM_TYPE, dt_playlist_vk},
                            {tkn_grab_refresh, album.value(Vk::tkn_id).toString()}
                        },
                        itm.value(Vk::tkn_title).toString(),
                        itm.value(Vk::tkn_id).toString()
                    );
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (audios.count() > 0)
            itemsAmount += proceedVkList(audios, rootItem);
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
