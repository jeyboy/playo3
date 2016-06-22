#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include "settings.h"
#include "item_field_defines.h"
#include <qjsonobject.h>
#include <qurl.h>
#include <qvariant.h>

#include "modules/core/data_sub_types.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/misc/format.h"

namespace Core {
    class ItemFields : public ItemState {
    public:
        static inline QString toUid(const QVariant & owner, const QVariant & id) {
            return /*owner.isValid() &&*/ id.isValid() ? owner.toString() % QStringLiteral("_") % id.toString() : QString();
        }

        inline ItemFields() {}
        inline virtual ~ItemFields() {}

        ItemFields(const QVariantHash & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(const DataSubType & subType, int state = DEFAULT_ITEM_STATE);

        inline QVariantMap cueMap()                             { return attrs[JSON_TYPE_CUE_MAP].toMap(); }
        inline QVariantMap takeCueMap()                         { return attrs.take(JSON_TYPE_CUE_MAP).toMap(); }

        inline bool isLoadable()                                { return attrs.contains(JSON_TYPE_CONTAINER_LOADABLE); }
        inline bool removeLoadability()                         { return attrs.remove(JSON_TYPE_CONTAINER_LOADABLE); }
        inline QVariant loadableAttrs()                         { return attrs[JSON_TYPE_CONTAINER_LOADABLE]; }

        inline QVariant id() const                              { return attrs.value(JSON_TYPE_ID); }
        inline QVariant owner() const                           { return attrs.value(JSON_TYPE_OWNER_ID); }
        inline QVariant artistIds() const                       { return attrs.value(JSON_TYPE_ARTIST_IDS); }
        inline QVariant songId() const                          { return attrs.value(JSON_TYPE_SONG_ID); }

        inline QVariant title() const                           { return attrs.value(JSON_TYPE_TITLE); }
        inline QVariant path() const                            { return attrs.value(JSON_TYPE_PATH); }
        virtual inline QString refresh_path()                   { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline QVariant extension() const                       { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QString extensionStr() const                     { return attrs.value(JSON_TYPE_EXTENSION, QStringLiteral("mp3")).toString(); }

        inline QVariant startPos() const                        { return attrs.value(JSON_TYPE_START_POS); }
        inline qint64 startPosMillis() const                    { return attrs.value(JSON_TYPE_START_POS, 0).toLongLong(); }
        inline QVariant duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }
        QStringList info() const;
        inline QVariant _info() const                           { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant genreID() const                         { return attrs.value(JSON_TYPE_GENRE_ID, -1); }
        inline QVariant bpm() const                             { return attrs.value(JSON_TYPE_BPM, 0); }

        inline QVariant titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }
        inline QVariant error() const                           { return attrs.value(JSON_TYPE_ERROR); }

        inline DataMediaType mediaType() const                  { return (DataMediaType)attrs.value(JSON_TYPE_MEDIA_TYPE, dmt_unknow).toInt(); }
        inline void setDataMediaType(const DataMediaType & mediaType)
                                                                { attrs[JSON_TYPE_MEDIA_TYPE] = mediaType; }

        inline DataSubType dataType() const                     { return (DataSubType)attrs.value(JSON_TYPE_ITEM_TYPE, dt_none).toInt(); }
        inline void setDatatype(const DataSubType & dataType)   { attrs[JSON_TYPE_ITEM_TYPE] = dataType; }

        inline void setCueMap(const QVariant & map)             { attrs[JSON_TYPE_CUE_MAP] = map; }
        inline void setLoadableAttrs(const QVariant & data)     { attrs[JSON_TYPE_CONTAINER_LOADABLE] = data; }

        inline void setId(const QVariant & newId)               { attrs[JSON_TYPE_ID] = newId; }
        inline void addArtistId(const QString & id)             { attrs[JSON_TYPE_ARTIST_IDS] = attrs[JSON_TYPE_ARTIST_IDS].toStringList() << id; }
        inline void setArtistIds(const QStringList & ids)       { attrs[JSON_TYPE_ARTIST_IDS] = ids; }
        inline void setSongId(const QVariant & newSongId)       { attrs[JSON_TYPE_SONG_ID] = newSongId; }

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
        inline void setError(const QVariant & error)            {
            if (error.toInt() == err_none)
                attrs.remove(JSON_TYPE_ERROR);
            else
                attrs[JSON_TYPE_ERROR] = error;
        }
        bool hasInfo() const {return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && _info().isValid());}


        void openLocation();
        bool removePhysicalObject();

        bool isExist() const;
        bool isShareable() const;
        bool isRemote() const;

        QString toUid();
        QUrl toUrl() const;

        QString fullPath() const;

        inline void setParted(const QVariant & isParted)        { attrs[JSON_TYPE_PARTIAL] = isParted; }
        inline bool isParted() const                            { return attrs.value(JSON_TYPE_PARTIAL, false).toBool(); }

        virtual QJsonObject toJson();
        QVariantHash toInnerAttrs() const;
    protected:
        QVariantHash attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
