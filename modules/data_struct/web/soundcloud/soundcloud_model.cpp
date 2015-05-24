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
    SoundcloudApi::instance() -> getUidInfo(
        this,
        retryPlaing ? SLOT(proceedAudioListAndRetry(QJsonObject &)) : SLOT(proceedAudioList(QJsonObject &)),
        tab_uid
    );
}

void SoundcloudModel::proceedAudioList(QJsonObject & hash) {
    QJsonArray albums = hash.value("playlists").toArray();
    QJsonArray audios = hash.value("audio_list").toArray();
    int itemsAmount = 0;

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums.count() + audios.count()); // refresh all indexes // maybe this its not good idea
    {
        if (albums.count() > 0) {
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

        if (audios.count() > 0)
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
