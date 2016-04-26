#ifndef MODEL_SETTINGS
#define MODEL_SETTINGS

#include "modules/core/data_sub_types.h"
#include <qjsonobject.h>

namespace Models {
    struct Params {
        Params(Core::DataSubType cType, const QString & uniq_id = QString(), Core::RelType rel = Core::rel_none) : deleteFile(false), playlist(true),
            interactive(false), common(false), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(Core::DataSubType cType = Core::dt_tree, bool isCommon = false, bool delFile = false, bool isInteractive = false,
            bool isPlaylist = false, const QString & uniq_id = QString(), Core::RelType rel = Core::rel_none) : deleteFile(delFile), playlist(isPlaylist),
            interactive(isInteractive), common(isCommon), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(const QJsonObject & obj) {
            deleteFile = obj[QStringLiteral("del")].toBool();
            playlist = obj[QStringLiteral("play")].toBool();
            interactive = obj[QStringLiteral("int")].toBool();
            common = obj[QStringLiteral("common")].toBool();
            type = (Core::DataSubType)obj[QStringLiteral("type")].toInt();
            uid = obj[QStringLiteral("uid")].toString();
            rel_type = (Core::RelType)obj[QStringLiteral("rel_type")].toInt();
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
        Core::RelType rel_type;

        Core::DataSubType type;
    };
}

#endif // VIEW_SETTINGS
