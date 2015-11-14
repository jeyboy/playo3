#include "soundcloud_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void SoundcloudModel::refresh(bool retryPlaing) {
//    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

//    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();
    Soundcloud::Api::obj().objectInfo(tab_uid, new Func(this, retryPlaing ? SLOT(proceedAudioListAndRetry(QJsonObject &)) : SLOT(proceedAudioList(QJsonObject &))));
}

void SoundcloudModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray albums = hash.value(Soundcloud::playlist_key).toArray();
    QJsonArray audios = hash.value(Soundcloud::audio_list_key).toArray();
    int itemsAmount = 0, albums_count = albums.size(), audios_count = audios.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudPlaylist * folder;
            QJsonObject album;

            for(QJsonArray::Iterator it = albums.begin(); it != albums.end(); it++) {
                album = (*it).toObject();

                QJsonArray albumItems = album.value(Soundcloud::tracks_key).toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createPlaylist<SoundcloudPlaylist>(
                        album.value(Soundcloud::id_key).toString(),
                        album.value(Soundcloud::title_key).toString()
                    );

                    int folderItemsAmount = proceedScList(albumItems, folder);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
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
            group = (*group_it).toObject();

            Soundcloud::Api::obj().addGroup(
                QString::number(group.value(Soundcloud::id_key).toInt()),
                group.value(Soundcloud::name_key).toString()
            );
        }
    }
    /////////////////////////////////////////////////////////////////////
    QJsonObject frend;
    QString name;

    {        
        QJsonArray friends = hash.value(Soundcloud::followings_key).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            frend = (*friend_it).toObject();

            name = frend.value(Soundcloud::full_name_key).toString();
            if (name.isEmpty())
                name = frend.value(Soundcloud::username_key).toString();

            Soundcloud::Api::obj().addFriend(
                QString::number(frend.value(Soundcloud::id_key).toInt()),
                name
            );
        }
    }

    {
        QJsonArray friends = hash.value(Soundcloud::followers_key).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            frend = (*friend_it).toObject();

            name = frend.value(Soundcloud::full_name_key).toString();
            if (name.isEmpty())
                name = frend.value(Soundcloud::username_key).toString();

            Soundcloud::Api::obj().addFriend(
                QString::number(frend.value(Soundcloud::id_key).toInt()),
                name
            );
        }
    }

    emit moveOutProcess();
}

void SoundcloudModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
//    Player::obj().playIndex(Player::obj().playedIndex());
}
