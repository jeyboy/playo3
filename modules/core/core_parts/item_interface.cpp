#include "item_interface.h"
#include "playlist.h"
#include "external_keys.h"
#include <qdebug.h>

using namespace Core;

IItem::IItem(Playlist * parent, int initState)
    : ItemFields(initState), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, QVariantMap & hash, int pos)
    : ItemFields(hash), _parent(parent) {

    if (_parent) {
        if (pos < 0)
            _parent -> declareChild(this);
        else
            _parent -> declareChild(pos, this);
    }
}
IItem::IItem(Playlist * parent, QJsonObject * hash)
    : ItemFields(hash), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, QString title, int pos, int initState)
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
    Playlist * curr = _parent;

    QString path_buff = path().toString();

    if (!path_buff.contains('/')) {
        while(curr != 0 && curr -> title().isValid()) {
            path_buff = curr -> title().toString() % '/' % path_buff;
            curr = curr -> _parent;
        }
    }

#ifdef Q_OS_LINUX
    path_buff = '/' % path_buff;
#endif

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
            params.insert(Keys::name, title());
            params.insert(Keys::checkable, Settings::obj().isCheckboxShow() ?  is(checked) : QVariant());
            if (!isContainer()) {
                if (Settings::obj().isShowSystemIcons())
                    params.insert(Keys::icon, IconProvider::fileIcon(fullPath(), extension().toString()));
                params.insert(Keys::info, info());
                params.insert(Keys::ext, extension());
                params.insert(Keys::state, visualStates());
                params.insert(Keys::played, is(played));
                params.insert(Keys::shareable, isShareable());

                if (is(proccessing))
                    params.insert(Keys::proccessing, true);
                else
                    params.insert((isRemote() ? Keys::undefined : Keys::not_exist), is(not_exist));
                params.insert(
                    Keys::type,
                    subtipe() > 0 ? (itemType() + subtipe()) : itemType()
                );
            }
            return params;
        }

        case IPLAYABLE:        return isPlayable();
        case IURL:             return toUrl();
        case IFOLDER:          return isContainer();
        case Qt::FontRole:     return Settings::obj().itemFont();
        case ITREEPATH:        return buildTreePath();
        case ITREESTR:         return buildTreeStr();
        case Qt::DisplayRole:  return title();
//        case IUID:             return uid();

//        case IADDFONT:         return Settings::obj().getItemInfoFont();

        case Qt::SizeHintRole: { // scrollTo work wrongly with different item heights
                if (!Settings::obj().isHeightUnificate() && isContainer())
                    return QSize(0, Settings::obj().itemHeight() + 6);
                else
                    return QSize(0, Settings::obj().totalItemHeight() + 6);
        }
        case Qt::TextAlignmentRole:
            if (isContainer() || !Settings::obj().isShowInfo())
                return Qt::AlignVCenter;
            else
                return Qt::AlignLeft;

        case Qt::CheckStateRole: {
            if (Settings::obj().isCheckboxShow()) {
                return is(checked);
            } else return QVariant();
        }

        case Qt::ToolTipRole:
            return relationStr() + '\n' + title().toString()/* + "(" + _extension + ")" + "\n" + _path*/;

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

void IItem::removeYouself() { if (_parent) _parent -> removeChildren(row(), 1); }

void IItem::packToStream(QHash<QUrl, int> & urls, QDataStream & stream) {
    QUrl lastUrl = toUrl(); // maybe needed update for some services // need to retreive download link

    if (!lastUrl.isEmpty() && urls.contains(lastUrl)) return;

    stream << lastUrl << isRemote() << toInnerAttrs(itemType());
    urls.insert(lastUrl, 0);
}

void IItem::setParent(Playlist * pNode, int pos) {
    if (pNode == _parent) return;
    _parent -> undeclareChild(this);
    _parent = pNode;
    (pos == -1) ? _parent -> declareChild(this) : _parent -> declareChild(pos, this);
}
