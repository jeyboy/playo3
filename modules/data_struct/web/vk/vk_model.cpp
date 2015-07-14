#include "vk_model.h"
#include "media/player.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////
void VkModel::refresh(bool retryPlaing) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    VkApi::instance() -> userInfo(tab_uid, tab_uid == VkApi::instance() -> userID(), Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
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
        FolderItem * folder, * rootFolder = rootItem -> createFolder("!!!WALL!!!");
        int index = rootItem -> childRow(rootFolder);

        beginInsertRows(QModelIndex(), index, index);

        QJsonArray::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = (*it).toObject();
            audios = post.value("audios").toArray();

            title = post.value("title").toString();
            title = QDateTime::fromTime_t(post.value("date").toInt()).toString() + (title.isEmpty() ? "" : " : ") + title;

            folder = rootFolder -> createFolder(title);
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
    QJsonArray albums = hash.value("albums").toArray();
    QJsonArray audios = hash.value("audio_list").toObject().value("items").toArray();
    int itemsAmount = 0, albums_count = VkApi::extractCount(albums);

//    beginResetModel();
    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count());
    {
        if (albums_count > 0) {
            VkFolder * folder;
            QJsonObject album;

            for(QJsonArray::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                QJsonArray part_arr = (*album_part).toArray();
                QJsonArray::Iterator it = part_arr.begin();
                for(int pos = 0; it != part_arr.end(); it++, pos++) {
                    album = (*it).toObject();

                    QJsonArray albumItems = album.value("items").toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createFolder<VkFolder>(
                            album.value("folder_id").toString(),
                            album.value("title").toString(),
                            pos
                        );

                        int folderItemsAmount = proceedVkList(albumItems, folder);
                        folder -> updateItemsCountInBranch(folderItemsAmount);
                        itemsAmount += folderItemsAmount;
                    }
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
        QJsonArray groups = hash.value("groups").toArray();
        QJsonArray::Iterator it = groups.begin();

        for(; it != groups.end(); it++) {
            group = (*it).toObject();

            VkApi::instance() -> addGroup(
                QString::number(group.value("id").toInt()),
                group.value("title").toString()
            );
        }
    }
/////////////////////////////////////////////////////////////////////
    {
        QJsonObject frend;
        QJsonArray friends = hash.value("friends").toArray();
        QJsonArray::Iterator it = friends.begin();

        for(; it != friends.end(); it++) {
            frend = (*it).toObject();

            VkApi::instance() -> addFriend(
                QString::number(frend.value("id").toInt()),
                frend.value("title").toString()
            );
        }
    }

    emit moveOutProcess();
}

void VkModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
