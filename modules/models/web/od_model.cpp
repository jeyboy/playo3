#include "od_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void OdModel::refresh() {
    emit moveInProcess();

    Od::Queries::obj().userInfoAsync(
        sttngs.uid,
        new Func(this, SLOT(proceedJson(QJsonValue &)))
    );
}

void OdModel::proceedJson(QJsonValue & jval) {
    //{"albums":[{"ensemble":"","id":82297694950393,"name":"Другие песни"}],"artists":[{"id":82297693897464,"name":"Kaka 47"}],"chunk":{"count":10},"friends":[{"fullName":"Юрий Бойко","gender":1,"id":"511343312018","name":"Юрий","surname":"Бойко"}],"totalTracks":1,"tracks":[{"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}]}

    QJsonObject hash;

    if (jval.isArray()) {
        // reserved
    } else
        hash = jval.toObject();

    QJsonArray audios = hash.value(Od::tkn_tracks).toArray();
    QJsonArray playlists = hash.value(Od::tkn_playlists).toArray();

    int itemsAmount = 0, albums_count = 0, playlists_count = playlists.size(), audios_count = audios.size();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count + playlists_count); // refresh all indexes // maybe this its not good idea
        if (playlists_count > 0) {
            for(QJsonArray::Iterator it = playlists.begin(); it != playlists.end(); it++) {
                QJsonObject playlist = (*it).toObject();

                int items_amount = playlist.value(Od::tkn_count).toInt();
                if (items_amount > 0) {
                    QString pid = Od::Requests::idToStr(playlist.value(Od::tkn_id));

                    Playlist * folder = rootItem -> createPlaylist(
                        dt_playlist_od,
                        pid,
                        playlist.value(Od::tkn_name).toString()
                    );

                    folder -> setOwner(playlist.value(Od::tkn_owner).toString());

                    QJsonArray tracks = Od::Queries::obj().tracksByPlaylist(pid).toArray(); //tracksByPlaylist(pid, items_amount);
                    int folderItemsAmount = proceedOdList(tracks, folder, dmt_audio);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
                }
            }
        }

//    /////////////////////////////////////////////////////////////////////

        if (audios.size() > 0)
            itemsAmount += proceedOdList(audios, rootItem, dmt_audio);
//    }
    rootItem -> updateItemsCountInBranch(itemsAmount);
    endInsertRows();

    emit moveOutProcess();
}
