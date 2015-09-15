#include "od_model.h"
#include "media/player.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////

void OdModel::refresh(bool retryPlaing) {
    emit moveInProcess();
    QApplication::processEvents();
    Od::Api::instance() -> objectInfo((tab_uid == Od::Api::instance() -> userID() ? QString() : tab_uid), Func(this, retryPlaing ? "proceedAudioListAndRetry" : "proceedAudioList"));
}

void OdModel::proceedAudioList(Json::Obj & hash) {
    //{"albums":[{"ensemble":"","id":82297694950393,"name":"Другие песни"}],"artists":[{"id":82297693897464,"name":"Kaka 47"}],"chunk":{"count":10},"friends":[{"fullName":"Юрий Бойко","gender":1,"id":"511343312018","name":"Юрий","surname":"Бойко"}],"totalTracks":1,"tracks":[{"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}]}

    Json::Arr audios = hash.arr(QStringLiteral("tracks"));

    if (audios.isEmpty()) {
        int totalTracks = hash.intVal(QStringLiteral("totalTracks"));
        if (totalTracks > 0) {
            audios = Od::Api::instance() -> userInfo(hash.numStr(QStringLiteral("me"))).arr(QStringLiteral("tracks"));
        }
    }

    Json::Arr playlists = hash.arr(QStringLiteral("playlists"));

//    Json::Arr albums = hash.value(Soundcloud::playlist_key).toArray();
//    Json::Arr audios = hash.value(Soundcloud::audio_list_key).toArray();
    int itemsAmount = 0, albums_count = 0, playlists_count = playlists.size()/*Soundcloud::Api::extractCount(albums)*/, audios_count = audios.size();//Soundcloud::Api::extractCount(audios);

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count + playlists_count); // refresh all indexes // maybe this its not good idea

        if (playlists_count > 0) {
            OdFolder * folder;
            Json::Obj playlist;

            for(Json::Arr::Iterator it = playlists.begin(); it != playlists.end(); it++) {
                playlist = (*it).toObject();

                int items_amount = playlist.intVal(QStringLiteral("count"));
                if (items_amount > 0) {
                    QString pid = playlist.numStr(QStringLiteral("id"));

                    folder = rootItem -> createFolder<OdFolder>(
                        pid,
                        playlist.str(QStringLiteral("name"))
                    );

                    folder -> setOwner(playlist.str(QStringLiteral("owner")));

                    Json::Arr tracks = Od::Api::instance() -> playlistInfo(pid, items_amount);

                    Od::Api::extractCount(tracks);
                    int folderItemsAmount = proceedOdList(tracks, folder);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
                }


//                Json::Arr albumItems = album.value(Soundcloud::tracks_key).toArray();
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
//        Json::Obj group;
//        Json::Arr groups = hash.value(Soundcloud::groups_key).toArray();

//        for(Json::Arr::Iterator group_it = groups.begin(); group_it != groups.end(); group_it++) {
//            Json::Arr group_part = (*group_it).toArray();
//            for(Json::Arr::Iterator it = group_part.begin(); it != group_part.end(); it++) {
//                group = (*it).toObject();

//                Soundcloud::Api::instance() -> addGroup(
//                    QString::number(group.value(Soundcloud::id_key).toInt()),
//                    group.value(Soundcloud::name_key).toString()
//                );
//            }
//        }
//    }
//    /////////////////////////////////////////////////////////////////////
//    Json::Obj frend;
//    QString name;

    {
        Json::Arr friends = hash.arr(QStringLiteral("friends"));

        for(Json::Arr::Iterator friend_it = friends.begin(); friend_it != friends.end(); friend_it++) {
            Json::Obj frend = (*friend_it).toObject();

            Od::Api::instance() -> addFriend(
                frend.str(QStringLiteral("id")),
                frend.str(QStringLiteral("fullName"))
            );
        }
    }

    emit moveOutProcess();
}

void OdModel::proceedAudioListAndRetry(Json::Obj & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
