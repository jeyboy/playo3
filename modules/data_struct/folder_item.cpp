#include "item_index.h"

using namespace Playo3;

///////////////////////////////////////////////////////////
FolderItem::FolderItem(int initState) : ItemInterface(0, initState), inBranchCount(0) {

}

FolderItem::FolderItem(QJsonObject * hash, FolderItem * parent) : ItemInterface(parent, hash -> take(JSON_TYPE_STATE).toInt()), inBranchCount(hash -> take(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {
    if (hash -> contains(JSON_TYPE_CHILDS)) {
        QJsonArray ar = hash -> take(JSON_TYPE_CHILDS).toArray();
        QJsonObject iterObj;

        foreach(QJsonValue obj, ar) {
            iterObj = obj.toObject();
            switch(iterObj.take(JSON_TYPE_ITEM_TYPE).toInt()) {
                case FILE_ITEM: {
                    new FileItem(&iterObj, this);
                break;}
                case FOLDER_ITEM: {
                    new FolderItem(&iterObj, this);
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

    attrs = hash -> toVariantMap();
    if (parent != 0)
        parent -> declareFolder(title().toString(), this);
}

FolderItem::FolderItem(QString folderPath, QString folderTitle, FolderItem * parent, int pos, int initState)
    : ItemInterface(parent, folderTitle, pos, initState), inBranchCount(0) {

    setPath(folderPath);

    if (parent != 0)
        parent -> declareFolder(folderTitle, this);
}

FolderItem::FolderItem(QString folderTitle, FolderItem * parent, int pos, int initState)
    : ItemInterface(parent, folderTitle, pos, initState), inBranchCount(0) {

    if (parent != 0)
        parent -> declareFolder(folderTitle, this);
}

FolderItem::~FolderItem() {
    if (_parent)
        _parent -> undeclareFolder(title().toString());

    qDeleteAll(children);
}

void FolderItem::backPropagateItemsCountInBranch(int offset) {
    inBranchCount += offset;

    if (_parent)
        _parent -> backPropagateItemsCountInBranch(offset);
}

QVariant FolderItem::data(int column) const {
    switch(column) {
        case Qt::ToolTipRole:  return title().toString() + "(" + QString::number(inBranchCount) + " items)";
        case IEXECCOUNTS:      return inBranchCount;
        default:               return ItemInterface::data(column);
    }
}

bool FolderItem::removePhysicalObject() {
    bool res = true;

    foreach(ItemInterface * item, children) {
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

    if (children.length() > 0) {
        root[JSON_TYPE_CONTAINER_ITEMS_COUNT] = inBranchCount;

        QJsonArray ar = QJsonArray(); // TODO: rewrite on iteration through ++
        for(int i = 0; i < children.length(); i++)
            ar.append(children.at(i) -> toJson());

        root[JSON_TYPE_CHILDS] = ar;
    }

    return root;
}

FolderItem * FolderItem::createFolderPath(QString path) { // usable only for tree
    QStringList list = path.split('/', QString::SkipEmptyParts);
    if (list.isEmpty())
        return this;
    return createFolder(list.takeFirst(), &list);
}

FolderItem * FolderItem::createFolder(QString name, QStringList * list, int pos) {
    FolderItem * curr = folders.value(name, 0);

    if (!curr)
        curr = new FolderItem(name, this);

    if (list && !list -> isEmpty())
        return curr -> createFolder(list -> takeFirst(), list, pos);
    else
        return curr;
}

FolderItem * FolderItem::findNearestFolder(QStringList * list) { // find last exist folder in the path
    if (list -> isEmpty()) return this;
    FolderItem * curr = folders.value(list -> at(0), 0);
    if (!curr) return this;
    list -> removeFirst();
    return curr -> findNearestFolder(list);
}

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

int FolderItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > children.size())
        return 1;

    ItemInterface * it;
    int totalItems = 0;

    for (int row = 0; row < count; ++row) {
        it = children.takeAt(position);
        totalItems -= it -> itemsCountInBranch();
        delete it;
    }

    backPropagateItemsCountInBranch(totalItems);
    return totalItems;
}

void FolderItem::propagateFolderSetFlag(ItemStateFlag flag) {
    set(flag);
    foreach(FolderItem * item, folders.values())
        item -> propagateFolderSetFlag(flag);
}
void FolderItem::propagateFolderUnsetFlag(ItemStateFlag flag) {
    unset(flag);
    foreach(FolderItem * item, folders.values())
        item -> propagateFolderUnsetFlag(flag);
}

void FolderItem::propagateCheckedState(bool checked) {
    ItemInterface::updateCheckedState(checked);

    foreach(ItemInterface * item, children)
        item -> updateCheckedState(checked);
}

void FolderItem::shuffle() { //TODO: test needed
    qsrand((uint)QTime::currentTime().msec());
    int n = children.count() - 1;

    for (int i = 0; i < n; ++i)
        children.swap(i, qrand() % n);/*((n + 1) - i) + i)*/;

    foreach(FolderItem * item, folders.values())
        item -> shuffle();
}
