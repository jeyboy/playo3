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

    QJsonObject tracks = hash.value(block_items_audio).toObject();
    QJsonObject playlists = hash.value(block_sets_audio).toObject();
    int start_index = rootItem -> childCount(), items_amount = 0;

    QJsonArray playlists_content = playlists.value(tkn_content).toArray();

    if (playlists_content.size() > 0) {
        for(QJsonArray::Iterator it = playlists_content.begin(); it != playlists_content.end(); it++) {
            QJsonObject playlist = (*it).toObject();

            int amount = playlist.value(Od::tkn_count).toInt();
            if (amount > 0) {
                QString pid = Od::Requests::idToStr(playlist.value(Od::tkn_id));
                QString name = playlist.value(Od::tkn_name).toString();
                Playlist * folder;

                if (playlist.contains(tkn_loadable_cmd)) {
                    folder = rootItem -> createLoadablePlaylist(
                        playlist.value(tkn_loadable_cmd).toString(),
                        name, pid
                    );
                } else {
                    folder = rootItem -> createPlaylist(
                        dt_playlist_od,
                        pid, name
                    );

                    QJsonObject tracks = Od::Queries::obj().tracksByPlaylist(pid);
                    int folder_items_amount = proceedOdList(tracks, folder, dmt_audio);
                    folder -> updateItemsCountInBranch(folder_items_amount);
                    items_amount += folder_items_amount;
                }

                folder -> setOwner(playlist.value(Od::tkn_owner).toString());
            }
        }
    }

//    /////////////////////////////////////////////////////////////////////

    QJsonArray tracks_content = tracks.value(tkn_content).toArray();

        if (tracks_content.size() > 0)
            items_amount += proceedOdList(tracks, rootItem, dmt_audio);

    if (items_amount > 0) {
        beginInsertRows(QModelIndex(), start_index, start_index + items_amount);
        rootItem -> updateItemsCountInBranch(items_amount);
        endInsertRows();
    }

    emit moveOutProcess();
}
