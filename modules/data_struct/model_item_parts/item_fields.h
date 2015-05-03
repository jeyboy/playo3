#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "misc/settings.h"
#include "misc/file_utils/filename_conversions.h"

namespace Playo3 {
    #define IURL Qt::UserRole + 1
    #define IFULLPATH IURL + 1
    #define IPLAYABLE IFULLPATH + 1
    #define IATTRS IPLAYABLE + 1
    #define IEXECCOUNTS IATTRS + 1
    #define ITREEPATH IEXECCOUNTS + 1
    #define ITREESTR ITREEPATH + 1
    #define ISTATERESTORE ITREESTR + 1
    #define IUID ISTATERESTORE + 1


    #define ITITLE Qt::DisplayRole
    #define IEXTENSION Qt::UserRole - 1
    #define IPATH IEXTENSION - 1
    #define IFOLDER IPATH - 1
    #define ISTATE IFOLDER - 1
    #define IINFO ISTATE - 1
    #define ITITLESCACHE IINFO - 1
    #define IADDFONT ITITLESCACHE - 1
    #define IREMOTE IADDFONT - 1

    class ItemFields : public ItemState {
    public:
        inline ItemFields() {}

        ItemFields(QVariantMap & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(QString title, int initState = DEFAULT_MODEL_ITEM_STATE);
        ItemFields(int state = DEFAULT_MODEL_ITEM_STATE);

        inline QVariant uid() const         { return attrs.value(JSON_TYPE_UID); }
        inline QVariant owner() const       { return attrs.value(JSON_TYPE_OWNER_ID); }

        inline QVariant title() const       { return attrs.value(JSON_TYPE_TITLE); }
        inline QVariant path() const        { return attrs.value(JSON_TYPE_PATH); }
        inline QVariant extension() const   { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QVariant duration() const    { return attrs.value(JSON_TYPE_DURATION); }
        QStringList info() const;
        inline QVariant _info() const       { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const        { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant genreID() const     { return attrs.value(JSON_TYPE_GENRE_ID, ""); }
        inline QVariant bpm() const         { return attrs.value(JSON_TYPE_BPM, 0); }

        inline QVariant titlesCache() const    { return attrs.value(JSON_TYPE_TITLE_CACHES); }

        inline void setUid(QVariant newId)              { attrs[JSON_TYPE_UID] = newId; }
        inline void setOwner(QVariant newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline void setBpm(QVariant newBeat)            { attrs[JSON_TYPE_BPM] = newBeat; }
        inline void setDuration(QVariant newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline void setGenre(QVariant newGenreID)       { attrs[JSON_TYPE_GENRE_ID] = newGenreID; }
        inline void setSize(QVariant newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline void setPath(QVariant newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline void setTitle(QVariant newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline void setExtension(QVariant newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline void setInfo(QVariant newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }
        inline void setTitlesCache(QVariant newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline void setVideoPath(QVariant videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; }

        inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && _info().isValid());}

        inline virtual QVariant toUid() { return QVariant(); }
        virtual QJsonObject toJson();
        QVariantMap toInnerAttrs(int itemType) const;
    protected:
        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
