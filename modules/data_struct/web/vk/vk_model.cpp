#include "vk_model.h"
#include "media/library.h"
#include "media/player.h"
#include "misc/func_container.h"
#include <QDebug>

///////////////////////////////////////////////////////////

VkModel::VkModel(QString uid, QJsonObject * hash, QObject *parent) : WebModel(uid, hash, parent) {
    connect(IpChecker::instance(), SIGNAL(ipChanged()), this, SLOT(refresh()));
    connect(Player::instance(), SIGNAL(remoteUnprocessed()), this, SLOT(refresh()));
}

VkModel::~VkModel() {

}

void VkModel::refresh() {
    emit showSpinner();
//    clearAll();
//    Library::instance() -> clearRemote();
//    VkApi::instance() -> clearData();
    QApplication::processEvents();
    VkApi::instance() -> audioList(FuncContainer(this, SLOT(proceedAudioList(QJsonObject &))), tabUid);
    QApplication::processEvents();
}

void VkModel::refreshWall() {
    emit showSpinner();
    QApplication::processEvents();
    VkApi::instance() -> wallMediaList(FuncContainer(this, SLOT(proceedWallList(QJsonObject &))), tabUid);
    QApplication::processEvents();
}

//void VkModel::refresh() {
//    emit showSpinner();
//    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Update !!!!!!!!!!!!!!!!!!!!";
////    clearAll();
////    VkApi::instance() -> clearData();
//    QApplication::processEvents();

//    QHash<ModelItem*, QString> store;
//    rootItem -> accumulateUids(store);

//    VkApi::instance() -> refreshAudioList(
//                FuncContainer(this, SLOT(proceedAudioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &))),
//                store
//                );
//    QApplication::processEvents();
//    emit hideSpinner();
//}


QModelIndex VkModel::dropProcession(const QList<QUrl> & /*list*/) {
//    ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path());
//    filesRoutine(index, list);
//    return model -> index(index);
    return QModelIndex();
}

void VkModel::filesRoutine(ModelItem * /*index*/, QFileInfo /*currFile*/){
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

void VkModel::filesRoutine(ModelItem * /*index*/, QList<QUrl> /*list*/){
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

void VkModel::proceedWallList(QJsonObject & hash) {
//    hash.value("date").toInt()// top date
    qDebug() << "DATE " << QDateTime::fromTime_t(hash.value("date").toInt());
    QJsonArray filesAr, ar = hash.value("posts").toArray();
    QJsonObject iterObj;

    ModelItem * rootFolder = addFolder("!!!WALL!!!", rootItem);

    QHash<ModelItem*, QString> store;
    rootFolder -> accumulateUids(store);

    if (ar.count() > 0) {
        ModelItem * folder;
        QString title;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            filesAr = iterObj.value("audios").toArray();

            title = iterObj.value("title").toString();
            title = QDateTime::fromTime_t(iterObj.value("date").toInt()).toString() + (title.isEmpty() ? "" : " : ") + title;

            folder = addFolder(title, rootFolder);
            proceedAudioList(filesAr, folder, store);
            if (folder -> childCount() == 0)
                removeFolderPrebuild(folder);
        }
    }

    TreeModel::refresh();
    emit expandNeeded(index(rootFolder));
    emit hideSpinner();
}

void VkModel::proceedAudioList(QJsonObject & hash) {
    QHash<ModelItem*, QString> store;
    rootItem -> accumulateUids(store);

    QJsonArray filesAr, ar = hash.value("albums").toArray();
    QJsonObject iterObj;

//    qDebug() << ar;

    if (ar.count() > 0) {
        ModelItem * folder;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();

            filesAr = iterObj.value("items").toArray();

            if (filesAr.size() > 0) {
                folder = addFolder(iterObj.value("title").toString(), rootItem, QString::number(iterObj.value("folder_id").toInt()));

                proceedAudioList(filesAr, folder, store);
            }
        }
    }

/////////////////////////////////////////////////////////////////////
    ar = hash.value("audio_list").toObject().value("items").toArray();

//    qDebug() << ar;

    if (ar.count() > 0) {        
        proceedAudioList(ar, root(), store);
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("groups").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            VkApi::instance() -> addGroup(
                            QString::number(iterObj.value("id").toInt()),
                            iterObj.value("title").toString()
                        );
        }
    }
/////////////////////////////////////////////////////////////////////
    ar = hash.value("friends").toArray();

    if (ar.count() > 0) {
        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            VkApi::instance() -> addFriend(
                            QString::number(iterObj.value("id").toInt()),
                            iterObj.value("title").toString()
                        );
        }
    }

    deleteRemoved(store);

    TreeModel::refresh();
    emit hideSpinner();

    if (count == 0)
        emit showMessage(QString("This object did not have any items. Use wall parse from context menu"));
}

void VkModel::proceedAudioList(QJsonArray & ar, ModelItem * parent, QHash<ModelItem*, QString> & store) {
    QJsonObject fileIterObj;
    VkFile * newItem;
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

void VkModel::proceedAudioListUpdate(QJsonObject & obj, QHash<ModelItem *, QString> & collation) {
    QJsonObject iterObj;
    QJsonArray ar = obj.value("response").toArray();
    QString uid;
    ModelItem * item;

    foreach(QJsonValue obj, ar) {
        iterObj = obj.toObject();
        uid = QString::number(iterObj.value("owner_id").toInt()) + "_" + QString::number(iterObj.value("id").toInt());

        item = collation.key(uid);
        item -> setPath(iterObj.value("url").toString());
    }
}
