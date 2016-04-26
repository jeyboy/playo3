#include "od_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void OdModel::refresh(bool retryPlaing) {
    emit moveInProcess();

    Od::Api::obj().objectInfo(
        (sttngs.uid == Od::Api::obj().userID() ? QString() : sttngs.uid),
        new Func(
            this,
            retryPlaing ? SLOT(proceedAudioListAndRetry(QJsonObject &)) : SLOT(proceedAudioList(QJsonObject &))
        )
    );
}

void OdModel::proceedAudioList(QJsonObject & hash) {
    //{"albums":[{"ensemble":"","id":82297694950393,"name":"Другие песни"}],"artists":[{"id":82297693897464,"name":"Kaka 47"}],"chunk":{"count":10},"friends":[{"fullName":"Юрий Бойко","gender":1,"id":"511343312018","name":"Юрий","surname":"Бойко"}],"totalTracks":1,"tracks":[{"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}]}

    QJsonArray audios = hash.value(QStringLiteral("tracks")).toArray();

    if (audios.isEmpty()) {
        int totalTracks = hash.value(QStringLiteral("totalTracks")).toInt();
        if (totalTracks > 0) {
            audios = Od::Api::obj().userInfo(QString::number((qint64)hash.value(QStringLiteral("me")).toDouble())).value(QStringLiteral("tracks")).toArray();
        }
    }

    QJsonArray playlists = hash.value(QStringLiteral("playlists")).toArray();

//    QJsonArray albums = hash.value(Soundcloud::playlist_key).toArray();
//    QJsonArray audios = hash.value(Soundcloud::audio_list_key).toArray();
    int itemsAmount = 0, albums_count = 0, playlists_count = playlists.size(), audios_count = audios.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count + playlists_count); // refresh all indexes // maybe this its not good idea
        if (playlists_count > 0) {
            Playlist * folder;
            QJsonObject playlist;

            for(QJsonArray::Iterator it = playlists.begin(); it != playlists.end(); it++) {
                playlist = (*it).toObject();

                int items_amount = playlist.value(QStringLiteral("count")).toInt();
                if (items_amount > 0) {
                    QString pid = QString::number((qint64)playlist.value(QStringLiteral("id")).toDouble());

                    folder = rootItem -> createPlaylist(
                        dt_playlist_od,
                        pid,
                        playlist.value(QStringLiteral("name")).toString()
                    );

                    folder -> setOwner(playlist.value(QStringLiteral("owner")).toString());

                    QJsonArray tracks = Od::Api::obj().playlistInfo(pid, items_amount);
                    int folderItemsAmount = proceedOdList(tracks, folder);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
                }


//                QJsonArray albumItems = album.value(Soundcloud::tracks_key).toArray();
//                if (albumItems.size() > 0) {
//                    folder = rootItem -> createFolder<SoundcloudFolder>(
//                        album.value(Soundcloud::id_key).toString(),
//                        album.value(Soundcloud::title_key).toString()
//                    );

//                    int folderItemsAmount = proceedScList(albumItems, folder);
//                    folder -> updateItemsCountInBranch(folderItemsAmount);
//                    itemsAmount += folderItemsAmount;
//                }
            }
        }

//    /////////////////////////////////////////////////////////////////////

        if (audios.size() > 0)
            itemsAmount += proceedOdList(audios, rootItem);
//    }
    rootItem -> updateItemsCountInBranch(itemsAmount);
    endInsertRows();
//    /////////////////////////////////////////////////////////////////////

//    {
//        QJsonObject group;
//        QJsonArray groups = hash.value(Soundcloud::groups_key).toArray();

//        for(QJsonArray::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
//            QJsonArray group_part = (*group_it).toArray();
//            for(QJsonArray::Iterator it = group_part.begin(); it != group_part.end(); it++) {
//                group = (*it).toObject();

//                Soundcloud::Api::instance() -> addGroup(
//                    QString::number(group.value(Soundcloud::id_key).toInt()),
//                    group.value(Soundcloud::name_key).toString()
//                );
//            }
//        }
//    }
//    /////////////////////////////////////////////////////////////////////
//    QJsonObject frend;
//    QString name;

    {
        QJsonArray friends = hash.value(QStringLiteral("friends")).toArray();

        for(QJsonArray::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            QJsonObject frend = (*friend_it).toObject();

            Od::Api::obj().addFriend(
                Linkable(
                    frend.value(QStringLiteral("id")).toString(),
                    frend.value(QStringLiteral("fullName")).toString()
                )
            );
        }
    }

    emit moveOutProcess();
}
