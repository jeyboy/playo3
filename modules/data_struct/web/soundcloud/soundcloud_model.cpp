#include "soundcloud_model.h"
#include "media/player.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////

void SoundcloudModel::refresh(bool retryPlaing) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();
    SoundcloudApi::instance() -> objectInfo(tab_uid, Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
}

void SoundcloudModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray albums = hash.value("playlists").toArray();
    QJsonArray audios = hash.value("audio_list").toArray();
    int itemsAmount = 0, albums_count = SoundcloudApi::extractCount(albums), audios_count = SoundcloudApi::extractCount(audios);

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudFolder * folder;
            QJsonObject album;

            for(QJsonArray::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                QJsonArray part_arr = (*album_part).toArray();
                for(QJsonArray::Iterator it = part_arr.begin(); it != part_arr.end(); it++) {
                    album = (*it).toObject();

                    QJsonArray albumItems = album.value("tracks").toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createFolder<SoundcloudFolder>(
                            album.value("id").toString(),
                            album.value("title").toString()
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
        QJsonObject group;
        QJsonArray groups = hash.value("groups").toArray();

        for(QJsonArray::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
            QJsonArray group_part = (*group_it).toArray();
            for(QJsonArray::Iterator it = group_part.begin(); it != group_part.end(); it++) {
                group = (*it).toObject();

                SoundcloudApi::instance() -> addGroup(
                    QString::number(group.value("id").toInt()),
                    group.value("name").toString()
                );
            }
        }
    }
    /////////////////////////////////////////////////////////////////////
    QJsonObject frend;
    QString name;

    {        
        QJsonArray friends = hash.value("followings").toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            QJsonArray friend_part = (*friend_it).toArray();
            for(QJsonArray::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.value("full_name").toString();
                if (name.isEmpty())
                    name = frend.value("username").toString();

                SoundcloudApi::instance() -> addFriend(
                    QString::number(frend.value("id").toInt()),
                    name
                );
            }
        }
    }

    {
        QJsonArray friends = hash.value("followers").toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            QJsonArray friend_part = (*friend_it).toArray();
            for(QJsonArray::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.value("full_name").toString();
                if (name.isEmpty())
                    name = frend.value("username").toString();

                SoundcloudApi::instance() -> addFriend(
                    QString::number(frend.value("id").toInt()),
                    name
                );
            }
        }
    }

    emit moveOutProcess();
}

void SoundcloudModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
