#include "vk_model.h"
//#include "media/library.h"
//#include "media/player.h"
//#include "misc/func_container.h"
//#include <QDebug>

/////////////////////////////////////////////////////////////

VkModel::VkModel(QString uid, QJsonObject * hash, QObject * parent) : WebModel(uid, hash, parent) {
//    connect(IpChecker::instance(), SIGNAL(ipChanged()), this, SLOT(refresh()));
    connect(Player::instance(), SIGNAL(remoteUnprocessed()), this, SLOT(refresh()));
}

VkModel::~VkModel() {
}

void VkModel::refresh() {
    emit moveInProcess();
//    QApplication::processEvents();
    VkApi::instance() -> audioList(this, SLOT(proceedAudioList(QJsonObject &)), tab_uid);
//    emit moveOutProcess();
}

void VkModel::refreshWall() {
    emit moveInProcess();
//    QApplication::processEvents();
    VkApi::instance() -> wallMediaList(this, SLOT(proceedWallList(QJsonObject &)), tab_uid);
}

void VkModel::proceedWallList(QJsonObject & hash) {
    qDebug() << "DATE " << QDateTime::fromTime_t(hash.value("date").toInt());


    QJsonArray posts = hash.value("posts").toArray(), audios;
    QJsonObject post;


    if (ar.count() > 0) {
        beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums.count() + audios.count()); // refresh all indexes // maybe this its not good idea
        FolderItem * rootFolder = rootItem -> createFolder("!!!WALL!!!");

        QHash<IItem *, QString> store;
        rootFolder -> accumulateUids(store);


        FolderItem * folder;
        QString title;

        QJsonArray::Iterator it = posts.begin();

        for(; it != posts.end(); it++) {
            post = obj.toObject();
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
    QHash<IItem *, QString> store;
    rootItem -> accumulateUids(store);

    QJsonArray albums = hash.value("albums").toArray();
    QJsonArray audios = hash.value("audio_list").toObject().value("items").toArray();

    beginInsertRows(QModelIndex(), 0, rootItem -> childCount() + albums.count() + audios.count()); // refresh all indexes // maybe this its not good idea
    {
        if (albums.count() > 0) {
            FolderItem * folder;
            QJsonObject album;

            QJsonArray::Iterator it = albums.begin();

            for(; it != albums.end(); it++) {
                album = (*it).toObject();

                QJsonArray albumItems = album.value("items").toArray();
                if (albumItems.size() > 0) {
                    folder = rootItem -> createFolder(
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
                group.value("id").toString(),
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
                frend.value("id").toString(),
                frend.value("title").toString()
            );
        }
    }

//    deleteRemoved(store);
    emit moveOutProcess();
}

void VkModel::proceedAudioList(QJsonArray & ar, FolderItem * parent, QHash<IItem *, QString> & store) {
    QJsonObject fileIterObj;
    VkItem * newItem;
    QString id, owner, key;
    QList<ModelItem *> items;

    foreach(QJsonValue obj, ar) {
        fileIterObj = obj.toObject();

        if (fileIterObj.isEmpty()) continue;

        owner = QString::number(fileIterObj.value("owner_id").toInt());
        id = QString::number(fileIterObj.value("id").toInt());
        key = ModelItem::buildUid(owner, id);
        items = store.keys(key);
        if (items.isEmpty() && !containsUID(key)) {
            newItem = new VkFile(
                        fileIterObj.value("url").toString(),
                        fileIterObj.value("artist").toString() + " - " + fileIterObj.value("title").toString(),
                        owner,
                        id,
                        parent,
                        fileIterObj.value("genre_id").toInt(-1),
                        Duration::fromSeconds(fileIterObj.value("duration").toInt(0))
                        );

            appendRow(newItem -> toModelItem());
//            qDebug() << "NEW ITEM " << newItem -> data(0);
        } else {
            foreach(ModelItem * item, items) {
//                store.remove(item);
                item -> setPath(fileIterObj.value("url").toString());
                item -> setGenre(fileIterObj.value("genre_id").toInt(-1));
            }
            store.remove(items.first());
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
