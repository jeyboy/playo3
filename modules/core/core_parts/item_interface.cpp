#include "item_interface.h"
#include "playlist.h"
#include "external_keys.h"
#include <qdebug.h>

using namespace Core;

IItem::IItem(Playlist * parent, int initState)
    : ItemFields(initState), activeSourceIndexLimit(0), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, QVariantMap & hash, int pos)
    : ItemFields(hash), activeSourceIndexLimit(0), _parent(parent) {

    if (_parent) {
        if (pos < 0)
            _parent -> declareChild(this);
        else
            _parent -> declareChild(this, pos);
    }
}
IItem::IItem(Playlist * parent, QJsonObject * hash)
    : ItemFields(hash), activeSourceIndexLimit(0), _parent(parent) {

    if (_parent)
        _parent -> declareChild(this);
}
IItem::IItem(Playlist * parent, const QString & title, int pos, int initState)
    : ItemFields(title, initState), activeSourceIndexLimit(0), _parent(parent) {

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
            sourcesJson.append((*source) -> ItemFields::toJson());
        root[JSON_TYPE_SOURCES] = sourcesJson;
    }

//    root[JSON_TYPE_ITEM_TYPE] = itemType();

    return root;
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
            params.insert(Keys::checkable, Settings::obj().isCheckboxShow() ?  has(flag_checked) : QVariant());
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
                params.insert(Keys::played, has(flag_played));
                params.insert(Keys::shareable, isShareable());
                params.insert(Keys::sources_amount, activeSourceItem() -> sources.size());

                if (has(flag_proccessing))
                    params.insert(Keys::proccessing, true);
                else {
                    int err_code = error().toInt();
                    params.insert((isRemote() ? Keys::undefined : Keys::not_exist), err_code > 0 && err_code < 1000);
                    params.insert(Keys::unsupported, err_code > 999 && err_code < 2000);
                }
                params.insert(Keys::type, dataType());
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
                return has(flag_checked);
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

    stream << lastUrl << isRemote() << toInnerAttrs(dataType());
    urls.insert(lastUrl, 0);
}

void IItem::setParent(Playlist * pNode, int pos) {
    if (pNode == _parent) return;
    _parent -> undeclareChild(this);
    _parent = pNode;
    if (pNode)
        (pos == -1) ? _parent -> declareChild(this) : _parent -> declareChild(this, pos);
}

bool IItem::addSource(QJsonObject * hash) { return addSource(Playlist::restoreItem(0, *hash), false, false); }

bool IItem::addSource(IItem * newSource, bool setAsMain, bool checkExistance) {
    if (checkExistance) {
        newSource -> setParent(0);
        for(QList<IItem *>::Iterator source = sources.begin(); source != sources.end(); source++)
            if ((*source) -> eqlByLocation(newSource)) {
                qDebug() << "SOURCE ALREADY EXISTS";
                return false;
            }
    }

    sources.append(newSource);

    if (setAsMain) {
        setActiveSourceIndex(sources.length() - 1);
        connectToSource(activeSourceItem());
    }

    qDebug() << "ADD NEW SOURCE";

    return true;
}

bool IItem::useNextSource() {
    if (sources.length() == 1) return false;

    int currSourceIndex = activeSourceIndex() + 1;
    if (currSourceIndex >= sources.length())
        currSourceIndex = 0;

    setActiveSourceIndex(currSourceIndex);
    connectToSource(activeSourceItem());
    return currSourceIndex != activeSourceIndexLimit;
}

IItem * IItem::activeSourceItem() const {
    if (isContainer()) return const_cast<IItem *>(this);

    if (sources.isEmpty()) {
        IItem * self = const_cast<IItem *>(this);
        QJsonArray arr = const_cast<QVariantMap &>(attrs).take(JSON_TYPE_SOURCES).toJsonArray();
        if (arr.isEmpty()) // root node is always empty // so it shpuld be source for yourself
            self -> addSource(self, false, false);
        else
            for(QJsonArray::ConstIterator it = arr.constBegin(); it != arr.constEnd(); it++){
                QJsonObject obj = (*it).toObject();
                self -> addSource(&obj);
            }
    }

    int ind = activeSourceIndex();
    if (ind >= sources.length()) ind = 0;

    return sources[ind];
}

QString IItem::relationStr() const {
    switch(dataType()) {
        case dt_local: return QStringLiteral("(Local) ");
        case dt_playlist_local: return QStringLiteral("(Folder) ");
        case dt_playlist_cue: return QStringLiteral("(Folder Cueta) ");
        case dt_local_cue: return QStringLiteral("(Cueta) ");
        case dt_site_myzuka: return QStringLiteral("(Myzika) ");
        case dt_site_fourshared: return QStringLiteral("(4shared) ");
        case dt_site_zaycev: return QStringLiteral("(Zaycev) ");
        case dt_site_mp3base: return QStringLiteral("(Mp3base) ");
        case dt_site_promodj: return QStringLiteral("(PromoDj) ");
        case dt_site_mp3cc: return QStringLiteral("(Mp3cc) ");
        case dt_site_mp3pm: return QStringLiteral("(Mp3pm) ");
        case dt_site_shmidt: return QStringLiteral("(Shmidt) ");
        case dt_site_jetune: return QStringLiteral("(Jetune) ");
        case dt_site_music_shara: return QStringLiteral("(MShara) ");
        case dt_site_redmp3: return QStringLiteral("(Redmp3) ");
        case dt_site_yandex: return QStringLiteral("(Yandex) ");
        case dt_site_youtube: return QStringLiteral("(Youtube) ");
        case dt_site_vk: return QStringLiteral("(Vk) ");
        case dt_playlist_vk: return QStringLiteral("(Vk Folder) ");
        case dt_site_sc: return QStringLiteral("(Sc) ");
        case dt_playlist_sc: return QStringLiteral("(Sc Folder) ");
        case dt_site_od: return QStringLiteral("(Od) ");
        case dt_playlist_od: return QStringLiteral("(Od Folder) ");
        case dt_site_echonest: return QStringLiteral("(Echonest) ");
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
