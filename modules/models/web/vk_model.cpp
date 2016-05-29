#include "vk_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkModel::refresh(bool retryPlaing) {
    emit moveInProcess();
    QApplication::processEvents();

    if (sttngs.rec_type == Core::rec_none) {
        Vk::Api::obj().userInfo(
            sttngs.uid,
            sttngs.uid == Vk::Api::obj().userID() ? Vk::Api::info_all : Vk::Api::info_music,
            new Func(this, retryPlaing ? SLOT(proceedAudioListAndRetry(QJsonObject &)) : SLOT(proceedAudioList(QJsonObject &)))
        );
    } else {
        Vk::Api::obj().audioRecomendations(
            sttngs.uid,
            sttngs.rec_type == Core::rec_user,
            true,
            new Func(this, SLOT(proceedAudioList(QJsonObject &)))
        );
    }
}

//void VkModel::refreshWall() {
//    emit moveInProcess();
//    QApplication::processEvents();
//    VkApi::instance() -> wallAudio(tab_uid, Func(this, "proceedWallList"));
//}

void VkModel::proceedWallList(QJsonArray & posts) {
    if (posts.count() > 0) {
        QJsonArray audios;
        QJsonObject post;
        QString title;
        Playlist * folder, * rootFolder = rootItem -> createPlaylist(dt_playlist, QStringLiteral("!!!WALL!!!"));
        int index = rootItem -> childRow(rootFolder);

        beginInsertRows(QModelIndex(), index, index);

        QJsonArray::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = (*it).toObject();
            audios = post.value(Vk::tkn_audios).toArray();

            title = post.value(Vk::tkn_title).toString();
            title = QDateTime::fromTime_t(post.value(Vk::tkn_date).toInt()).toString() % (title.isEmpty() ? QString() : Vk::tkn_time_sep) % title;

            folder = rootFolder -> createPlaylist(dt_playlist, title);
            proceedVkList(audios, folder);
//            if (folder -> childCount() == 0)
//                removeFolderPrebuild(folder);
        }
        endInsertRows();
    }

//    emit expandNeeded(index(rootFolder));
    emit moveOutProcess();
}

void VkModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray albums = hash.value(Vk::tkn_albums).toArray();
    QJsonArray audios = hash.value(Vk::tkn_audio_list).toObject().value(Vk::tkn_items).toArray();
    int itemsAmount = 0, albums_count = albums.size();

//    beginResetModel();
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
//    endResetModel();
    /////////////////////////////////////////////////////////////////////
    {
        QJsonObject group;
        QJsonArray groups = hash.value(Vk::tkn_groups).toArray();

        for(QJsonArray::Iterator it = groups.begin(); it != groups.end(); it++) {
            group = (*it).toObject();

            Vk::Api::obj().addGroup(
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

            Vk::Api::obj().addFriend(
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

void VkModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
//    Player::obj().playIndex(Player::obj().playedIndex());
}
