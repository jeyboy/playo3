#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "item_field_defines.h"
#include "json_fields.h"
#include "item_errors.h"
#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/data_sub_types.h"
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

        // const info
        inline void setGenreID(const QVariant & newGenreID)     { attrs[JSON_TYPE_GENRE_ID] = newGenreID; }
        inline QVariant genreID() const                         { return attrs.value(JSON_TYPE_GENRE_ID, -1); }

        inline void setBpm(const QVariant & newBeat)            { attrs[JSON_TYPE_BPM] = newBeat; }
        inline QVariant bpm() const                             { return attrs.value(JSON_TYPE_BPM, 0); }

        inline void setError(const QVariant & error)            {
            if (error.toInt() == err_none)
                attrs.remove(JSON_TYPE_ERROR);
            else
                attrs[JSON_TYPE_ERROR] = error;
        }
        inline QVariant error() const                           { return attrs.value(JSON_TYPE_ERROR); }

        inline void setActiveSourceIndex(int index)             { attrs[JSON_TYPE_ACTIVE_SOURCE] = index; }
        inline int activeSourceIndex() const                    { return attrs.value(JSON_TYPE_ACTIVE_SOURCE, 0).toInt(); }

        inline void setTitle(const QVariant & newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline QVariant title() const                           { return attrs.value(JSON_TYPE_TITLE); }

        inline void setTitlesCache(const QVariant & newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline QVariant titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }

        inline void setVideoPath(const QVariant & videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; } // maybe need move to sources if source site is supported ?

        inline void setDuration(const QVariant & newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline QVariant duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }

        // per source info
        inline void setId(const QVariant & newId)               { attrs[JSON_TYPE_ID] = newId; }
        inline QVariant id() const                              { return attrs.value(JSON_TYPE_ID); }

        inline void setOwner(const QVariant & newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline QVariant owner() const                           { return attrs.value(JSON_TYPE_OWNER_ID); }

        inline void addArtistUid(const QString & id)            { attrs[JSON_TYPE_ARTIST_UIDS].toStringList() << id; }
        inline void setArtistUids(const QStringList & ids)      { attrs[JSON_TYPE_ARTIST_UIDS] = ids; }
        inline QVariant artistUids() const                      { return attrs.value(JSON_TYPE_ARTIST_UIDS); }

        inline void setSongUid(const QVariant & newSongId)      { attrs[JSON_TYPE_SONG_UID] = newSongId; }
        inline QVariant songUid() const                         { return attrs.value(JSON_TYPE_SONG_UID); }

        inline void setPath(const QVariant & newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline QVariant path() const                            { return attrs.value(JSON_TYPE_PATH); }

        inline void setRefreshPath(const QVariant & newPath)    { attrs[JSON_TYPE_REFRESH_PATH] = newPath; }
        virtual inline QString refresh_path()                   { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline void setExtension(const QVariant & newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline QVariant extension() const                       { return attrs.value(JSON_TYPE_EXTENSION); }

        inline void setStartPos(const QVariant & newStartPos)   { attrs[JSON_TYPE_START_POS] = newStartPos; }
        inline QVariant startPos() const                        { return attrs.value(JSON_TYPE_START_POS); }
        inline qint64 startPosMillis() const                    { return attrs.value(JSON_TYPE_START_POS, 0).toLongLong(); }

        inline void setInfo(const QVariant & newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }
        QStringList info() const;
        inline QVariant infoVar() const                         { return attrs.value(JSON_TYPE_INFO); }
        inline bool hasInfo() const                             { return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && infoVar().isValid());}

        inline void setSize(const QVariant & newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline QVariant size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }

        inline void setDatatype(const DataSubType & dataType)   { attrs[JSON_TYPE_DATA_SUB_TYPE] = dataType; }
        inline DataSubType dataType() const                     { return (DataSubType)attrs.value(JSON_TYPE_SUB_TYPE, dt_none).toInt(); }

        inline bool setShareable(const bool able)               { attrs[JSON_TYPE_IS_SHAREABLE] = able; }
        inline bool isShareable() const                         { return attrs.value(JSON_TYPE_IS_SHAREABLE, false).toBool(); }

        inline bool setRemote(const bool able)                  { attrs[JSON_TYPE_IS_REMOTE] = able; }
        inline bool isRemote() const                            { return attrs.value(JSON_TYPE_IS_REMOTE, false).toBool(); }

        inline virtual QString toUid() { return QString(); } // not virtual anymore
        virtual QJsonObject toJson();
        QVariantMap toInnerAttrs(int itemType) const; // remove later
    protected:
        inline void setParted(const QVariant & isParted)        { attrs[JSON_TYPE_PARTIAL] = isParted; }
        inline bool isParted() const                            { return attrs.value(JSON_TYPE_PARTIAL, false).toBool(); }

        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
