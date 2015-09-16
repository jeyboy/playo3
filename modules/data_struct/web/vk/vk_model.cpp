#include "vk_model.h"
#include "media/player.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////
void VkModel::refresh(bool retryPlaing) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    Vk::Api::instance() -> userInfo(tab_uid, tab_uid == Vk::Api::instance() -> userID(), Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
}

//void VkModel::refreshWall() {
//    emit moveInProcess();
//    QApplication::processEvents();
//    VkApi::instance() -> wallAudio(tab_uid, Func(this, "proceedWallList"));
//}

void VkModel::proceedWallList(Json::Arr & posts) {
    if (posts.count() > 0) {
        Json::Arr audios;
        Json::Obj post;
        QString title;
        FolderItem * folder, * rootFolder = rootItem -> createFolder(QStringLiteral("!!!WALL!!!"));
        int index = rootItem -> childRow(rootFolder);

        beginInsertRows(QModelIndex(), index, index);

        Json::Arr::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = Json::Val::fromQVal(*it).obj();
            audios = post.arr(QStringLiteral("audios"));

            title = post.str(QStringLiteral("title"));
            title = QDateTime::fromTime_t(post.num(QStringLiteral("date"))).toString() % (title.isEmpty() ? QString() : QStringLiteral(" : ")) % title;

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

void VkModel::proceedAudioList(Json::Obj & hash) {
    Json::Arr albums = hash.arr(QStringLiteral("albums"));
    Json::Arr audios = hash.obj(QStringLiteral("audio_list")).arr(QStringLiteral("items"));
    int itemsAmount = 0, albums_count = Vk::Api::extractCount(albums);

//    beginResetModel();
    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios.count());
    {
        if (albums_count > 0) {
            VkFolder * folder;
            Json::Obj album;

            for(Json::Arr::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                Json::Arr part_arr = Json::Val::fromQVal(*album_part).arr();
                Json::Arr::Iterator it = part_arr.begin();
                for(int pos = 0; it != part_arr.end(); it++, pos++) {
                    album = Json::Val::fromQVal(*it).obj();

                    Json::Arr albumItems = album.arr(QStringLiteral("items"));
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createFolder<VkFolder>(
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
        Json::Obj group;
        Json::Arr groups = hash.arr(QStringLiteral("groups"));
        Json::Arr::Iterator it = groups.begin();

        for(; it != groups.end(); it++) {
            group = Json::Val::fromQVal(*it).obj();

            Vk::Api::instance() -> addGroup(
                group.numStr(QStringLiteral("id")),
                group.str(QStringLiteral("title"))
            );
        }
    }
/////////////////////////////////////////////////////////////////////
    {
        Json::Obj frend;
        Json::Arr friends = hash.arr(QStringLiteral("friends"));

        for(Json::Arr::Iterator it = friends.begin(); it != friends.end(); it++) {
            frend = Json::Val::fromQVal(*it).obj();

            Vk::Api::instance() -> addFriend(
                frend.numStr(QStringLiteral("id")),
                frend.str(QStringLiteral("title"))
            );
        }
    }

    emit moveOutProcess();
}

void VkModel::proceedAudioListAndRetry(Json::Obj & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
