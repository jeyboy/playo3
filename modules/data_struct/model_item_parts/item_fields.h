#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "misc/settings.h"
#include "misc/file_utils/filename_conversions.h"

namespace Playo3 {
    #define TITLEID Qt::DisplayRole
    #define EXTENSIONID Qt::UserRole - 1
    #define PATHID EXTENSIONID - 1
    #define FOLDERID PATHID - 1
    #define STATEID FOLDERID - 1
    #define INFOID STATEID - 1

    #define TITLESCACHEID INFOID - 1
    #define PROGRESSID TITLESCACHEID - 1
    #define ADDFONTID PROGRESSID - 1

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
