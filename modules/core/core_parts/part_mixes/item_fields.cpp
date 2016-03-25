#include "item_fields.h"
#include "modules/core/web/apis/social/soundcloud_api.h"

#include <qdesktopservices.h>

using namespace Core;

ItemFields::ItemFields(QVariantMap & hash) : ItemState(hash.take(JSON_TYPE_STATE).toInt()) {
    attrs = QVariantMap(hash);
}

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> take(JSON_TYPE_STATE).toInt()) {
    attrs = hash -> toVariantMap();
}

ItemFields::ItemFields(const DataSubType & subType, const QString & title, int initState) : ItemState(initState) {
    attrs = {{JSON_TYPE_TITLE, title}, {JSON_TYPE_ITEM_TYPE, subType}};
}

ItemFields::ItemFields(const DataSubType & subType, int state) : ItemState(state) {
    attrs[JSON_TYPE_ITEM_TYPE] = subType;
}

QStringList ItemFields::info() const {
    QStringList list;
    QVariant i = _info();

    if (!i.isValid())
        list.append(QStringLiteral("Wait on proc..."));
    else
        list.append(i.toString());
    list.append(duration().toString());

    return list;
}

void ItemFields::openLocation() { QDesktopServices::openUrl(toUrl()); }
//void ItemFields::openLocation() {
//    QFileInfo info(fullPath());
//    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
//}
bool ItemFields::removePhysicalObject() {
    switch(dataType()) {
        case dt_local:
            return QFile::remove(fullPath());
        case dt_local_cue: // this required on some additional checks
        case dt_playlist_local:
            //    QDir delDir(fullPath());
            //    if (delDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System).count() == 0)
            //        return delDir.removeRecursively();
                return false;
        default: return false;
    }
}
bool ItemFields::isExist() const {
    switch(dataType()) {
        case dt_local:
        case dt_local_cue: return QFile::exists(fullPath());
        case dt_playlist_local: return QDir(fullPath()).exists();
        default: return true;
    }
}

bool ItemFields::isShareable() const                         {
//            return attrs.value(JSON_TYPE_IS_SHAREABLE, false).toBool();
    switch(dataType()) {
        case dt_site_od:
        case dt_site_vk:
        case dt_site_sc:
            return true;
        default: return false;
    }
}

bool ItemFields::isRemote() const                            {
//            return attrs.value(JSON_TYPE_IS_REMOTE, false).toBool();
    switch(dataType()) {
        case dt_local:
        case dt_local_cue:
        case dt_playlist_local:
        case dt_playlist_cue:
            return false;
        default: return true;
    }
}

QString ItemFields::toUid() {
    switch(dataType()) {
        case dt_site_vk:
//            case dt_playlist_vk:
            return toUid(owner(), id());
        case dt_site_sc:
        case dt_site_od:
            return id().toString();

        default: return QString();
    }
}

QString ItemFields::fullPath() const {
    QString path_buff = path().toString();
    #ifdef Q_OS_LINUX
        path_buff = '/' % path_buff;
    #endif
    return path_buff;
}

QUrl ItemFields::toUrl() const {
    if (isRemote()) {
        QUrl url = QUrl(path().toString());
        switch(dataType()) {
            case dt_site_sc: { url.setQuery(Web::Soundcloud::Api::obj().genDefaultParams()); break;}
            default: ;
        }
        return url;
    } else
        return QUrl::fromLocalFile(fullPath());
}

QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject::fromVariantMap(attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantMap ItemFields::toInnerAttrs(int itemType) const {
    QVariantMap a(attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
    a.insert(JSON_TYPE_ITEM_TYPE, itemType);
    return a;
}
