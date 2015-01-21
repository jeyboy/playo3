#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "misc/settings.h"
#include "misc/file_utils/filename_conversions.h"

namespace Playo3 {
    #define UPDATEID Qt::DisplayRole + 1

    #define TITLEID Qt::DisplayRole
    #define EXTENSIONID Qt::UserRole - 1
    #define PATHID EXTENSIONID - 1
    #define FOLDERID PATHID - 1
    #define STATEID FOLDERID - 1
    #define INFOID STATEID - 1

    #define TITLESCACHEID INFOID - 1
    #define PROGRESSID TITLESCACHEID - 1
    #define ADDFONTID PROGRESSID - 1
    #define REMOTEID ADDFONTID - 1
    #define FULLPATHID REMOTEID - 1

    class ItemFields : public ItemState {
    public:
        inline ItemFields() {}

        ItemFields(QJsonObject * hash);

        ItemFields(QString title, int initState = DEFAULT_MODEL_ITEM_STATE);

        ItemFields(int state = DEFAULT_MODEL_ITEM_STATE);

        inline QVariant title() const       { return attrs.value(JSON_TYPE_TITLE); }
        inline QVariant path() const        { return attrs.value(JSON_TYPE_PATH); }
        inline QVariant extension() const   { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QVariant duration() const    { return attrs.value(JSON_TYPE_DURATION); }
        inline QVariant info() const        { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const        { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant genreID() const     { return attrs.value(JSON_TYPE_GENRE_ID, ""); }
        inline QVariant bpm() const         { return attrs.value(JSON_TYPE_BPM, 0); }

        inline void setBpm(QVariant newBeat)            { attrs[JSON_TYPE_BPM] = newBeat; }
        inline void setDuration(QVariant newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline void setGenre(QVariant newGenreID)       { attrs[JSON_TYPE_GENRE_ID] = newGenreID; }
        inline void setSize(QVariant newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline void setPath(QVariant newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline void setTitle(QVariant newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline void setExtension(QVariant newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline void setInfo(QVariant newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }

        inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && info().isValid());}

        inline virtual QString toUID() { return ""; }
        virtual QJsonObject toJson();

        QString downloadTitle() const;

        //TODO: add prepare titles method

    protected:
        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
