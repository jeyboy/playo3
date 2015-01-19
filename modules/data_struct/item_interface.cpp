#include "item_interface.h"
#include "folder_item.h"

using namespace Playo3;

ItemInterface::ItemInterface(FolderItem * parent, int initState)
    : ItemFields(initState), parentItem(parent) {

    if (parentItem)
        parentItem -> declareChild(this);
}
ItemInterface::ItemInterface(FolderItem * parent, QJsonObject * hash)
    : ItemFields(hash), parentItem(parent) {

    if (parentItem)
        parentItem -> declareChild(this);
}
ItemInterface::ItemInterface(FolderItem * parent, QString path, QString title, QString extension, int size, int initState)
    : ItemFields(path, title, extension, size, initState), parentItem(parent) {

    if (parentItem)
        parentItem -> declareChild(this);
}

ItemInterface::~ItemInterface() {

}

QString ItemInterface::fullPath() const {
    FolderItem * curr = parentItem;
    QString path_buff = _path;

    while(curr != 0 && !curr -> _path.isEmpty()) {
        path_buff = curr -> _path + '/' + path_buff;
        curr = curr -> parentItem;
    }

#ifdef Q_OS_LINUX
    path_buff = '/' + path_buff;
#endif

    return path_buff;
}

void ItemInterface::openLocation() {// TODO: improve
    if (!parentItem -> fullPath().isEmpty())
        QDesktopServices::openUrl(parentItem -> toUrl());
    else {
        QFileInfo info(fullPath());
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
    }
}

int ItemInterface::row() const {
    if (parentItem)
        return parentItem -> childRow(const_cast<ItemInterface *>(this));

    return 0;
}

int ItemInterface::column() const { // ?
    return 0;
}

//bool ItemInterface::insertColumns(int position, int columns) {
//    if (position < 0 || position > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.insert(position, QVariant());

//    foreach (ItemInterface * child, childItems)
//        child -> insertColumns(position, columns);

//    return true;
//}

//bool ItemInterface::removeColumns(int position, int columns) {
//    if (position < 0 || position + columns > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.remove(position);

//    foreach (ItemInterface * child, childItems)
//        child -> removeColumns(position, columns);

//    return true;
//}

QString ItemInterface::buildTreePath() const {
    return parentItem ? parentItem -> buildTreePath() + " " + QString::number(row()) : "";
}

QVariant ItemInterface::data(int column) const {
    switch(column) {
        case Qt::DisplayRole:   return _title;
        case EXTENSIONID:       return _extension;
        case PATHID:            return _path;
        case FULLPATHID:        return fullPath();
        case FOLDERID:          return isContainer();
        case REMOTEID:          return isRemote();
//        case TITLESCACHEID:     return QVariant(*getTitlesCache());
//        case STATEID:           return itemStateVal();
        default:                return QVariant();
    }
}

bool ItemInterface::setData(int column, const QVariant &value) {
    switch(column) {
        case TITLEID:       { _title = value.toString(); break; }
        case EXTENSIONID:   { _extension = value.toString(); break; }
        case PATHID:        { _path = value.toString(); break; }
        case STATEID:       { /*setState(value.toInt(), false);*/ break;}
        default:            { return false; }
    }

    return true;
}

///////////////////////////////////////////////////////////
//    ModelItem::~ModelItem() {
//        delete state;
//        delete titlesCache;
//    }

//    QStringList ModelItem::getInfo() const {
//        QStringList list;
//        if (info.isEmpty())
//            list.append("Processing ...");
//        else
//            list.append(info);
//        list.append(duration);

//        return list;
//    }

//    void ModelItem::accumulateUids(QHash<ModelItem*, QString> & store) {
//        if (isFolder()) {
//            foreach(ModelItem * item, *childItemsList())
//                item -> accumulateUids(store);
//        } else {
//            QString currUid = toUID();
//            if (!currUid.isEmpty()) {
//                store.insert(this, currUid);
//            }
//        }
//    }

//    void ModelItem::setState(int new_state, bool append_to_library) {
////        if (state -> setBit(new_state) && append_to_library) {
////            if (state -> isListened())
////                Library::instance() -> addItem(this, STATE_LISTENED);
////            else if (state -> isLiked())
////                Library::instance() -> addItem(this, STATE_LIKED);
////        }
//    }


//    bool ModelItem::cacheIsPrepared() const {
//        return titlesCache != 0;
//    }

//    void ModelItem::setCache(QList<QString> * newCache) {
//        titlesCache = newCache;
//    }

//    void ModelItem::addToCache(QString title) {
//        if (!titlesCache -> contains(title))
//            titlesCache -> append(title);
//    }
//    QList<QString> *ModelItem::getTitlesCache() const {
//        return titlesCache;
//    }
