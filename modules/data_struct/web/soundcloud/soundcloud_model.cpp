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

                    int folderItemsAmount = proceedAudioList(albumItems, folder);
                    folder -> updateItemsCountInBranch(folderItemsAmount);
                    itemsAmount += folderItemsAmount;
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (audios.count() > 0)
            itemsAmount += proceedAudioList(audios, rootItem);
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

int SoundcloudModel::proceedAudioList(QJsonArray & collection, FolderItem * parent) {
    int itemsAmount = 0;
    QJsonObject itm;
    SoundcloudItem * newItem;
    QString uri, id, owner;
    QVariant uid;
    QList<IItem *> items;
    bool original;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    QJsonArray::Iterator it = collection.begin();

    for(; it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        id = QString::number(itm.value("id").toInt());
        owner = QString::number(itm.value("user_id").toInt());
        uid = WebItem::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        uri = itm.value("download_url").toString();
        if (uri.isEmpty()) {
            uri = itm.value("stream_url").toString();
            original = false;
        } else { original = true;}
        if (uri.isEmpty()) continue;

        items = store.values(uid.toString());

        if (items.isEmpty()) {
            itemsAmount++;
            newItem = new SoundcloudItem(
                id,
                uri,
                itm.value("title").toString(),
                parent
            );

            newItem -> setVideoPath(itm.value("video_url").toString());
            newItem -> setExtension(original ? itm.value("original_format").toString() : "mp3");
            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromMillis(itm.value("duration").toInt(0)));

//            Genre::instance() -> toInt(fileIterObj.value("genre").toString())
            if (itm.contains("genre_id"))
                newItem -> setGenre(itm.value("genre_id").toInt());
        } else {
            QList<IItem *>::Iterator it_it = items.begin();

            for(; it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }
    }

    return itemsAmount;

    //    foreach(QJsonValue obj, ar) {
    //        fileIterObj = obj.toObject();

    //        if (fileIterObj.isEmpty()) continue;

    //        owner = QString::number(fileIterObj.value("user_id").toInt());
    //        id = QString::number(fileIterObj.value("id").toInt());
    //        key = ModelItem::buildUid(owner, id);
    //        items = store.keys(key);
    //        if (items.isEmpty() && !containsUID(key)) {
    //            url = fileIterObj.value("download_url").toString();
    //            if (url.isEmpty()) {
    //                url = fileIterObj.value("stream_url").toString();
    //                original = false;
    //            } else { original = true;}
    //            if (url.isEmpty()) continue;

    ////            video_url

    //            newItem = new SoundcloudFile(
    //                            url,
    //                            fileIterObj.value("title").toString(),
    //                            original ? fileIterObj.value("original_format").toString() : "mp3",
    //                            owner,
    //                            id,
    //                            parent,
    //                            Genre::instance() -> toInt(fileIterObj.value("genre").toString()),
    //                            Duration::fromMillis(fileIterObj.value("duration").toInt(0)),
    //                            original ? fileIterObj.value("original_content_size").toInt() : -1,
    //                            fileIterObj.value("bpm").toInt(0)
    //                        );

    //            appendRow(newItem -> toModelItem());
    //            qDebug() << "NEW ITEM " << original << " " << newItem -> data(0) << " " << fileIterObj.value("bpm").toInt(0);
    //        } else {
    //            foreach(ModelItem * item, items) {
    ////                store.remove(item);
    //                item -> setPath(fileIterObj.value("url").toString());
    //                item -> setGenre(fileIterObj.value("genre_id").toInt(-1));
    //            }
    //            store.remove(items.first());
    //        }
    //    }
}
