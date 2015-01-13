#include "folder_item.h"
#include "web/vk/vk_folder.h"
#include "web/vk/vk_file.h"
#include "web/soundcloud/soundcloud_file.h"
#include "web/soundcloud/soundcloud_playlist.h"
#include <QDebug>

///////////////////////////////////////////////////////////
FolderItem::FolderItem(int initState) : ModelItem(initState) {
    folders = new QHash<QString, ModelItem *>();
}

FolderItem::FolderItem(QJsonObject * hash, ModelItem *parent) : ModelItem(hash, parent) {
    folders = new QHash<QString, ModelItem *>();

    if (parent != 0) {
        parent -> foldersList() -> insert(title, this -> toModelItem());
    }

    if (hash -> contains("c")) {
        QJsonArray ar = hash -> value("c").toArray();
        QJsonObject iterObj;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            switch(iterObj.value("i").toInt()) {
                case FILE_ITEM: {
                    new FileItem(&iterObj, this -> toModelItem());
                break;}
                case FOLDER_ITEM: {
                    new FolderItem(&iterObj, this -> toModelItem());
                break;}               
                // case CUE_ITEM: {
                // new CueItem(&iter_obj, this -> toModelItem());
                // break;}
                case VK_FOLDER: {
                    new VkFolder(&iterObj, this -> toModelItem());
                break;}
                case VK_FILE: {
                    new VkFile(&iterObj, this -> toModelItem());
                break;}

                case SOUNDCLOUD_FILE: {
                    new SoundcloudFile(&iterObj, this -> toModelItem());
                break;}
                case SOUNDCLOUD_PLAYLIST: {
                    new SoundcloudPlaylist(&iterObj, this -> toModelItem());
                break;}
            }
        }
    }
}

FolderItem::FolderItem(const QString folderPath, QString folderTitle, ModelItem *parent, int initState)
    : ModelItem(folderPath, folderTitle, parent, -1, "", -1, "", initState) {
    folders = new QHash<QString, ModelItem *>();

    if (title.isEmpty())
        title = folderPath;

    if (parent != 0) {
        parent -> foldersList() -> insert(title, this -> toModelItem());
    }
}

FolderItem::~FolderItem() {
    qDeleteAll(childItems);

    delete folders;
}

void FolderItem::openLocation() {
    QDesktopServices::openUrl(toUrl());
}

bool FolderItem::removePhysicalObject() {
    bool res = true;

    foreach(ModelItem * item, *childItemsList()) {
        res &= item -> removePhysicalObject();
    }



//    QDir delDir(fullPath());
//    if (fullPath().split('/').length() >= 2) {
//        return delDir.removeRecursively();
//    }

    return res;
}

bool FolderItem::isExist() const {
    return QDir(fullPath()).exists();
}

bool FolderItem::isFolder() const {
    return true;
}

QJsonObject FolderItem::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = FOLDER_ITEM;

    if (childItems.length() > 0) {
        QJsonArray ar = QJsonArray();
        for(int i = 0; i < childItems.length(); i++)
            ar.append(childItems.at(i) -> toJSON());

        root["c"] = ar;
    }

    return root;
}


QList<ModelItem *> * FolderItem::childItemsList() {
    return &childItems;
}

ModelItem *FolderItem::child(int row) {
    return childItems.value(row);
}

int FolderItem::childTreeCount() const {
    int ret = childItems.count() - foldersList() -> count();
    foreach(ModelItem * folder, foldersList() -> values()) {
        ret += folder -> childTreeCount();
    }


//    int ret = 0;
//    foreach(ModelItem * childItem, childItems) {
//        if (childItem -> folders == 0) // not is unprocessed
//            ret += 1;
//        else
//            ret += childItem -> childTreeCount();
//    }

    return ret;
}

int FolderItem::childCount() const {
    return childItems.count();
}

void FolderItem::insertChild(int pos, ModelItem *item) {
    childItems.insert(pos, item);
}

void FolderItem::appendChild(ModelItem *item) {
    childItems.append(item);
}

bool FolderItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

void FolderItem::dropExpandProceedFlags() {
    getState() -> unsetProceed();
    foreach(ModelItem *item, folders -> values()) {
        item -> dropExpandProceedFlags();
    }
}

void FolderItem::changeCheckedState(bool checked) {
    ModelItem::changeCheckedState(checked);

    foreach(ModelItem * item, childItems)
        item -> changeCheckedState(checked);
}


QHash<QString, ModelItem *> * FolderItem::foldersList() const {
    return folders;
}
int FolderItem::removeFolder(QString name) {
    return folders -> remove(name);
}
