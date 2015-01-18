#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "misc/settings.h"
#include "misc/file_utils/filename_conversions.h"

namespace Playo3 {
    #define TITLEID Qt::UserRole - 1
    #define EXTENSIONID Qt::UserRole - 2
    #define PATHID Qt::UserRole - 3
    #define FOLDERID Qt::UserRole - 4
    #define STATEID Qt::UserRole - 5
    #define INFOID Qt::UserRole - 6

    #define TITLESCACHEID Qt::UserRole - 7
    #define PROGRESSID Qt::UserRole - 8
    #define ADDFONTID Qt::UserRole - 9

    class ItemFields : public ItemState {
    public:
        inline ItemFields() {}

        ItemFields(QJsonObject * hash);

        ItemFields(QString path, QString title = "", QString extension = "", int size = 0, int initState = DEFAULT_MODEL_ITEM_STATE);

        ItemFields(int state);

        inline QString title() const { return _title; }

        inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && !_info.isEmpty());}
        inline void setInfo(QString newInfo) { _info = newInfo; }
//        QStringList getInfo() const;

        inline void setBpm(int newBeat) { _bpm = newBeat; }
        inline void setDuration(QString newDuration) { _duration = newDuration;}
        inline void setGenre(int newGenreID) { _genreID = newGenreID;}
        inline void setPath(QString newPath) { _path = newPath;}

        inline virtual QString toUID() { return ""; }
        virtual QJsonObject toJson();

        QString downloadTitle() const;

        //TODO: add prepare titles method

    protected:
        QString _path, _title, _extension, _duration, _info;
        int _genreID, _size, _bpm;
    };
}

#endif // MODEL_ITEM_FIELDS_H
