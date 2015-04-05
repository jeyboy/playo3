#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/data_struct/container_types.h"
#include <qjsonobject.h>

namespace Playo3 {
    struct ViewSettings {
        ViewSettings(bool isCommon = false, bool delFile = false, bool isInteractive = false, bool isPlaylist = false, QString uniq_id = QString())
            : deleteFile(delFile), playlist(isPlaylist), interactive(isInteractive), common(isCommon), uid(uniq_id), type(tree)  {
        }

        ViewSettings(QJsonObject obj) {
            deleteFile = obj["del"].toBool();
            playlist = obj["play"].toBool();
            interactive = obj["int"].toBool();
            common = obj["common"].toBool();
            type = (ContainerType)obj["type"].toInt();
            uid = obj["uid"].toString();
        }

        QJsonObject toJson() {
            QJsonObject obj;

            obj["del"] = deleteFile;
            obj["play"] = playlist;
            obj["int"] = interactive;
            obj["common"] = common;
            obj["type"] = type;
            if (!uid.isEmpty())
                obj["uid"] = uid;

            return obj;
        }

        bool deleteFile;
        bool playlist;
        bool interactive;
        bool common;
        QString uid;

        ContainerType type;
    };
}

#endif // VIEW_SETTINGS
