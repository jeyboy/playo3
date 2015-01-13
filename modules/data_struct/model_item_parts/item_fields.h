#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"

namespace Playo3 {
    #define TITLEID Qt::UserRole - 1
    #define EXTENSIONID Qt::UserRole - 2
    #define PATHID Qt::UserRole - 3
    #define FOLDERID Qt::UserRole - 4
    #define STATEID Qt::UserRole - 5
    #define TITLESCACHEID Qt::UserRole - 6
    #define PROGRESSID Qt::UserRole - 7
    #define INFOID Qt::UserRole - 8
    #define ADDFONTID Qt::UserRole - 9

    class ItemFields : public ItemState {
    public:
        ItemFields() {}

        ItemFields(QJsonObject * hash) : ItemState(hash -> value(JSON_TYPE_STATE).toInt()) {
            path = hash -> value(JSON_TYPE_PATH).toString();
            title = hash -> value(JSON_TYPE_TITLE).toString();
            extension = hash -> value(JSON_TYPE_EXTENSION).toString();
            genreID = hash -> value(JSON_TYPE_GENRE_ID).toInt(-1);
            duration = hash -> value(JSON_TYPE_DURATION).toString("");

            size = hash -> value(JSON_TYPE_BYTES_SIZE).toInt(-1);
            info = hash -> value(JSON_TYPE_INFO).toString("");
            bpm = hash -> value(JSON_TYPE_BPM).toInt(0);
        }

        QJsonObject * toJson() {
            return 0;
        }

    protected:
        QString path, title, extension, duration, info;
        int genreID, size, bpm;
    };
}

#endif // MODEL_ITEM_FIELDS_H
