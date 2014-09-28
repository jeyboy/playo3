#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <QObject>

#include "model_item_state.h"
#include "model_item_json_fields.h"
#include "../json/json_base.h"

namespace ModelItems {
    #include <QJsonObject>

    #define TITLEID Qt::UserRole + 1
    #define EXTENSIONID Qt::UserRole + 2
    #define PATHID Qt::UserRole + 3
    #define FOLDERID Qt::UserRole + 4
    #define STATEID Qt::UserRole + 5
    #define TITLESCACHEID Qt::UserRole + 6
    #define PROGRESSID Qt::UserRole + 7
    #define INFOID Qt::UserRole + 8
    #define ADDFONTID Qt::UserRole + 9

    struct Fields {
        Fields() {}

        Fields(QJsonObject * hash) {
            state = new ModelItemState(hash -> value(JsonType::state).toInt());
            path = hash -> value(JsonType::path).toString();
            title = hash -> value(JsonType::title).toString();
            extension = hash -> value(JsonType::extension).toString();
            genreID = hash -> value(JsonType::genre_id).toInt(-1);
            duration = hash -> value(JsonType::duration).toString("");

            size = hash -> value(JsonType::bytes_size).toInt(-1);
            info = hash -> value(JsonType::info).toString("");
            bpm = hash -> value(JsonType::bpm).toInt(0);
        }

        ModelItemState * state;
        QString path, title, extension, duration, info;
        int genreID, size, bpm;

    };
}

#endif // MODEL_ITEM_FIELDS_H
