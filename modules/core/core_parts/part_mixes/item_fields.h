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

        inline bool hasMoreItems() const                        { return isLoadable() || isFetchable(); }
        inline void removeLoader()                              { //INFO: remove first valid loader
            if (isLoadable())
                removeLoadability();
            else if (isFetchable())
                removeFetchability();
        }

        inline QVariantMap cueMap()                             { return attrs[JSON_TYPE_CUE_MAP].toMap(); }
        inline QVariantMap takeCueMap()                         { return attrs.take(JSON_TYPE_CUE_MAP).toMap(); }

        inline bool isLoadable() const                          { return attrs.contains(JSON_TYPE_CONTAINER_LOADABLE); }
        inline bool removeLoadability()                         { return attrs.remove(JSON_TYPE_CONTAINER_LOADABLE); }
        inline QString loadableCmd()                            { return attrs[JSON_TYPE_CONTAINER_LOADABLE].toString(); }

        inline bool isFetchable() const                         { return attrs.contains(JSON_TYPE_CONTAINER_FETCHABLE); }
        inline bool removeFetchability()                        { return attrs.remove(JSON_TYPE_CONTAINER_FETCHABLE); }
        inline QString fetchableCmd()                           { return attrs[JSON_TYPE_CONTAINER_FETCHABLE].toString(); }

        inline QVariant id() const                              { return attrs.value(JSON_TYPE_ID); }
        inline QVariant ownerId() const                         { return attrs.value(JSON_TYPE_OWNER_ID); }
        inline QVariant artistIds() const                       { return attrs.value(JSON_TYPE_ARTIST_IDS); }
        inline QVariant songId() const                          { return attrs.value(JSON_TYPE_SONG_ID); }
        inline QVariant albumId() const                         { return attrs.value(JSON_TYPE_ALBUM_ID); }
        inline QVariant genreId() const                         { return attrs.value(JSON_TYPE_GENRE_ID, -1); }

        inline QVariant tags()                                  { return attrs.value(JSON_TYPE_TAGS); }

        inline QVariant title() const                           { return attrs.value(JSON_TYPE_TITLE); }
        QVariant path() const;
        inline QString refresh_path()                           { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline QVariant extension() const                       { return attrs.value(JSON_TYPE_EXTENSION); }
        inline QString extensionStr() const                     { return attrs.value(JSON_TYPE_EXTENSION, QStringLiteral("mp3")).toString(); }

        inline QVariant startPos() const                        { return attrs.value(JSON_TYPE_START_POS); }
        inline qint64 startPosMillis() const                    { return attrs.value(JSON_TYPE_START_POS, 0).toLongLong(); }
        inline QVariant duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }
        QStringList info() const;
        inline QVariant _info() const                           { return attrs.value(JSON_TYPE_INFO); }

        inline QVariant size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }
        inline QVariant bpm() const                             { return attrs.value(JSON_TYPE_BPM, 0); }

        inline QVariant artPath() const                         { return attrs.value(JSON_TYPE_MEDIA_ART); }

        inline QVariant titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }
        inline QVariant error() const                           { return attrs.value(JSON_TYPE_ERROR); }

        inline DataMediaType mediaType() const                  { return (DataMediaType)attrs.value(JSON_TYPE_MEDIA_TYPE, dmt_unknow).toInt(); }
        inline void setDataMediaType(const DataMediaType & media_type)
                                                                { attrs[JSON_TYPE_MEDIA_TYPE] = media_type; }

        inline DataSubType dataType() const                     { return (DataSubType)attrs.value(JSON_TYPE_ITEM_TYPE, dt_none).toInt(); }
        inline void setDatatype(const DataSubType & data_type)  { attrs[JSON_TYPE_ITEM_TYPE] = data_type; }

        inline void setCueMap(const QVariant & map)             { attrs[JSON_TYPE_CUE_MAP] = map; }
        inline void setLoadableAttrs(const QVariant & data)     { attrs[JSON_TYPE_CONTAINER_LOADABLE] = data; }
        inline void setFetchableAttrs(const QVariant & data)    { attrs[JSON_TYPE_CONTAINER_FETCHABLE] = data; }

        inline void setId(const QVariant & newId)               { attrs[JSON_TYPE_ID] = newId; }
        inline void setOwnerId(const QVariant & new_owner_id)   { attrs[JSON_TYPE_OWNER_ID] = new_owner_id; }
        inline void addArtistId(const QString & id)             { attrs[JSON_TYPE_ARTIST_IDS] = attrs[JSON_TYPE_ARTIST_IDS].toStringList() << id; }
        inline void setArtistIds(const QStringList & ids)       { attrs[JSON_TYPE_ARTIST_IDS] = ids; }
        inline void setSongId(const QVariant & new_song_id)     { attrs[JSON_TYPE_SONG_ID] = new_song_id; }
        inline void setAlbumId(const QVariant & new_album_id)   { attrs[JSON_TYPE_ALBUM_ID] = new_album_id; }
        inline void setGenreId(const QVariant & new_genre_id)   { attrs[JSON_TYPE_GENRE_ID] = new_genre_id; }

        inline void addTag(const QString & tag)                 { attrs[JSON_TYPE_TAGS] = attrs[JSON_TYPE_TAGS].toStringList() << tag; }
        inline void setTags(const QStringList & tags)           { attrs[JSON_TYPE_TAGS] = tags; }


        inline void setBpm(const QVariant & new_beat)           { attrs[JSON_TYPE_BPM] = new_beat; }
        inline void setArtPath(const QVariant & new_path)       { attrs[JSON_TYPE_MEDIA_ART] = new_path; }

        inline void setStartPos(const QVariant & new_start_pos) { attrs[JSON_TYPE_START_POS] = new_start_pos; }
        inline void setDuration(const QVariant & new_duration)  { attrs[JSON_TYPE_DURATION] = new_duration; }
        inline void setSize(const QVariant & new_size)          { attrs[JSON_TYPE_BYTES_SIZE] = new_size; }
        inline void setPath(const QVariant & new_path)          { attrs[JSON_TYPE_PATH] = new_path; }
        inline void setRefreshPath(const QVariant & new_path)   { attrs[JSON_TYPE_REFRESH_PATH] = new_path; }

        inline void setTitle(const QVariant & new_title)        { attrs[JSON_TYPE_TITLE] = new_title; }
        inline void setExtension(const QVariant & new_extension){ attrs[JSON_TYPE_EXTENSION] = new_extension; }
        inline void setInfo(const QVariant & new_info)          { attrs[JSON_TYPE_INFO] = new_info; }
        inline void setTitlesCache(const QVariant & new_titles) { attrs[JSON_TYPE_TITLE_CACHES] = new_titles; }
//        inline void setVideoPath(const QVariant & videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; }
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
