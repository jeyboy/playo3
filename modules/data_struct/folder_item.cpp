#include "item_index.h"

using namespace Playo3;

int FolderItem::restoreItem(int item_type, FolderItem * parentFolder, int pos, QVariantMap & attrs) {
    switch(item_type) {
        case ITEM: {
            new FileItem(attrs, parentFolder, pos);
            return 1;
        }
        case VK_ITEM: {
            new VkItem(attrs, parentFolder, pos);
            return 1;
        }
        case SOUNDCLOUD_ITEM: {
            new SoundcloudItem(attrs, parentFolder, pos);
            return 1;
        }
//                case CUE_ITEM: {
//                    new CueItem(data -> attrs, parentFolder, pos);
//                    return 1;
//                }

        default: qDebug() << "ITEM TYPE NOT SUPPORTED YET";
    }

    return 0;
}

///////////////////////////////////////////////////////////
FolderItem::FolderItem(QJsonObject * hash, FolderItem * parent)
    : IItem(parent, hash -> take(JSON_TYPE_STATE).toInt()),
      inBranchCount(hash -> take(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {

    if (hash -> contains(JSON_TYPE_CHILDS)) {
        QJsonArray ar = hash -> take(JSON_TYPE_CHILDS).toArray();
        QJsonObject iterObj;

        for(QJsonArray::Iterator it = ar.begin(); it!= ar.end(); it++) {
            iterObj = (*it).toObject();
            switch(iterObj.take(JSON_TYPE_ITEM_TYPE).toInt()) {
                case ITEM: {
                    new FileItem(&iterObj, this);
                break;}
                case PLAYLIST: {
                    new FolderItem(&iterObj, this);
                break;}

                case WEB_ITEM: {
                    new WebItem(&iterObj, this);
                break;}

                case VK_ITEM: {
                    new VkItem(&iterObj, this);
                break;}
                case VK_PLAYLIST: {
                    new VkFolder(&iterObj, this);
                break;}
                case SOUNDCLOUD_ITEM: {
                    new SoundcloudItem(&iterObj, this);
                break;}
                case SOUNDCLOUD_PLAYLIST: {
                    new SoundcloudFolder(&iterObj, this);
                break;}
                // case CUE_ITEM: {
                // new CueItem(&iter_obj, this); // ?
                // break;}
            }
        }
    }

    attrs = hash -> toVariantMap();
    if (parent != 0)
        parent -> declareFolder(folderUid(), this);
}

FolderItem::FolderItem(QString folderPath, QString folderTitle, FolderItem * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), inBranchCount(0) {

    setPath(folderPath);

    if (parent != 0)
        parent -> declareFolder(folderUid(), this);
}

FolderItem::FolderItem(QString folderTitle, FolderItem * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), inBranchCount(0) {

    if (parent != 0)
        parent -> declareFolder(folderUid(), this);
}

FolderItem::FolderItem(QString folderTitle, FolderItem * parent, QString uid, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), inBranchCount(0) {

    setUid(uid);

    if (parent != 0)
        parent -> declareFolder(folderUid(), this);
}

FolderItem::~FolderItem() {
    if (_parent)
        _parent -> undeclareFolder(folderUid());

    bool remove_marked = is(mark_on_removing);

    QList<IItem *>::Iterator begin = children.begin(), end = children.end();
    while (begin != end) {
        if (remove_marked) (*begin) -> set(mark_on_removing);
        delete *begin;
        ++begin;
    }

    if (remove_marked)
        removePhysicalObject();
}

void FolderItem::linkNode(FolderItem * node) {
    node -> setParent(this);
    declareFolder(node -> folderUid(), node);
    declareChild(node);
    node -> backPropagateItemsCountInBranch(node -> childCount() - node -> foldersAmount());
}

void FolderItem::accumulateUids(QHash<QString, IItem *> & store) {
    QVariant item_uid;

    for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++) {
        if ((*it) -> isRemote()) {
            if (!(*it) -> isContainer()) {
                item_uid = (*it) -> toUid();
                if (item_uid.isValid())
                    store.insert(item_uid.toString(), (*it));
            }
        }
    }
}

QVariantList FolderItem::childrenUids(int position, int count) {
    QVariantList uids;

    if (position < 0 || position + count > children.size())
        return uids;

    IItem * it;

    for (int row = 0; row < count; ++row) {
        it = children.at(position);
        if (it -> isRemote()) {
            QVariant item_uid = it -> toUid();
            if (item_uid.isValid())
                uids << item_uid;
        }
    }

    return uids;
}

void FolderItem::backPropagateItemsCountInBranch(int offset) {
    inBranchCount += offset;

    if (_parent)
        _parent -> backPropagateItemsCountInBranch(offset);
}

QVariant FolderItem::data(int column) const {
    switch(column) {
        case Qt::ToolTipRole:  return QVariant(title().toString() % QStringLiteral("(") % QString::number(inBranchCount) % QStringLiteral(" items)"));
        case IEXECCOUNTS:      return inBranchCount;
        default:               return IItem::data(column);
    }
}

bool FolderItem::removePhysicalObject() { // this is a little dangerous (
//    QDir delDir(fullPath());
//    if (delDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System).count() == 0)
//        return delDir.removeRecursively();
    return false;
}

QJsonObject FolderItem::toJson() {
    QJsonObject root = IItem::toJson();

    if (children.length() > 0) {
        root[JSON_TYPE_CONTAINER_ITEMS_COUNT] = inBranchCount;

        QJsonArray ar = QJsonArray();
        QList<IItem *>::Iterator it = children.begin();

        for( ;it != children.end(); it++)
            ar.append((*it) -> toJson());

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
        curr = new FolderItem(name, this, pos);

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
////    foreach(IItem * childItem, childItems) {
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
//        IItem * item = new IItem(data, this);
//        childItems.insert(position, item);
//    }

//    return true;
//}

int FolderItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > children.size())
        return 1;

    IItem * it;
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
    for(QHash<QString, FolderItem *>::Iterator it = folders.begin(); it!= folders.end(); it++)
        it.value() -> propagateFolderSetFlag(flag);
}
void FolderItem::propagateFolderUnsetFlag(ItemStateFlag flag) {
    unset(flag);
    for(QHash<QString, FolderItem *>::Iterator it = folders.begin(); it!= folders.end(); it++)
        it.value() -> propagateFolderUnsetFlag(flag);
}

void FolderItem::updateCheckedState(bool checked) {
    IItem::updateCheckedState(checked);

    for(QList<IItem *>::Iterator it = children.begin(); it!= children.end(); it++)
        (*it) -> updateCheckedState(checked);
}

void FolderItem::updateCheckedStateByPredicate(ItemStateFlag pred_state) { // not check folders without checked items
    if (is(checked)) {
        for(QList<IItem *>::Iterator it = children.begin(); it!= children.end(); it++)
            (*it) -> updateCheckedStateByPredicate(pred_state);
    }
}

void FolderItem::shuffle() {
    qsrand((uint)QTime::currentTime().msec());
    int n = children.count() - 1;

    for (int i = 0; i < n; ++i)
        children.swap(i, qrand() % n);/*((n + 1) - i) + i)*/;

    for(QHash<QString, FolderItem *>::Iterator it = folders.begin(); it!= folders.end(); it++)
        it.value() -> shuffle();
}

void FolderItem::packToStream(QHash<QUrl, int> & urls, QDataStream & stream) {
    QList<IItem *>::Iterator child = children.begin();

    for(; child != children.end(); child++)
        (*child) -> packToStream(urls, stream);
}
