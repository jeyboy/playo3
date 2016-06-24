#include "soundcloud_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void SoundcloudModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();
    Soundcloud::Api::obj().objectInfo(
        sttngs.uid,
        new Func(this, SLOT(proceedJson(QJsonObject &)))
    );
}

void SoundcloudModel::proceedJson(QJsonObject & hash) {
    QJsonArray albums = hash.value(Soundcloud::tkn_playlist).toArray();
    QJsonArray audios = hash.value(Soundcloud::tkn_audio_list).toArray();
    int itemsAmount = 0, albums_count = albums.size(), audios_count = audios.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            Playlist * folder;
            QJsonObject album;

            for(QJsonArray::Iterator it = albums.begin(); it != albums.end(); it++) {
                album = (*it).toObject();

                QJsonArray albumItems = album.value(Soundcloud::tkn_tracks).toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createPlaylist(
                        dt_playlist_sc,
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
                Linkable(
                    QString::number(group.value(Soundcloud::tkn_id).toInt()),
                    group.value(Soundcloud::tkn_name).toString(),
                    group.value(Soundcloud::tkn_permalink).toString(),
                    group.value(Soundcloud::tkn_artwork_url).toString()
                )
            );
        }
    }
    /////////////////////////////////////////////////////////////////////

    proceedFriendsList(hash.value(Soundcloud::tkn_followings).toArray());
    proceedFriendsList(hash.value(Soundcloud::tkn_followers).toArray());

    emit moveOutProcess();
}

void SoundcloudModel::proceedFriendsList(const QJsonArray & friends) {
    QJsonObject frend;
    QString name;

    for(QJsonArray::ConstIterator friend_it = friends.constBegin(); friend_it != friends.constEnd(); friend_it++) {
        frend = (*friend_it).toObject();

        name = frend.value(Soundcloud::tkn_full_name).toString();
        if (name.isEmpty())
            name = frend.value(Soundcloud::tkn_username).toString();

        Soundcloud::Api::obj().addFriend(
            Linkable(
                QString::number(frend.value(Soundcloud::tkn_id).toInt()),
                name,
                frend.value(Soundcloud::tkn_permalink).toString(),
                frend.value(Soundcloud::tkn_avatar_url).toString()
            )
        );
    }
}
