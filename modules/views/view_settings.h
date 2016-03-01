#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/core/playlist_types.h"
#include <qjsonobject.h>

namespace Views {
    using namespace Core;

    struct Params {
        Params(PlaylistType cType, const QString & uniq_id = QString(), PlaylistRelType rel = playlist_none_rel) : deleteFile(false), playlist(true),
            interactive(false), common(false), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(PlaylistType cType = playlist_tree, bool isCommon = false, bool delFile = false, bool isInteractive = false,
            bool isPlaylist = false, const QString & uniq_id = QString(), PlaylistRelType rel = playlist_none_rel) : deleteFile(delFile), playlist(isPlaylist),
            interactive(isInteractive), common(isCommon), uid(uniq_id), rel_type(rel), type(cType) { }

        Params(const QJsonObject & obj) {
            deleteFile = obj[QStringLiteral("del")].toBool();
            playlist = obj[QStringLiteral("play")].toBool();
            interactive = obj[QStringLiteral("int")].toBool();
            common = obj[QStringLiteral("common")].toBool();
            type = (PlaylistType)obj[QStringLiteral("type")].toInt();
            uid = obj[QStringLiteral("uid")].toString();
            rel_type = (PlaylistRelType)obj[QStringLiteral("rel_type")].toInt();
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
        PlaylistRelType rel_type;

        PlaylistType type;
    };
}

#endif // VIEW_SETTINGS
