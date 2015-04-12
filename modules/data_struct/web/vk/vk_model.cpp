#include "vk_model.h"

//#include "media/library.h"
//#include "media/player.h"
//#include "misc/func_container.h"
//#include <QDebug>

/////////////////////////////////////////////////////////////

VkModel::VkModel(QString uid, QJsonObject * hash, QObject * parent) : WebModel(uid, hash, parent) {
//    connect(IpChecker::instance(), SIGNAL(ipChanged()), this, SLOT(refresh()));
    connect(Player::instance(), SIGNAL(remoteUnprocessed()), this, SLOT(refresh()));

    if (!hash) refresh();
}

VkModel::~VkModel() {
}

void VkModel::refresh() {
    emit moveInProcess();
//    QApplication::processEvents();
    VkApi::instance() -> audioList(this, SLOT(proceedAudioList(QJsonObject &)), tab_uid);
}

void VkModel::refreshWall() {
    emit moveInProcess();
    VkApi::instance() -> wallMediaList(this, SLOT(proceedWallList(QJsonObject &)), tab_uid);
}

void VkModel::proceedWallList(QJsonObject & hash) {
    qDebug() << "DATE " << QDateTime::fromTime_t(hash.value("date").toInt());

    QJsonArray posts = hash.value("posts").toArray();

    if (posts.count() > 0) {
        QJsonArray audios;
        QJsonObject post;
        QString title;
        FolderItem * folder, * rootFolder = rootItem -> createFolder("!!!WALL!!!");
        int index = rootItem -> childRow(rootFolder);

        beginInsertRows(QModelIndex(), index, index);

        QHash<QString, IItem *> store;
        rootFolder -> accumulateUids(store);

        QJsonArray::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = (*it).toObject();
            audios = post.value("audios").toArray();

            title = post.value("title").toString();
            title = QDateTime::fromTime_t(post.value("date").toInt()).toString() + (title.isEmpty() ? "" : " : ") + title;

            folder = rootFolder -> createFolder(title);
            proceedAudioList(audios, folder, store);
//            if (folder -> childCount() == 0)
//                removeFolderPrebuild(folder);
        }
        endInsertRows();
    }

//    emit expandNeeded(index(rootFolder));
    emit moveOutProcess();
}

void VkModel::proceedAudioList(QJsonObject & hash) {
    QHash<QString, IItem *> store;

    QJsonArray albums = hash.value("albums").toArray();
    QJsonArray audios = hash.value("audio_list").toObject().value("items").toArray();

    if (albums.count() > 0 || audios.count() > 0)
        rootItem -> accumulateUids(store);

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums.count() + audios.count()); // refresh all indexes // maybe this its not good idea
    {
        if (albums.count() > 0) {
            VkFolder * folder;
            QJsonObject album;

            QJsonArray::Iterator it = albums.begin();

            for(; it != albums.end(); it++) {
                album = (*it).toObject();

                QJsonArray albumItems = album.value("items").toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createFolder<VkFolder>(
                        album.value("folder_id").toString(),
                        album.value("title").toString()
                    );

                    proceedAudioList(albumItems, folder, store);
                }
            }
        }

    /////////////////////////////////////////////////////////////////////

        if (audios.count() > 0)
            proceedAudioList(audios, rootItem, store);
    }
    endInsertRows();
    /////////////////////////////////////////////////////////////////////
    {
        QJsonObject group;
        QJsonArray groups = hash.value("groups").toArray();
        QJsonArray::Iterator it = groups.begin();

        for(; it != groups.end(); it++) {
            group = (*it).toObject();

            VkApi::instance() -> addGroup(
                QString::number(group.value("id").toInt()),
                group.value("title").toString()
            );
        }
    }
/////////////////////////////////////////////////////////////////////
    {
        QJsonObject frend;
        QJsonArray friends = hash.value("friends").toArray();
        QJsonArray::Iterator it = friends.begin();

        for(; it != friends.end(); it++) {
            frend = (*it).toObject();

            VkApi::instance() -> addFriend(
                QString::number(frend.value("id").toInt()),
                frend.value("title").toString()
            );
        }
    }

//    deleteRemoved(store);
    emit moveOutProcess();
}

void VkModel::proceedAudioList(QJsonArray & collection, FolderItem * parent, QHash<QString, IItem *> & store) {
    QJsonObject itm;
    VkItem * newItem;
    QString uri, id, owner;
    QVariant uid;
    QList<IItem *> items;

    QJsonArray::Iterator it = collection.begin();

    for(; it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        id = QString::number(itm.value("id").toInt());
        owner = QString::number(itm.value("owner_id").toInt());
        uid = WebItem::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        uri = itm.value("url").toString();
        uri = uri.section('?', 0, 0); // remove extra info from url

        items = store.values(uid.toString());

        if (items.isEmpty()) {
            newItem = new VkItem(
                id,
                uri,
                itm.value("artist").toString() + " - " + itm.value("title").toString(),
                parent
            );

            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromSeconds(itm.value("duration").toInt(0)));
            if (itm.contains("genre_id"))
                newItem -> setGenre(VkGenres::instance() -> toStandartId(itm.value("genre_id").toInt()));
        } else {
            QList<IItem *>::Iterator it_it = items.begin();

            for(; it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }
    }
}

//void VkModel::proceedAudioListUpdate(QJsonObject & obj, QHash<ModelItem *, QString> & collation) {
//    QJsonObject iterObj;
//    QJsonArray ar = obj.value("response").toArray();
//    QString uid;
//    ModelItem * item;

//    foreach(QJsonValue obj, ar) {
//        iterObj = obj.toObject();
//        uid = QString::number(iterObj.value("owner_id").toInt()) + "_" + QString::number(iterObj.value("id").toInt());

//        item = collation.key(uid);
//        item -> setPath(iterObj.value("url").toString());
//    }
//}
