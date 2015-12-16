#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/web/web_sub_types.h"

namespace Core {
    #define IURL Qt::UserRole + 1
    #define IFULLPATH IURL + 1
    #define IPLAYABLE IFULLPATH + 1
    #define IATTRS IPLAYABLE + 1
    #define IEXECCOUNTS IATTRS + 1
    #define ITREEPATH IEXECCOUNTS + 1
    #define ITREESTR ITREEPATH + 1
    #define ISTATERESTORE ITREESTR + 1
    #define IUID ISTATERESTORE + 1
    #define ITYPE IUID + 1

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
        inline virtual ~ItemFields() {}

        ItemFields(QVariantMap & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(QString title, int initState = DEFAULT_FILE_STATE);
        ItemFields(int state = DEFAULT_FILE_STATE);

        inline QVariant uid() const         { return attrs.value(JSON_TYPE_UID); }
        inline QVariant owner() const       { return attrs.value(JSON_TYPE_OWNER_ID); }
        inline QVariant artistUid() const   { return attrs.value(JSON_TYPE_ARTIST_UID); }
        inline QVariant songUid() const     { return attrs.value(JSON_TYPE_SONG_UID); }

        inline QVariant title() const       { return attrs.value(JSON_TYPE_TITLE); }
        inline QVariant path() const        { return attrs.value(JSON_TYPE_PATH); }
        virtual inline QString refresh_path() { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline QVariant extension() const   { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QVariant duration() const    { return attrs.value(JSON_TYPE_DURATION); }
        QStringList info() const;
        inline QVariant _info() const       { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const        { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant genreID() const     { return attrs.value(JSON_TYPE_GENRE_ID, -1); }
        inline QVariant bpm() const         { return attrs.value(JSON_TYPE_BPM, 0); }

        inline Web::SubType subtipe() const   { return (Web::SubType)attrs.value(JSON_TYPE_SUB_TYPE, Web::site_none).toInt(); }

        inline QVariant titlesCache() const    { return attrs.value(JSON_TYPE_TITLE_CACHES); }

        inline void setUid(QVariant newId)              { attrs[JSON_TYPE_UID] = newId; }
        inline void setArtistUid(QVariant newArtistId)  { attrs[JSON_TYPE_ARTIST_UID] = newArtistId; }
        inline void setSongUid(QVariant newSongId)      { attrs[JSON_TYPE_SONG_UID] = newSongId; }

        inline void setOwner(QVariant newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline void setBpm(QVariant newBeat)            { attrs[JSON_TYPE_BPM] = newBeat; }
        inline void setDuration(QVariant newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline void setGenre(QVariant newGenreID)       { attrs[JSON_TYPE_GENRE_ID] = newGenreID; }
        inline void setSize(QVariant newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline void setPath(QVariant newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline void setRefreshPath(QVariant newPath)    { attrs[JSON_TYPE_REFRESH_PATH] = newPath; }

        inline void setTitle(QVariant newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline void setExtension(QVariant newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline void setInfo(QVariant newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }
        inline void setTitlesCache(QVariant newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline void setVideoPath(QVariant videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; }
        inline void setSubtype(Web::SubType subType)    { attrs[JSON_TYPE_SUB_TYPE] = subType; }

        inline bool hasInfo() const {return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && _info().isValid());}

        inline virtual QString toUid() { return QString(); }
        virtual QJsonObject toJson();
        QVariantMap toInnerAttrs(int itemType) const;
    protected:
        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
