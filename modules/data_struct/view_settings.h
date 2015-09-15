#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/data_struct/container_types.h"
#include "modules/data_struct/web/vk/vk_rel_types.h"
#include "misc/web_utils/json.h"

namespace Playo3 {
    struct ViewSettings {
        static ViewSettings vk(QString id) { return ViewSettings(Playo3::vk, false, false, false, true, id); }
        static ViewSettings soundcloud(QString id) { return ViewSettings(Playo3::soundcloud, false, false, false, true, id); }
        static ViewSettings echo() { return ViewSettings(Playo3::echo, false, false, false, true); }

        ViewSettings(ContainerType cType = tree, bool isCommon = false, bool delFile = false, bool isInteractive = false,
            bool isPlaylist = false, QString uniq_id = QString(), RelType rel = none_rel) : deleteFile(delFile), playlist(isPlaylist),
            interactive(isInteractive), common(isCommon), uid(uniq_id), rel_type(rel), type(cType) { }

        ViewSettings(const Json::Obj & obj) {
            deleteFile = obj.boolVal(QStringLiteral("del"));
            playlist = obj.boolVal(QStringLiteral("play"));
            interactive = obj.boolVal(QStringLiteral("int"));
            common = obj.boolVal(QStringLiteral("common"));
            type = (ContainerType)obj.intVal(QStringLiteral("type"));
            uid = obj.str(QStringLiteral("uid"));
            rel_type = (RelType)obj.intVal(QStringLiteral("rel_type"));
        }

        Json::Obj toJson() {
            Json::Obj obj;

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
        RelType rel_type;

        ContainerType type;
    };
}

#endif // VIEW_SETTINGS
