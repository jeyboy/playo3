#include "item_interface.h"
#include "folder_item.h"

using namespace Playo3;

ItemInterface::ItemInterface(FolderItem * parent, int initState)
    : ItemFields(initState), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
ItemInterface::ItemInterface(FolderItem * parent, QJsonObject * hash)
    : ItemFields(hash), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
ItemInterface::ItemInterface(FolderItem * parent, QString title, int initState)
    : ItemFields(title, initState), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}

ItemInterface::~ItemInterface() {

}

QString ItemInterface::fullPath() const {
    FolderItem * curr = _parent;

    QString path_buff;

    if (path().isValid()) {
        path_buff = path().toString() + '/' + title().toString();
    } else {
        path_buff = title().toString();

        while(curr != 0 && curr -> title().isValid()) {
            path_buff = curr -> title().toString() + '/' + path_buff;
            curr = curr -> _parent;
        }
    }

#ifdef Q_OS_LINUX
    path_buff = '/' + path_buff;
#endif

    if (extension().isValid())
        path_buff += '.' + extension().toString();

    return path_buff;
}

void ItemInterface::openLocation() {
    QFileInfo info(fullPath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
}

int ItemInterface::row() const {
    if (_parent)
        return _parent -> childRow(const_cast<ItemInterface *>(this));

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
    return _parent ? _parent -> buildTreePath() + " " + QString::number(row()) : "";
}

QVariant ItemInterface::data(int column) const {
    switch(column) {
        case IATTRS: {
            QVariantMap params;
            params.insert("info", info());
            params.insert("ext", extension());
            params.insert("state", visualStates());
            return params;
        }
        case Qt::DisplayRole:   return title();
        case Qt::DecorationRole: {
           if (is(not_exist))
               return IconProvider::missedIcon();

           if (isContainer())
               return QVariant(); // IconProvider::fileIcon("", "");
           else
               return IconProvider::fileIcon(fullPath(), extension().toString());
        }

        case IPLAYABLE:        return isPlayable();
        case IURL:             return toUrl();
        case Qt::FontRole:      return Settings::instance() -> getItemFont();
//        case IADDFONT:         return Settings::instance() -> getItemInfoFont();

        case Qt::SizeHintRole:
            if (isContainer())
                return QSize(0, Settings::instance() -> getItemHeight());
            else
                return QSize(0, Settings::instance() -> getTotalItemHeight());
        case Qt::TextAlignmentRole:
            if (isContainer() || !Settings::instance() -> isShowInfo())
                return Qt::AlignVCenter;
            else
                return Qt::AlignLeft;

        case IINFO:            return info();
        case Qt::CheckStateRole: {
            if (Settings::instance() -> isCheckboxShow()) {
                return is(checked);
            } else return QVariant();
        }

        case Qt::ToolTipRole:
            return title()/* + "(" + _extension + ")" + "\n" + _path*/;

//        case EXTENSIONID:       return extension();
//        case PATHID:            return path();
//        case FOLDERID:          return isContainer();
        case IREMOTE:          return isRemote();
//        case TITLESCACHEID:     return QVariant(*getTitlesCache());
        case ISTATE:           return visualStates();
        case IPROGRESS:        return -1;//Download::instance() -> getProgress(item);
        case IFULLPATH:        return fullPath();

        default:                return QVariant();
    }
}

bool ItemInterface::setData(int column, const QVariant &value) {
    switch(column) {
        case ITITLE:       { setTitle(value); break; }
        case IEXTENSION:   { setExtension(value); break; }
        case IPATH:        { setPath(value); break; }
        case ISTATE:       { setStates(value.toInt()); break;}
        default:           { return false; }
    }

    return true;
}

///////////////////////////////////////////////////////////
//    ModelItem::~ModelItem() {
//        delete state;
//        delete titlesCache;
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
