#include "item_interface.h"

using namespace Playo3;

ItemInterface::ItemInterface(const QVector<QVariant> & data, ItemInterface * parent)
    : ItemFields(), parentItem(parent) {

    itemData = data;
}

ItemInterface::ItemInterface(ItemInterface * parent, int initState = DEFAULT_MODEL_ITEM_STATE)
    : ItemFields(initState), parentItem(parent) {
}
ItemInterface::ItemInterface(ItemInterface * parent, QJsonObject * hash)
    : ItemFields(hash), parentItem(parent) {
}
ItemInterface::ItemInterface(ItemInterface * parent, QString path, QString title, QString extension, int size)
    : ItemFields(path, title, extension, size), parentItem(parent) {
}

ItemInterface::~ItemInterface() {
    qDeleteAll(childItems);
}

int ItemInterface::row() const {
    if (parentItem)
        return parentItem -> childItems.indexOf(const_cast<ItemInterface *>(this));

    return 0;
}

int ItemInterface::column() const { // ?
    return 0;
}

bool ItemInterface::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        ItemInterface * item = new ItemInterface(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool ItemInterface::insertColumns(int position, int columns) {
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (ItemInterface * child, childItems)
        child -> insertColumns(position, columns);

    return true;
}

bool ItemInterface::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool ItemInterface::removeColumns(int position, int columns) {
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (ItemInterface * child, childItems)
        child -> removeColumns(position, columns);

    return true;
}

bool ItemInterface::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}


///////////////////////////////////////////////////////////

//    ModelItem::ModelItem(int initState) {
//        parentItem = 0;
//        titlesCache = 0;
//        state = new ModelItemState(initState);
//        path = QString();
//        title = QString("--(O_o)--");
//        extension = QString();
//        genreID = -1;
//        duration = -1;
//        size = -1;
//        info = -1;
//        bpm = 0;
//    }

//    ModelItem::ModelItem(QJsonObject * hash, ModelItem * parent) {
//        parentItem = parent;
//        titlesCache = 0;
//        state = new ModelItemState(hash -> value("s").toInt());
//        path = hash -> value("p").toString();
//        title = hash -> value("t").toString();
//        extension = hash -> value("e").toString();
//        genreID = hash -> value("g").toInt(-1);
//        duration = hash -> value("d").toString("");

//        size = hash -> value("b").toInt(-1);
//        info = hash -> value("a").toString("");
//        bpm = hash -> value("m").toInt(0);

//        if (parent != 0) {
//           parent -> appendChild(this);
//        }
//    }

//    ModelItem::ModelItem(const QString filePath, QString fileName, ModelItem * parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState) {
//        parentItem = parent;
//        titlesCache = 0;
//        size = itemSize;
//        info = itemInfo;
//        state = new ModelItemState(initState);

//        title = fileName.replace("\n", " ");
//        path = filePath;
//        genreID = genre_id;
//        bpm = 0;
//        duration = itemDuration;
//    //    extension = fileExtension;

//        if (parent != 0) {
//           parent -> appendChild(this);
//        }
//    }

//    ModelItem::~ModelItem() {
//        delete state;
//        delete titlesCache;
//    }

//    QString ModelItem::fullPath() const {
//    //    if (parentItem != 0) {
//    //        return parentItem -> fullPath() + '/' + path;
//    //    }

//    //    return "file://";


//        ModelItem * curr = parentItem;
//        QString path_buff = path;

//        while(curr != 0 && !curr -> path.isEmpty()) {
//            path_buff = curr -> path + '/' + path_buff;
//            curr = curr -> parentItem;
//        }

//    #ifdef Q_OS_LINUX
//        path_buff = '/' + path_buff;
//    //#elif defined(Q_OS_WINDOWS)

//    #endif

//        return path_buff;
//    //    return path_buff.mid(1);
//    }

//    QString ModelItem::getDownloadTitle() const {
//        QString ret = filenameFilter(getTitle());

//        if (!extension.isEmpty())
//            ret = ret + '.' + extension;

//        return ret;
//    }

//    void ModelItem::openLocation() {
//        if (!parent() -> fullPath().isEmpty())
//            QDesktopServices::openUrl(parent() -> toUrl());
//        else {
//            QFileInfo info(fullPath());
//            QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
//        }
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

//    QUrl ModelItem::toUrl() {
//        return QUrl::fromLocalFile(fullPath());
//    }

//    ModelItem * ModelItem::toModelItem() {
//        return dynamic_cast<ModelItem *>(this);
//    }


//    ModelItem * ModelItem::parent() {
//        return parentItem;
//    }

//    ModelItem *ModelItem::child(int) { return 0; }

//    int ModelItem::childTreeCount() const { return 0; }

//    int ModelItem::childCount() const { return 0; }

//    void ModelItem::insertChild(int, ModelItem *) { }

//    void ModelItem::appendChild(ModelItem *) {}

//    bool ModelItem::removeChildren(int, int) { return false; }


//    QVariant ModelItem::data(int column) const {
//        switch(column) {
//            case TITLEID: return getTitle();
//            case EXTENSIONID: return extension;
//            case PATHID: return path;
//            case FOLDERID: return isFolder();
//            case TITLESCACHEID: return QVariant(*getTitlesCache());
//            case STATEID: return getState() -> currStateValue();
//            default: return QVariant();
//        }
//    }

//    bool ModelItem::setData(int column, const QVariant &value) {
//        if (column < 0 || column >= 2)
//    //    if (column < 0 || column >= itemData.size())
//            return false;

//    //    itemData[column] = value;

//        switch(column) {
//            case TITLEID: { title = value.toString(); break; }
//            case EXTENSIONID: { extension = value.toString(); break; }
//            case PATHID: { path = value.toString(); break; }
//            case STATEID: { setState(value.toInt(), false); break;}
//        }

//        return true;
//    }

//    void ModelItem::setState(int new_state, bool append_to_library) {
////        if (state -> setBit(new_state) && append_to_library) {
////            if (state -> isListened())
////                Library::instance() -> addItem(this, STATE_LISTENED);
////            else if (state -> isLiked())
////                Library::instance() -> addItem(this, STATE_LIKED);
////        }
//    }


//    void ModelItem::dropExpandProceedFlags() {}


//    QHash<QString, ModelItem *> *ModelItem::foldersList() const { return 0; }
//    int ModelItem::removeFolder(QString) { return -1; }

//    void ModelItem::changeCheckedState(bool checked) {
//        if (checked)
//            getState() -> setChecked();
//        else
//            getState() -> unsetChecked();
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

//    QList<ModelItem *> * ModelItem::childItemsList() { return 0;}

//    void ModelItem::shuffle() {
////        if (isFolder()) {
////            Rand::shuffle(childItemsList());

////            foreach(ModelItem * item, *childItemsList())
////                item -> shuffle();
////        }
//    }

//}
