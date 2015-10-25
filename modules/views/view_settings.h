#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/core/playlist_types.h"
#include <qjsonobject.h>

namespace Views {
    struct Params {
        Params(Data::Type cType, const QString & uniq_id = QString(), Data::RelType rel = Data::none_rel) : deleteFile(false), playlist(true),
            interactive(false), common(false), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(Data::Type cType = Data::tree, bool isCommon = false, bool delFile = false, bool isInteractive = false,
            bool isPlaylist = false, const QString & uniq_id = QString(), Data::RelType rel = Data::none_rel) : deleteFile(delFile), playlist(isPlaylist),
            interactive(isInteractive), common(isCommon), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(const QJsonObject & obj) {
            deleteFile = obj[QStringLiteral("del")].toBool();
            playlist = obj[QStringLiteral("play")].toBool();
            interactive = obj[QStringLiteral("int")].toBool();
            common = obj[QStringLiteral("common")].toBool();
            type = (Data::Type)obj[QStringLiteral("type")].toInt();
            uid = obj[QStringLiteral("uid")].toString();
            rel_type = (Data::RelType)obj[QStringLiteral("rel_type")].toInt();
        }

        QJsonObject toJson() {
            QJsonObject obj;

            obj[QStringLiteral("del")] = deleteFile;
            obj[QStringLiteral("play")] = playlist;
            obj[QStringLiteral("int")] = interactive;
            obj[QStringLiteral("common")] = common;
            obj[QStringLiteral("type")] = type;
            if (!uid.isEmpty())
                obj[QStringLiteral("uid")] = uid;

            obj[QStringLiteral("rel_type")] = rel_type;

            return obj;
        }

        bool deleteFile;
        bool playlist;
        bool interactive;
        bool common;
        QString uid;
        Data::RelType rel_type;

        Data::Type type;
    };
}

#endif // VIEW_SETTINGS
