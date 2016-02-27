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
            _parent -> declareChild(this, pos);
    }
}
IItem::IItem(Playlist * parent, QJsonObject * hash)
    : ItemFields(hash), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, const QString & title, int pos, int initState)
    : ItemFields(title, initState), _parent(parent) {

    if (_parent) {
        if (pos < 0)
            _parent -> declareChild(this);
        else
            _parent -> declareChild(this, pos);
    }
}

QJsonObject IItem::toJson() {
    QJsonObject root = ItemFields::toJson();

    if (!sources.isEmpty()) {
        QJsonArray sourcesJson;
        for(QList<IItem *>::Iterator source = sources.begin(); source != sources.end(); source++)
            sourcesJson.append(((ItemFields*)(*source)) -> toJson());
        root[JSON_TYPE_SOURCES] = sourcesJson;
    }

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

                QString err_key;
                QString err_msg = errorStr(err_key);

                if (err_key.isEmpty())
                    params.insert(Keys::info, info());
                else
                    params.insert(err_key, err_msg);

                params.insert(Keys::ext, extension());
                params.insert(Keys::state, visualStates());
                params.insert(Keys::played, is(played));
                params.insert(Keys::shareable, isShareable());

                if (is(proccessing))
                    params.insert(Keys::proccessing, true);
                else {
                    int err_code = error().toInt();
                    params.insert((isRemote() ? Keys::undefined : Keys::not_exist), err_code > 0 && err_code < 1000);
                    params.insert(Keys::unsupported, err_code > 999 && err_code < 2000);
                }
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
            if (Settings::obj().isCheckboxShow())
                return is(checked);
            else return QVariant();
        }

        case Qt::ToolTipRole: {
            QString err_key;
            QString err_msg = errorStr(err_key);
            return
                relationStr() +
                (infoVar().isValid() ? ('\n' + infoVar().toString()) : "") +
                '\n' + title().toString() +
                (error().isValid() ? ("\n " + err_key + ": " + err_msg) : "") +
                (path().isValid() ? ('\n' + path().toString()) : "");
        }

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
    if (pNode)
        (pos == -1) ? _parent -> declareChild(this) : _parent -> declareChild(this, pos);
}

void IItem::addSource(QJsonObject * hash) { addSource(Playlist::restoreItem(0, *hash)); }

void IItem::addSource(IItem * newSource, bool setAsMain) {
    sources.append(newSource);

    if (setAsMain)
        setActiveSource(sources.length() - 1);
}

//bool IItem::hasSource(const QString & url, const QString & refresh_token) {
//    // update me :(
//}

//void IItem::removeSource(const QString & url) {
//    // update me :(
//}

//void IItem::removeSource(int index) {
//    // update me :(
//}

QString IItem::relationStr() const {
    switch(itemType()) {
        case SIMPLE_FILE: return QStringLiteral("(Local) ");
        case PLAYLIST: return QStringLiteral("(Folder) ");
        case CUE_FILE: return QStringLiteral("(Cueta) ");
        case WEB_FILE: {
            switch(subtipe()) {
                case Web::site_myzuka: return QStringLiteral("(Myzika) ");
                case Web::site_fourshared: return QStringLiteral("(4shared) ");
                case Web::site_zaycev: return QStringLiteral("(Zaycev) ");
                case Web::site_mp3base: return QStringLiteral("(Mp3base) ");
                case Web::site_promodj: return QStringLiteral("(PromoDj) ");
                case Web::site_mp3cc: return QStringLiteral("(Mp3cc) ");
                case Web::site_mp3pm: return QStringLiteral("(Mp3pm) ");
                case Web::site_shmidt: return QStringLiteral("(Shmidt) ");
                case Web::site_jetune: return QStringLiteral("(Jetune) ");
                case Web::site_music_shara: return QStringLiteral("(MShara) ");
                case Web::site_redmp3: return QStringLiteral("(Redmp3) ");
                case Web::site_yandex: return QStringLiteral("(Yandex) ");
                case Web::site_youtube: return QStringLiteral("(Youtube) ");
                default: return QStringLiteral("(Unknow subtype) ");
            }
        break;}
        case VK_FILE: return QStringLiteral("(Vk) ");
        case VK_PLAYLIST: return QStringLiteral("(Vk Folder) ");
        case SOUNDCLOUD_FILE: return QStringLiteral("(Sc) ");
        case SOUNDCLOUD_PLAYLIST: return QStringLiteral("(Sc Folder) ");
        case OD_FILE: return QStringLiteral("(Od) ");
        case OD_PLAYLIST: return QStringLiteral("(Od Folder) ");
        default: return QStringLiteral("(Unknow) ");
    }
}

QString IItem::errorStr(QString & key_name) const {
    switch(error().toInt()) {
        case err_not_existed: {
            key_name = Keys::error;
            return QStringLiteral("Not existed");
        break;}

        case err_not_finded: {
            key_name = Keys::error;
            return QStringLiteral("Noting founded");
        break;}

        case warn_not_supported: {
            key_name = Keys::warning;
            return QStringLiteral("Not supported");
        break;}
        case warn_not_accessable: {
            key_name = Keys::warning;
            return QStringLiteral("Not accessable at this time");
        break;}
        case warn_not_permitted: {
            key_name = Keys::warning;
            return QStringLiteral("Playing of this item is not permitted at this moment");
        break;}

        default: return QString();
    }
}
