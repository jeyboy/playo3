#include "soundcloud_model.h"
#include "media/library.h"
#include "media/player.h"
#include "misc/func_container.h"
#include <QDebug>

///////////////////////////////////////////////////////////

SoundcloudModel::SoundcloudModel(QString uid, QJsonObject * hash, QObject *parent) : WebModel(uid, hash, parent) {
    if (hash == 0) {
        SoundcloudApi::instance() -> getUidInfo(FuncContainer(this, SLOT(proceedResponse(QJsonObject &))), tabUid);
    }

//    connect(Player::instance(), SIGNAL(remoteUnprocessed()), this, SLOT(refresh()));
}

SoundcloudModel::~SoundcloudModel() { }

void SoundcloudModel::refresh() {
    TreeModel::refresh();
//    emit showSpinner();
//    QApplication::processEvents();
//    SoundcloudApi::instance() -> getUidInfo(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
//    QApplication::processEvents();
}

QModelIndex SoundcloudModel::dropProcession(const QList<QUrl> & /*list*/) {
//    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path());
//    filesRoutine(index, list);
//    return model -> index(index);
    return QModelIndex();
}

void SoundcloudModel::filesRoutine(ModelItem * /*index*/, QFileInfo /*currFile*/){
//    QFileInfoList fileList = folderFiles(currFile);

//    foreach(QFileInfo file, fileList) {
//        FileItem * fi = new FileItem(file.fileName(), index);
//        model -> appendRow(fi -> toModelItem());
//    }

//    QFileInfoList folderList = folderDirectories(currFile);

//    foreach(QFileInfo file, folderList) {
//        ModelItem * new_index = model -> addFolder(file.fileName(), index);
//        filesRoutine(new_index, file);
//    }
}

void SoundcloudModel::filesRoutine(ModelItem * /*index*/, QList<QUrl> /*list*/){
//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir()) {
//            ModelItem * new_index = model -> addFolder(file.fileName(), index);
//            filesRoutine(new_index, file);
//        } else {
//            if (Extensions::instance() -> respondToExtension(file.suffix())) {
//              FileItem * fi = new FileItem(file.fileName(), index);
//              model -> appendRow(fi -> toModelItem());
//            }
//        }
//    }
}

void SoundcloudModel::proceedResponse(QJsonObject & hash) {
    QHash<ModelItem*, QString> store;
    rootItem -> accumulateUids(store);

    QJsonArray filesAr, ar = hash.value("playlists").toArray();
    QJsonObject iterObj;

    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();

            filesAr = iterObj.value("tracks").toArray();

            if (filesAr.size() > 0) {
                folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("id").toInt()));

                proceedResponse(filesAr, folder, store);
            }
        }
    }

/////////////////////////////////////////////////////////////////////
    ar = hash.value("audio_list").toArray();

    qDebug() << ar;

    if (ar.count() > 0) {
        proceedResponse(ar, root(), store);
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("groups").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            SoundcloudApi::instance() -> addGroup(
                            QString::number(iterObj.value("id").toInt()),
                            iterObj.value("name").toString()
                        );
        }
    }
/////////////////////////////////////////////////////////////////////
    QString name;
    ar = hash.value("followings").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            name = iterObj.value("full_name").toString();
            if (name.isEmpty())
                name = iterObj.value("username").toString();

            SoundcloudApi::instance() -> addFriend(
                            QString::number(iterObj.value("id").toInt()),
                            name
                        );
        }
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("followers").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            name = iterObj.value("full_name").toString();
            if (name.isEmpty())
                name = iterObj.value("username").toString();

            SoundcloudApi::instance() -> addFriend(
                            QString::number(iterObj.value("id").toInt()),
                            name
                        );
        }
    }

    deleteRemoved(store);

    TreeModel::refresh();
    emit hideSpinner();

    if (count == 0)
        emit showMessage(QString("This object did not have any items. Use wall parse from context menu"));
}

void SoundcloudModel::proceedResponse(QJsonArray & ar, ModelItem * parent, QHash<ModelItem*, QString> & store) {
    QJsonObject fileIterObj;
    SoundcloudFile * newItem;
    QString id, owner, url, key;
    QList<ModelItem *> items;
    bool original;

    foreach(QJsonValue obj, ar) {
        fileIterObj = obj.toObject();

        if (fileIterObj.isEmpty()) continue;

        owner = QString::number(fileIterObj.value("user_id").toInt());
        id = QString::number(fileIterObj.value("id").toInt());
        key = ModelItem::buildUid(owner, id);
        items = store.keys(key);
        if (items.isEmpty() && !containsUID(key)) {
            url = fileIterObj.value("download_url").toString();
            if (url.isEmpty()) {
                url = fileIterObj.value("stream_url").toString();
                original = false;
            } else { original = true;}
            if (url.isEmpty()) continue;

//            video_url

            newItem = new SoundcloudFile(
                            url,
                            fileIterObj.value("title").toString(),
                            original ? fileIterObj.value("original_format").toString() : "mp3",
                            owner,
                            id,
                            parent,
                            Genre::instance() -> toInt(fileIterObj.value("genre").toString()),
                            Duration::fromMillis(fileIterObj.value("duration").toInt(0)),
                            original ? fileIterObj.value("original_content_size").toInt() : -1,
                            fileIterObj.value("bpm").toInt(0)
                        );

            appendRow(newItem -> toModelItem());
            qDebug() << "NEW ITEM " << original << " " << newItem -> data(0) << " " << fileIterObj.value("bpm").toInt(0);
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
