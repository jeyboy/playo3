#include "vk_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    if (sttngs.rec_type == Core::rec_none) {
        Vk::Queries::obj().userInfoAsync(
            sttngs.uid,
            new Func(this, SLOT(proceedJson(QJsonObject &)))
        );
    } else {
        Vk::Queries::obj().trackRecommendationsAsync(
            sttngs.uid,
            sttngs.rec_type == Core::rec_user,
            true,
            new Func(this, SLOT(proceedJson(QJsonObject &)))
        );
    }
}

void VkModel::proceedJson(QJsonObject & hash) {
    QJsonArray albums = hash.value(Vk::tkn_albums).toArray();
    QJsonArray audios = hash.value(Vk::tkn_audio_list).toObject().value(Vk::tkn_items).toArray();
    int itemsAmount = 0, albums_count = albums.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count());
    {
        if (albums_count > 0) {
            Playlist * folder;
            QJsonObject album;

            int pos = 0;
            for(QJsonArray::Iterator album_obj = albums.begin(); album_obj != albums.end(); album_obj++, pos++) {
                album = (*album_obj).toObject();

                QJsonArray albumItems = album.value(Vk::tkn_items).toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createPlaylist(
                        dt_playlist_vk,
                        album.value(Vk::tkn_folder_id).toString(),
                        album.value(Vk::tkn_title).toString(),
                        pos
                    );

                    int folderItemsAmount = proceedVkList(albumItems, folder);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
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
        QJsonArray groups = hash.value(Vk::tkn_groups).toArray();

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
        QJsonArray friends = hash.value(Vk::tkn_friends).toArray();

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
