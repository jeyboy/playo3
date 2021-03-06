#include "item.h"
#include "playlist.h"
#include "external_keys.h"
#include "modules/core/web/web_apis.h"

using namespace Core;

IItem::IItem(Playlist * parent, QJsonObject * hash) : ItemFields(hash), _parent(parent) {
    if (_parent) _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, const QVariantHash & hash, int pos) : ItemFields(hash), _parent(parent) {
    if (_parent) _parent -> declareChild(this, pos);
}
IItem::IItem(const DataSubType & subType, Playlist * parent, int initState) : ItemFields(subType, initState), _parent(parent) {
    if (_parent) _parent -> declareChild(this);
}

IItem::~IItem() {
    if (_parent && dataType() == dt_local_cue) {
        QVariantMap cue_map = _parent -> cueMap();

        if (!cue_map.isEmpty()) {
            QString fpath = fullPath();

            int counter = cue_map[fpath].toInt();
            setCueMap(--counter <= 0);
            cue_map[fpath] = counter;
            _parent -> setCueMap(cue_map);
        }
    }

    if (is(flag_mark_on_removing))
        removePhysicalObject();

    _parent = 0;
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
    return _parent ? _parent -> buildTreePath() % QLatin1String(" ") % QString::number(row()) : QString();
}

QString IItem::buildTreeStr() const {
    int rNum = row();
    return _parent ? _parent -> buildTreeStr() + (rNum / 255) + (rNum % 255) : QLatin1String();
}

QVariant IItem::data(int column) const {
    switch(column) {
        case IATTRS: {
            QVariantHash params;
            params.insert(Keys::name, title());
            params.insert(Keys::checkable, Settings::obj().isCheckboxShow() ?  is(flag_checked) : QVariant());
            if (!isContainer()) {
                if (Settings::obj().isShowSystemIcons())
                    params.insert(Keys::icon, IconProvider::fileIcon(fullPath(), extension().toString()));

                QString err_key;
                QString err_msg = errorStr(err_key);

                if (err_key.isEmpty())
                    params.insert(Keys::info, info());
                else
                    params.insert(err_key, err_msg);

                params.insert(Keys::ext, extension());
                params.insert(Keys::state, visualStates());
                params.insert(Keys::played, is(flag_played));
                params.insert(Keys::shareable, isShareable());

                if (is(flag_proccessing))
                    params.insert(Keys::proccessing, true);
                else {
                    int err_code = error().toInt();
                    params.insert((isRemote() ? Keys::undefined : Keys::not_exist), err_code > 0 && err_code < 1000);
                    params.insert(Keys::unsupported, err_code > 999 && err_code < 2000);
                }
                params.insert(Keys::type, dataType());
            } else {
                if (hasMoreItems())
                    params.insert(Keys::loadable, true);
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
            if (Settings::obj().isCheckboxShow())
                return is(flag_checked);
            else return QVariant();
        }

        case Qt::ToolTipRole: {
            QString err_key;
            QString err_msg = errorStr(err_key);
            return
                relationStr() +
                (_info().isValid() ? ('\n' + _info().toString()) : "") +
                '\n' + title().toString() +
                (error().isValid() ? ("\n " + err_key + ": " + err_msg) : "") +
                (!isRemote() && path().isValid() ? ('\n' + path().toString()) : "");
        }

        case IEXTENSION:        return extension();
//        case PATHID:            return path();
        case IREMOTE:           return isRemote();
        case ISTATE:            return visualStates();
        case IFULLPATH:         return fullPath();
        case ITYPE:             return dataType();

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

    stream << lastUrl << isRemote() << toInnerAttrs();
    urls.insert(lastUrl, 0);
}

void IItem::setParent(Playlist * pNode, int pos) {
    if (pNode == _parent) return;
    _parent -> undeclareChild(this);
    _parent = pNode;
    (pos == -1) ? _parent -> declareChild(this) : _parent -> declareChild(this, pos);
}

QString IItem::relationStr() const {
    DataSubType dst = dataType();

    switch(dst) {
        case dt_local:              return QLatin1String("(Local) ");
        case dt_playlist_local:     return QLatin1String("(Folder) ");
        case dt_playlist_cue:       return QLatin1String("(Folder Cueta) ");
        case dt_local_cue:          return QLatin1String("(Cueta) ");

        default: {
            if (dst & dt_web) {
                return QStringLiteral("(%1%2) ").arg(
                    Web::Apis::source_name(dst),
                    DST_IS_PLAYLIST(dst) ? ' ' % QLatin1String("Folder") : QString()
                );
            } else return QLatin1String("(Unknow) ");
        }
    }
}

QString IItem::errorStr(QString & key_name) const {
    switch(error().toInt()) {
        case err_not_existed: {
            key_name = Keys::error;
            return QLatin1String("Not exists");
        break;}
        case err_unknow: {
            key_name = Keys::unknow;
            return QLatin1String("Unknown error");
        break;}
        case warn_not_supported: {
            key_name = Keys::warning;
            return QLatin1String("Not supported");
        break;}
        case warn_not_accessable: {
            key_name = Keys::warning;
            return QLatin1String("Not accessable at this time");
        break;}
        case warn_not_permitted: {
            key_name = Keys::warning;
            return QLatin1String("Playing of this item is not permitted at this moment");
        break;}

        default: return QLatin1String();
    }
}
