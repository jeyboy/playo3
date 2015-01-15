#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "misc/settings.h"

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
            _path = hash -> value(JSON_TYPE_PATH).toString();
            _title = hash -> value(JSON_TYPE_TITLE).toString();
            _extension = hash -> value(JSON_TYPE_EXTENSION).toString();
            _genreID = hash -> value(JSON_TYPE_GENRE_ID).toInt(-1);
            _duration = hash -> value(JSON_TYPE_DURATION).toString("");

            _size = hash -> value(JSON_TYPE_BYTES_SIZE).toInt(-1);
            _info = hash -> value(JSON_TYPE_INFO).toString("");
            _bpm = hash -> value(JSON_TYPE_BPM).toInt(0);
        }

        ItemFields(QString path, QString title = "", QString extension = "", int size = 0) : ItemState() {
            _path = path;
            _title = title;
            _extension = extension;
            _genreID = -1;
            _duration = "";

            _size = size;
            _info = "";
            _bpm = 0;
        }

        inline QString title() const { return _title; }

        inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && !_info.isEmpty());}
        inline void setInfo(QString newInfo) { _info = newInfo; }
        QStringList getInfo() const;

        inline void setBpm(int newBeat) { _bpm = newBeat; }
        inline void setDuration(QString newDuration) { _duration = newDuration;}
        inline void setGenre(int newGenreID) { _genreID = newGenreID;}
        inline void setPath(QString newPath) { _path = newPath;}

        inline virtual QString toUID() { return ""; }
        virtual QJsonObject * toJson() {
            QJsonObject root = QJsonObject();

            root[JSON_TYPE_TITLE] = _title;
            root[JSON_TYPE_STATE] = saveStateVal();
            root[JSON_TYPE_PATH] = _path;

            if (!_info.isEmpty())
                root[JSON_TYPE_INFO] = _info;

            if (_bpm != 0)
                root[JSON_TYPE_BPM] = _bpm;

            if (_size != -1)
                root[JSON_TYPE_BYTES_SIZE] = _size;

            if (_genreID != -1)
                root[JSON_TYPE_GENRE_ID] = _genreID;

            if (!_duration.isEmpty())
                root[JSON_TYPE_DURATION] = _duration;

            if (!_extension.isNull())
                root[JSON_TYPE_EXTENSION] = _extension;

            return root;
        }

        virtual QUrl toUrl();

    protected:
        QString _path, _title, _extension, _duration, _info;
        int _genreID, _size, _bpm;
    };
}

#endif // MODEL_ITEM_FIELDS_H
