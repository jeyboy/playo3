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

void SoundcloudModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray albums = hash.value(Soundcloud::playlist_key).toArray();
    QJsonArray audios = hash.value(Soundcloud::audio_list_key).toArray();
    int itemsAmount = 0, albums_count = Soundcloud::Api::extractCount(albums), audios_count = Soundcloud::Api::extractCount(audios);

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudFolder * folder;
            QJsonObject album;

            for(QJsonArray::Iterator album_part = albums.begin(); album_part != albums.end(); album_part++) {
                QJsonArray part_arr = (*album_part).toArray();
                for(QJsonArray::Iterator it = part_arr.begin(); it != part_arr.end(); it++) {
                    album = (*it).toObject();

                    QJsonArray albumItems = album.value(Soundcloud::tracks_key).toArray();
                    if (albumItems.size() > 0) {
                        folder = rootItem -> createFolder<SoundcloudFolder>(
                            album.value(Soundcloud::id_key).toString(),
                            album.value(Soundcloud::title_key).toString()
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
        QJsonArray groups = hash.value(Soundcloud::groups_key).toArray();

        for(QJsonArray::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
            QJsonArray group_part = (*group_it).toArray();
            for(QJsonArray::Iterator it = group_part.begin(); it != group_part.end(); it++) {
                group = (*it).toObject();

                Soundcloud::Api::instance() -> addGroup(
                    QString::number(group.value(Soundcloud::id_key).toInt()),
                    group.value(Soundcloud::name_key).toString()
                );
            }
        }
    }
    /////////////////////////////////////////////////////////////////////
    QJsonObject frend;
    QString name;

    {        
        QJsonArray friends = hash.value(Soundcloud::followings_key).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            QJsonArray friend_part = (*friend_it).toArray();
            for(QJsonArray::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.value(Soundcloud::full_name_key).toString();
                if (name.isEmpty())
                    name = frend.value(Soundcloud::username_key).toString();

                Soundcloud::Api::instance() -> addFriend(
                    QString::number(frend.value(Soundcloud::id_key).toInt()),
                    name
                );
            }
        }
    }

    {
        QJsonArray friends = hash.value(Soundcloud::followers_key).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            QJsonArray friend_part = (*friend_it).toArray();
            for(QJsonArray::Iterator it = friend_part.begin(); it != friend_part.end(); it++) {
                frend = (*it).toObject();

                name = frend.value(Soundcloud::full_name_key).toString();
                if (name.isEmpty())
                    name = frend.value(Soundcloud::username_key).toString();

                Soundcloud::Api::instance() -> addFriend(
                    QString::number(frend.value(Soundcloud::id_key).toInt()),
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
