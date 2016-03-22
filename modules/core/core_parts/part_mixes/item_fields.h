#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "item_field_defines.h"
#include "item_errors.h"
#include "settings.h"
#include "modules/core/data_sub_types.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/misc/format.h"

namespace Core {
    class ItemFields : public ItemState {
    public:
        inline ItemFields() {}
        inline virtual ~ItemFields() {}

        ItemFields(QVariantMap & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(QString title, int initState = DEFAULT_ITEM_STATE);
        ItemFields(int state = DEFAULT_ITEM_STATE);

        inline QVariant uid() const                             { return attrs.value(JSON_TYPE_UID); }
        inline QVariant owner() const                           { return attrs.value(JSON_TYPE_OWNER_ID); }
        inline QVariant artistUids() const                      { return attrs.value(JSON_TYPE_ARTIST_UIDS); }
        inline QVariant songUid() const                         { return attrs.value(JSON_TYPE_SONG_UID); }

        inline QVariant title() const                           { return attrs.value(JSON_TYPE_TITLE); }
        inline QVariant path() const                            { return attrs.value(JSON_TYPE_PATH); }
        virtual inline QString refresh_path()                   { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline QVariant extension() const                       { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QVariant startPos() const                        { return attrs.value(JSON_TYPE_START_POS); }
        inline qint64 startPosMillis() const                    { return attrs.value(JSON_TYPE_START_POS, 0).toLongLong(); }
        inline QVariant duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }
        QStringList info() const;
        inline QVariant _info() const                           { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant genreID() const                         { return attrs.value(JSON_TYPE_GENRE_ID, -1); }
        inline QVariant bpm() const                             { return attrs.value(JSON_TYPE_BPM, 0); }

        inline Web::SubType subtipe() const                     { return (Web::SubType)attrs.value(JSON_TYPE_SUB_TYPE, Web::site_none).toInt(); }

        inline QVariant titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }
        inline QVariant error() const                           { return attrs.value(JSON_TYPE_ERROR); }


        inline void setUid(const QVariant & newId)              { attrs[JSON_TYPE_UID] = newId; }
        inline void addArtistUid(QString id)                    { attrs[JSON_TYPE_ARTIST_UIDS].toStringList() << id; }
        inline void setArtistUids(const QStringList & ids)      { attrs[JSON_TYPE_ARTIST_UIDS] = ids; }
        inline void setSongUid(const QVariant & newSongId)      { attrs[JSON_TYPE_SONG_UID] = newSongId; }

        inline void setOwner(const QVariant & newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline void setBpm(const QVariant & newBeat)            { attrs[JSON_TYPE_BPM] = newBeat; }
        inline void setStartPos(const QVariant & newStartPos)   { attrs[JSON_TYPE_START_POS] = newStartPos; }
        inline void setDuration(const QVariant & newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline void setGenre(const QVariant & newGenreID)       { attrs[JSON_TYPE_GENRE_ID] = newGenreID; }
        inline void setSize(const QVariant & newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline void setPath(const QVariant & newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline void setRefreshPath(const QVariant & newPath)    { attrs[JSON_TYPE_REFRESH_PATH] = newPath; }

        inline void setTitle(const QVariant & newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline void setExtension(const QVariant & newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline void setInfo(const QVariant & newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }
        inline void setTitlesCache(const QVariant & newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline void setVideoPath(const QVariant & videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; }
        inline void setSubtype(Web::SubType subType)            { attrs[JSON_TYPE_SUB_TYPE] = subType; }
        inline void setError(const QVariant & error)            {
            if (error.toInt() == err_none)
                attrs.remove(JSON_TYPE_ERROR);
            else
                attrs[JSON_TYPE_ERROR] = error;
        }

        inline bool hasInfo() const {return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && _info().isValid());}




        void openLocation();
        bool removePhysicalObject();

        bool isExist() const;
        bool isShareable() const;
        bool isRemote() const;

        QString toUid();
        QUrl toUrl() const;

        QString fullPath() const;

        inline void setParted(const QVariant & isParted)        { attrs -> operator[](JSON_TYPE_PARTIAL) = isParted; }
        inline bool isParted() const                            { return attrs -> value(JSON_TYPE_PARTIAL, false).toBool(); }





        virtual QJsonObject toJson();
        QVariantMap toInnerAttrs(int itemType) const;
    protected:
        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
