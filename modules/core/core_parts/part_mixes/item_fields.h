#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>

#include "item_state.h"
#include "json_fields.h"
#include "item_errors.h"
#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/web/web_sub_types.h"
#include "modules/core/misc/format.h"

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
    #define ISPOILITEM ITYPE + 1

    #define ITITLE Qt::DisplayRole
    #define IEXTENSION Qt::UserRole - 1
    #define IPATH IEXTENSION - 1
    #define IFOLDER IPATH - 1
    #define ISTATE IFOLDER - 1
    #define IINFO ISTATE - 1
    #define ITITLESCACHE IINFO - 1
    #define IADDFONT ITITLESCACHE - 1
    #define IREMOTE IADDFONT - 1
    #define IERROR IREMOTE - 1

    class ItemFields : public ItemState {
    public:
        inline ItemFields() {}
        inline virtual ~ItemFields() {}

        ItemFields(QVariantMap & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(QString title, int initState = DEFAULT_FILE_STATE);
        ItemFields(int state = DEFAULT_FILE_STATE);

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

        inline virtual QString toUid() { return QString(); }
        virtual QJsonObject toJson();
        QVariantMap toInnerAttrs(int itemType) const;
    protected:
        inline void setParted(const QVariant & isParted)        { attrs[JSON_TYPE_PARTIAL] = isParted; }
        inline bool isParted() const                            { return attrs.value(JSON_TYPE_PARTIAL, false).toBool(); }


        inline void _setTitle(const QVariant & newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline QVariant _title() const                           { return attrs.value(JSON_TYPE_TITLE); }

        inline void _setTitlesCache(const QVariant & newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline QVariant _titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }

        inline void _setVideoPath(const QVariant & videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; } // maybe need move to sources if source site is supported ?

        inline void _setDuration(const QVariant & newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline QVariant _duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 _durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }

        // per source info
        inline void _setUid(const QVariant & newId)              { attrs[JSON_TYPE_UID] = newId; }
        inline QVariant _uid() const                             { return attrs.value(JSON_TYPE_UID); }

        inline void _setOwner(const QVariant & newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline QVariant _owner() const                           { return attrs.value(JSON_TYPE_OWNER_ID); }

        inline void _addArtistUid(const QString & id)            { attrs[JSON_TYPE_ARTIST_UIDS].toStringList() << id; }
        inline void _setArtistUids(const QStringList & ids)      { attrs[JSON_TYPE_ARTIST_UIDS] = ids; }
        inline QVariant _artistUids() const                      { return attrs.value(JSON_TYPE_ARTIST_UIDS); }

        inline void _setSongUid(const QVariant & newSongId)      { attrs[JSON_TYPE_SONG_UID] = newSongId; }
        inline QVariant _songUid() const                         { return attrs.value(JSON_TYPE_SONG_UID); }

        inline void _setPath(const QVariant & newPath)           { attrs[JSON_TYPE_PATH] = newPath; }
        inline QVariant _path() const                            { return attrs.value(JSON_TYPE_PATH); }

        inline void _setRefreshPath(const QVariant & newPath)    { attrs[JSON_TYPE_REFRESH_PATH] = newPath; }
        virtual inline QString _refresh_path()                   { return attrs.value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline void _setExtension(const QVariant & newExtension) { attrs[JSON_TYPE_EXTENSION] = newExtension; }
        inline QVariant _extension() const                       { return attrs.value(JSON_TYPE_EXTENSION); }

        inline void _setStartPos(const QVariant & newStartPos)   { attrs[JSON_TYPE_START_POS] = newStartPos; }
        inline QVariant _startPos() const                        { return attrs.value(JSON_TYPE_START_POS); }
        inline qint64 _startPosMillis() const                    { return attrs.value(JSON_TYPE_START_POS, 0).toLongLong(); }

        inline void _setInfo(const QVariant & newInfo)           { attrs[JSON_TYPE_INFO] = newInfo; }
        QStringList _info() const;
        inline QVariant _infoVar() const                         { return attrs.value(JSON_TYPE_INFO); }
        inline bool _hasInfo() const                             { return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && _infoVar().isValid());}

        inline void _setSize(const QVariant & newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline QVariant _size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }

        inline void _setSubtype(Web::SubType subType)            { attrs[JSON_TYPE_SUB_TYPE] = subType; }
        inline Web::SubType _subtipe() const                     { return (Web::SubType)attrs.value(JSON_TYPE_SUB_TYPE, Web::site_none).toInt(); }

        QVariantMap attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
