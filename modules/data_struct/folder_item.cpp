//#include "folder_item.h"
#include "item_index.h"

using namespace Playo3;

///////////////////////////////////////////////////////////
FolderItem::FolderItem(int initState) : ItemInterface(0, initState) {

}

FolderItem::FolderItem(QJsonObject * hash, FolderItem * parent) : ItemInterface(parent, hash), inBranchCount(hash -> value(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {
    if (parent != 0)
        parent -> declareFolder(_title, this);

    if (hash -> contains(JSON_TYPE_CHILDS)) {
        QJsonArray ar = hash -> value(JSON_TYPE_CHILDS).toArray();
        QJsonObject iterObj;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            switch(iterObj.value(JSON_TYPE_ITEM_TYPE).toInt()) {
                case FILE_ITEM: {
                    inBranchCount++;
                    new FileItem(&iterObj, this);
                break;}
                case FOLDER_ITEM: {
                    inBranchCount += (new FolderItem(&iterObj, this)) -> inBranchCount;
                break;}               
                // case CUE_ITEM: {
                // new CueItem(&iter_obj, this); // ?
                // break;}
//                case VK_FOLDER: {
//                    inBranchCount += (new VkFolder(&iterObj, this)) -> inBranchCount;
//                break;}
//                case VK_FILE: {
//                    inBranchCount++;
//                    new VkFile(&iterObj, this);
//                break;}

//                case SOUNDCLOUD_FILE: {
//                    inBranchCount++;
//                    new SoundcloudFile(&iterObj, this);
//                break;}
//                case SOUNDCLOUD_PLAYLIST: {
//                    inBranchCount += (new SoundcloudPlaylist(&iterObj, this)) -> inBranchCount;
//                break;}
            }
        }
    }
}

FolderItem::FolderItem(const QString folderPath, QString folderTitle, FolderItem * parent, int initState)
    : ItemInterface(parent, folderPath, folderTitle, "", -1, initState) {

    if (_title.isEmpty())
        _title = folderPath;

    if (parent != 0)
        parent -> declareFolder(_title, this);
}

FolderItem::~FolderItem() {
    if (parentItem)
        parentItem -> undeclareFolder(_title);

    qDeleteAll(childItems);
}

bool FolderItem::removePhysicalObject() {
    bool res = true;

    foreach(ItemInterface * item, childItems) {
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

QJsonObject FolderItem::toJson() {
    QJsonObject root = ItemInterface::toJson();

    root[JSON_TYPE_ITEM_TYPE] = FOLDER_ITEM;

    if (childItems.length() > 0) {
        root[JSON_TYPE_CONTAINER_ITEMS_COUNT] = inBranchCount;

        QJsonArray ar = QJsonArray(); // TODO: rewrite on iteration through ++
        for(int i = 0; i < childItems.length(); i++)
            ar.append(childItems.at(i) -> toJson());

        root[JSON_TYPE_CHILDS] = ar;
    }

    return root;
}

FolderItem * FolderItem::createFolderPath(QString path) {
    QStringList list = path.split('/', QString::SkipEmptyParts);
    if (list.isEmpty())
        return this;
    return createFolder(list.takeFirst(), &list);
}

FolderItem * FolderItem::createFolder(QString name, QStringList * list) {
    FolderItem * curr = folders.value(name, 0);

    if (!curr)
        curr = new FolderItem(name, name, this);

    if (list && !list -> isEmpty())
        return curr -> createFolder(list -> takeFirst(), list);
    else
        return curr;
}


//int FolderItem::childTreeCount() const {
//    int ret = childItems.count() - foldersList() -> count();
//    foreach(ItemInterface * folder, foldersList() -> values()) {
//        ret += folder -> childTreeCount();
//    }


////    int ret = 0;
////    foreach(ItemInterface * childItem, childItems) {
////        if (childItem -> folders == 0) // not is unprocessed
////            ret += 1;
////        else
////            ret += childItem -> childTreeCount();
////    }

//    return ret;
//}

//bool FolderItem::insertChildren(int position, int count, int columns) {
//    if (position < 0 || position > childItems.size())
//        return false;

//    for (int row = 0; row < count; ++row) {
//        QVector<QVariant> data(columns);
//        ItemInterface * item = new ItemInterface(data, this);
//        childItems.insert(position, item);
//    }

//    return true;
//}

bool FolderItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

void FolderItem::dropExpandProceedFlags() {
    unset(proceeded);
    foreach(FolderItem * item, folders.values())
        item -> dropExpandProceedFlags();
}

void FolderItem::updateCheckedState(bool checked) {
    ItemInterface::updateCheckedState(checked);

    foreach(ItemInterface * item, childItems)
        item -> updateCheckedState(checked);
}

void FolderItem::shuffle() { //TODO: test needed
    qsrand((uint)QTime::currentTime().msec());
    int n = childItems.count() - 1;

    for (int i = 0; i < n; ++i)
        childItems.swap(i, qrand() % n);/*((n + 1) - i) + i)*/;

    foreach(FolderItem * item, folders.values())
        item -> shuffle();
}
