#include "vk_model.h"
#include "player/player_old.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkModel::refresh(bool retryPlaing) {
//    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

//    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    Vk::Api::instance() -> userInfo(tab_uid, tab_uid == Vk::Api::instance() -> userID(), Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
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
        Playlist * folder, * rootFolder = rootItem -> createPlaylist(QStringLiteral("!!!WALL!!!"));
        int index = rootItem -> childRow(rootFolder);

        beginInsertRows(QModelIndex(), index, index);

        QJsonArray::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = (*it).toObject();
            audios = post.value(QStringLiteral("audios")).toArray();

            title = post.value(QStringLiteral("title")).toString();
            title = QDateTime::fromTime_t(post.value(QStringLiteral("date")).toInt()).toString() % (title.isEmpty() ? QString() : QStringLiteral(" : ")) % title;

            folder = rootFolder -> createPlaylist(title);
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
    QJsonArray albums = hash.value(QStringLiteral("albums")).toArray();
    QJsonArray audios = hash.value(QStringLiteral("audio_list")).toObject().value(QStringLiteral("items")).toArray();
    int itemsAmount = 0, albums_count = Vk::Api::extractCount(albums);

//    beginResetModel();
    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count());
    {
        if (albums_count > 0) {
            VkPlaylist * folder;
            QJsonObject album;

            for(QJsonArray::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                QJsonArray part_arr = (*album_part).toArray();
                QJsonArray::Iterator it = part_arr.begin();
                for(int pos = 0; it != part_arr.end(); it++, pos++) {
                    album = (*it).toObject();

                    QJsonArray albumItems = album.value(QStringLiteral("items")).toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createPlaylist<VkPlaylist>(
                            album.value(QStringLiteral("folder_id")).toString(),
                            album.value(QStringLiteral("title")).toString(),
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
        QJsonArray groups = hash.value(QStringLiteral("groups")).toArray();
        QJsonArray::Iterator it = groups.begin();

        for(; it != groups.end(); it++) {
            group = (*it).toObject();

            Vk::Api::instance() -> addGroup(
                QString::number(group.value(QStringLiteral("id")).toInt()),
                group.value(QStringLiteral("title")).toString()
            );
        }
    }
/////////////////////////////////////////////////////////////////////
    {
        QJsonObject frend;
        QJsonArray friends = hash.value(QStringLiteral("friends")).toArray();
        QJsonArray::Iterator it = friends.begin();

        for(; it != friends.end(); it++) {
            frend = (*it).toObject();

            Vk::Api::instance() -> addFriend(
                QString::number(frend.value(QStringLiteral("id")).toInt()),
                frend.value(QStringLiteral("title")).toString()
            );
        }
    }

    emit moveOutProcess();
}

void VkModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
