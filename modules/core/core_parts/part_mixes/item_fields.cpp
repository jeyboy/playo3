#include "item_fields.h"
#include "modules/core/web/web_apis.h"

#include <qdesktopservices.h>
#include <qfile.h>

using namespace Core;

ItemFields::ItemFields(const QVariantHash & hash) : ItemState(const_cast<QVariantHash &>(hash).take(JSON_TYPE_STATE).toInt()) {
    attrs = QVariantHash(hash);
}

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> take(JSON_TYPE_STATE).toInt()) {
    attrs = hash -> toVariantHash();
}

ItemFields::ItemFields(const DataSubType & subType, int state) : ItemState(state) {
    attrs[JSON_TYPE_ITEM_TYPE] = subType;
}

QVariant ItemFields::path() const {
    QVariant res = attrs.value(JSON_TYPE_PATH);
    return res;

//    switch(res.type()) {
//        case QVariant::Map: {
//            QVariantMap map = res.toMap();

//            switch(dataType()) {
//                case dt_site_vk: {
//                    if (map.size() == 1 && map.contains(QLatin1String("external"))) {
//                        // do some actions with embeded link
//                    } else { // get best quality
//                        return map.values().last();
//                    }
//                break;}
//                default: map.values().last();
//            }
//        }
//        default: res;
//    }
}

QStringList ItemFields::info() const {
    QStringList list;
    QVariant i = _info();

    if (!i.isValid())
        list.append(QLatin1String("Wait on proc..."));
    else
        list.append(i.toString());
    list.append(duration().toString());

    return list;
}

void ItemFields::openLocation() {
    if (!isRemote()) {
        QString op = path().toString().section('/', 0, -2);
        QDesktopServices::openUrl(QUrl::fromLocalFile(op));
    }
}
//void ItemFields::openLocation() {
//    QFileInfo info(fullPath());
//    QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
//}
bool ItemFields::removePhysicalObject() {
    switch(dataType()) {
        case dt_local_cue: {// this required on some additional checks
            if (!attrs[JSON_TYPE_CUE_MAP].toBool())
                return false;
        }
        case dt_playlist_cue: // removing of cue file
        case dt_local: {
            QString fpath = fullPath();
            bool res = QFile::setPermissions(fpath, QFile::ReadOther | QFile::WriteOther);
            qDebug() << "CHANGE PERM" << fpath << res;
            res = QFile::remove(fpath);
            qDebug() << "REMOVING" << fpath << res;
            return res;
        }
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
    ISource * source = Web::Apis::source(dataType());
    return source && source -> isShareable();
}

bool ItemFields::isRemote() const { return DST_IS_WEB(dataType()); }

//QString ItemFields::toUid() {
//    switch(dataType()) {
//        case dt_web_vk:
////            case dt_playlist_vk:
//            return toUid(ownerId(), id());
//        default: return id().toString();
//    }
//}

QString ItemFields::fullPath() const {
    return
        #ifdef Q_OS_LINUX
            '/' %
        #endif
            path().toString();
}

QUrl ItemFields::toUrl() const {
    if (isRemote()) {
        QUrl url = QUrl(path().toString());
        switch(dataType()) {
            case dt_web_sc: {
                if (url.query().isEmpty()) // links with hashed policy should not be attachable for additional fields - this action broke policy
                    url.setQuery(Web::Soundcloud::Queries::obj().genDefaultParams());
            break;}
            default:;
        }
        return url;
    } else
        return QUrl::fromLocalFile(fullPath());
}

QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject::fromVariantHash(attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantHash ItemFields::toInnerAttrs() const {
    QVariantHash a(attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
    return a;
}
