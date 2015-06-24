#include "soundcloud_model.h"
#include "media/player.h"
#include <QDebug>

using namespace Playo3;
/////////////////////////////////////////////////////////////

SoundcloudModel::SoundcloudModel(QString uid, QJsonObject * hash, QObject * parent) : WebModel(uid, hash, parent) {}

SoundcloudModel::~SoundcloudModel() {}

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
    int itemsAmount = 0, albums_count = albums.takeAt(0).toObject().value("count").toInt(), audios_count = audios.takeAt(0).toObject().value("count").toInt();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums_count + audios_count); // refresh all indexes // maybe this its not good idea
    {
        if (albums_count > 0) {
            SoundcloudFolder * folder;
            QJsonObject album;

            QJsonArray::Iterator it = albums.begin();

            for(; it != albums.end(); it++) {
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
        QJsonArray::Iterator it = groups.begin();

        for(; it != groups.end(); it++) {
            group = (*it).toObject();

            SoundcloudApi::instance() -> addGroup(
                QString::number(group.value("id").toInt()),
                group.value("name").toString()
            );
        }
    }
    /////////////////////////////////////////////////////////////////////
    {
        QJsonObject frend;
        QJsonArray friends = hash.value("followings").toArray();
        QString name;

        for(QJsonArray::Iterator it = friends.begin(); it != friends.end(); it++) {
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

    {
        QJsonObject frend;
        QJsonArray friends = hash.value("followers").toArray();
        QJsonArray::Iterator it = friends.begin();
        QString name;

        for(; it != friends.end(); it++) {
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

    emit moveOutProcess();
}

void SoundcloudModel::proceedAudioListAndRetry(QJsonObject & hash) {
    proceedAudioList(hash);
    Player::instance() -> playIndex(Player::instance() -> playedIndex());
}
