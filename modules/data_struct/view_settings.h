#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/data_struct/view_types.h"
#include <qjsonobject.h>

namespace Playo3 { //TODO: maybe change on bitset ?
    struct ViewSettings {
        bool deleteFile;
        bool playlist;
        bool interactive;
        bool common;

        ViewType type;

        ViewSettings() {
            deleteFile = false;
            playlist = false;
            interactive = false;
            common = false;
            type = tree;
        }

        ViewSettings(QJsonObject obj) {
            deleteFile = obj["del"].toBool();
            playlist = obj["play"].toBool();
            interactive = obj["int"].toBool();
            common = obj["common"].toBool();
            type = (ViewType)obj["type"].toInt();
        }

        QJsonObject toJson() {
            QJsonObject obj;

            obj["del"] = deleteFile;
            obj["play"] = playlist;
            obj["int"] = interactive;
            obj["common"] = common;
            obj["type"] = type;

            return obj;
        }
    };
}

#endif // VIEW_SETTINGS
