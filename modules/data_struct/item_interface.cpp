#include "item_interface.h"
#include "folder_item.h"
#include "external_keys.h"

using namespace Playo3;

IItem::IItem(FolderItem * parent, int initState)
    : ItemFields(initState), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(FolderItem * parent, QVariantMap & hash, int pos)
    : ItemFields(hash), _parent(parent) {

    if (_parent) {
        if (pos < 0)
            _parent -> declareChild(this);
        else
            _parent -> declareChild(pos, this);
    }
}
IItem::IItem(FolderItem * parent, QJsonObject * hash)
    : ItemFields(hash), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(FolderItem * parent, QString title, int pos, int initState)
    : ItemFields(title, initState), _parent(parent) {

    if (_parent) {
        if (pos < 0)
            _parent -> declareChild(this);
        else
            _parent -> declareChild(pos, this);
    }
}

QJsonObject IItem::toJson() {
    QJsonObject root = ItemFields::toJson();

    root[JSON_TYPE_ITEM_TYPE] = itemType();

    return root;
}

QString IItem::fullPath() const {
    FolderItem * curr = _parent;

    QString path_buff;

    if (path().isValid()) {
        path_buff = path().toString() % '/' % title().toString();
    } else {
        path_buff = title().toString();

        while(curr != 0 && curr -> title().isValid()) {
            path_buff = curr -> title().toString() % '/' % path_buff;
            curr = curr -> _parent;
        }
    }

#ifdef Q_OS_LINUX
    path_buff = '/' % path_buff;
#endif

    if (extension().isValid())
        path_buff += '.' % extension().toString();

    return path_buff;
}

void IItem::openLocation() {
    QFileInfo info(fullPath());
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
}

int IItem::row() const {
    if (_parent)
        return _parent -> childRow(const_cast<IItem *>(this));

    return 0;
}

//bool IItem::insertColumns(int position, int columns) {
//    if (position < 0 || position > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.insert(position, QVariant());

//    foreach (IItem * child, childItems)
//        child -> insertColumns(position, columns);

//    return true;
//}

//bool IItem::removeColumns(int position, int columns) {
//    if (position < 0 || position + columns > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.remove(position);

//    foreach (IItem * child, childItems)
//        child -> removeColumns(position, columns);

//    return true;
//}

QString IItem::buildTreePath() const {
    return _parent ? _parent -> buildTreePath() % QStringLiteral(" ") % QString::number(row()) : QString();
}

QString IItem::buildTreeStr() const {
    int rNum = row();
    return _parent ? _parent -> buildTreeStr() + (rNum / 255) + (rNum % 255) : QString();
}

QVariant IItem::data(int column) const {
    switch(column) {
        case IATTRS: {
            QVariantMap params;
            params.insert(Key::name, title());
            params.insert(Key::checkable, Settings::instance() -> isCheckboxShow() ?  is(checked) : QVariant());
            if (!isContainer()) {
                if (Settings::instance() -> isShowSystemIcons())
                    params.insert(Key::icon, IconProvider::fileIcon(fullPath(), extension().toString()));
                params.insert(Key::info, info());
                params.insert(Key::ext, extension());
                params.insert(Key::state, visualStates());
                params.insert(Key::played, is(played));
                params.insert(Key::not_exist, is(not_exist));


                int iType = itemType();
                if (iType == WEB_ITEM && subtipe() == fourshared) iType = FOURSHARED_ITEM;
                params.insert(Key::type, iType);
            }
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
//        case IINNERCOPYURL:    return isRemote() ? QVariant() : toUrl();
        case IFOLDER:          return isContainer();
        case Qt::FontRole:     return Settings::instance() -> itemFont();
        case ITREEPATH:        return buildTreePath();
        case ITREESTR:         return buildTreeStr();
//        case IUID:             return uid();

//        case IADDFONT:         return Settings::instance() -> getItemInfoFont();

        case Qt::SizeHintRole: { // scrollTo work wrongly with different item heights
                if (!Settings::instance() -> isHeightUnificate() && isContainer())
                    return QSize(0, Settings::instance() -> itemHeight() + 6);
                else
                    return QSize(0, Settings::instance() -> totalItemHeight() + 6);
        }
        case Qt::TextAlignmentRole:
            if (isContainer() || !Settings::instance() -> isShowInfo())
                return Qt::AlignVCenter;
            else
                return Qt::AlignLeft;

//        case IINFO:            return info();
        case Qt::CheckStateRole: {
            if (Settings::instance() -> isCheckboxShow()) {
                return is(checked);
            } else return QVariant();
        }

        case Qt::ToolTipRole:
            return title()/* + "(" + _extension + ")" + "\n" + _path*/;

        case IEXTENSION:        return extension();
//        case PATHID:            return path();
        case IREMOTE:           return isRemote();
        case ISTATE:            return visualStates();
        case IFULLPATH:         return fullPath();
        case ITYPE:             return itemType();

        default:                return QVariant();
    }
}

bool IItem::setData(int column, const QVariant &value) {
    switch(column) {
        case ITITLE:       { setTitle(value); break; }
        case IEXTENSION:   { setExtension(value); break; }
        case IPATH:        { setPath(value); break; }
        case ISTATE:       { setStates(value.toInt()); break;}
        default:           { return false; }
    }

    return true;
}

void IItem::packToStream(QHash<QUrl, int> & urls, QDataStream & stream) {
    QUrl lastUrl = toUrl(); // maybe needed update for some services // need to retreive download link

    if (urls.contains(lastUrl)) return;

    stream << lastUrl << isRemote() << toInnerAttrs(itemType());
    urls.insert(lastUrl, 0);
}
