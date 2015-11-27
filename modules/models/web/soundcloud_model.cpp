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
    QJsonArray albums = hash.value(Soundcloud::tkn_playlist).toArray();
    QJsonArray audios = hash.value(Soundcloud::tkn_audio_list).toArray();
    int itemsAmount = 0, albums_count = albums.size(), audios_count = audios.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudPlaylist * folder;
            QJsonObject album;

            for(QJsonArray::Iterator it = albums.begin(); it != albums.end(); it++) {
                album = (*it).toObject();

                QJsonArray albumItems = album.value(Soundcloud::tkn_tracks).toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createPlaylist<SoundcloudPlaylist>(
                        album.value(Soundcloud::tkn_id).toString(),
                        album.value(Soundcloud::tkn_title).toString()
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
        QJsonArray groups = hash.value(Soundcloud::tkn_groups).toArray();

        for(QJsonArray::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
            group = (*group_it).toObject();

            Soundcloud::Api::obj().addGroup(
                QString::number(group.value(Soundcloud::tkn_id).toInt()),
                group.value(Soundcloud::tkn_name).toString()
            );
        }
    }
    /////////////////////////////////////////////////////////////////////
    QJsonObject frend;
    QString name;

    {        
        QJsonArray friends = hash.value(Soundcloud::tkn_followings).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            frend = (*friend_it).toObject();

            name = frend.value(Soundcloud::tkn_full_name).toString();
            if (name.isEmpty())
                name = frend.value(Soundcloud::tkn_username).toString();

            Soundcloud::Api::obj().addFriend(
                QString::number(frend.value(Soundcloud::tkn_id).toInt()),
                name
            );
        }
    }

    {
        QJsonArray friends = hash.value(Soundcloud::tkn_followers).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            frend = (*friend_it).toObject();

            name = frend.value(Soundcloud::tkn_full_name).toString();
            if (name.isEmpty())
                name = frend.value(Soundcloud::tkn_username).toString();

            Soundcloud::Api::obj().addFriend(
                QString::number(frend.value(Soundcloud::tkn_id).toInt()),
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
