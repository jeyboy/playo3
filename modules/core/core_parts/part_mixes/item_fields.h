#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>
#include <qurl.h>
#include <qstringbuilder.h>

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
        static inline QString toUid(const QVariant & owner, const QVariant & id) {
            return /*owner.isValid() &&*/ id.isValid() ? owner.toString() % QStringLiteral("_") % id.toString() : QString();
        }

        inline virtual ~ItemFields() { delete attrs; }
        inline void connectToSource(ItemFields * newSource) {
            attrs = newSource -> attrs;
            copyBits(newSource -> state());
        }

        inline ItemFields() : attrs(0) {}
        ItemFields(const QVariantHash & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(const QString & title, int initState = DEFAULT_ITEM_STATE);
        ItemFields(int state);

        // const info
        inline void setGenreID(const QVariant & newGenreID)     { attrs -> operator[](JSON_TYPE_GENRE_ID) = newGenreID; }
        inline QVariant genreID() const                         { return attrs -> value(JSON_TYPE_GENRE_ID, -1); }

        inline void setBpm(const QVariant & newBeat)            { attrs -> operator[](JSON_TYPE_BPM) = newBeat; }
        inline QVariant bpm() const                             { return attrs -> value(JSON_TYPE_BPM, 0); }

        void setError(const QVariant & error);
        inline QVariant error() const                           { return attrs -> value(JSON_TYPE_ERROR); }

        inline void setTitle(const QVariant & newTitle)         { attrs -> operator[](JSON_TYPE_TITLE) = newTitle; }
        inline QVariant title() const                           { return attrs -> value(JSON_TYPE_TITLE); }

        inline void setTitlesCache(const QVariant & newTitles)  { attrs -> operator[](JSON_TYPE_TITLE_CACHES) = newTitles; }
        inline QVariant titlesCache() const                     { return attrs -> value(JSON_TYPE_TITLE_CACHES); }

        inline void setVideoPath(const QVariant & videoPath)    { attrs -> operator[](JSON_TYPE_VIDEO_PATH) = videoPath; } // maybe need move to sources if source site is supported ?

        inline void setDuration(const QVariant & newDuration)   { attrs -> operator[](JSON_TYPE_DURATION) = newDuration; }
        inline QVariant duration() const                        { return attrs -> value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   {
            QVariant dur = attrs -> value(JSON_TYPE_DURATION);
            return dur.isValid() ? Duration::toMillis(dur.toString()) : 0;
        }

        // per source info

        inline void setDatabaseId(const QVariant & newId)       { attrs -> operator[](JSON_TYPE_DATABASE_UID) = newId; }
        inline QString databaseId() const                      { return attrs -> value(JSON_TYPE_DATABASE_UID).toString(); }

        inline void setId(const QVariant & newId)               { attrs -> operator[](JSON_TYPE_ID) = newId; }
        inline QVariant id() const                              { return attrs -> value(JSON_TYPE_ID); }

        inline void setOwner(const QVariant & newOwner)         { attrs -> operator[](JSON_TYPE_OWNER_ID) = newOwner; }
        inline QVariant owner() const                           { return attrs -> value(JSON_TYPE_OWNER_ID); }

        inline void addArtistUid(const QString & id)            { attrs -> operator[](JSON_TYPE_ARTIST_UIDS) = attrs -> operator[](JSON_TYPE_ARTIST_UIDS).toStringList() << id; }
        inline void setArtistUids(const QStringList & ids)      { attrs -> operator[](JSON_TYPE_ARTIST_UIDS) = ids; }
        inline QVariant artistUids() const                      { return attrs -> value(JSON_TYPE_ARTIST_UIDS); }

        inline void setSongUid(const QVariant & newSongId)      { attrs -> operator[](JSON_TYPE_SONG_UID) = newSongId; }
        inline QVariant songUid() const                         { return attrs -> value(JSON_TYPE_SONG_UID); }

        inline void setPath(const QVariant & newPath)           { attrs -> operator[](JSON_TYPE_PATH) = newPath; }
        inline QVariant path() const                            { return attrs -> value(JSON_TYPE_PATH); }

        inline void setRefreshPath(const QVariant & newPath)    { attrs -> operator[](JSON_TYPE_REFRESH_PATH) = newPath; }
        virtual inline QString refresh_path()                   { return attrs -> value(JSON_TYPE_REFRESH_PATH).toString(); }

        inline void setExtension(const QVariant & newExtension) { attrs -> operator[](JSON_TYPE_EXTENSION) = newExtension; }
        inline QVariant extension() const                       { return attrs -> value(JSON_TYPE_EXTENSION); }

        inline void setStartPos(const QVariant & newStartPos)   { attrs -> operator[](JSON_TYPE_START_POS) = newStartPos; }
        inline QVariant startPos() const                        { return attrs -> value(JSON_TYPE_START_POS); }
        inline qint64 startPosMillis() const                    { return attrs -> value(JSON_TYPE_START_POS, 0).toLongLong(); }

        inline void setInfo(const QVariant & newInfo)           { attrs -> operator[](JSON_TYPE_INFO) = newInfo; }
        QStringList info() const;
        inline QVariant infoVar() const                         { return attrs -> value(JSON_TYPE_INFO); }
        inline bool hasInfo() const                             { return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && infoVar().isValid()); }

        inline void setSize(const QVariant & newSize)           { attrs -> operator[](JSON_TYPE_BYTES_SIZE) = newSize; }
        inline QVariant size() const                            { return attrs -> value(JSON_TYPE_BYTES_SIZE, -1); }

        inline void setDatatype(const DataSubType & dataType)   { attrs -> operator[](JSON_TYPE_DATA_SUB_TYPE) = dataType; }
        inline DataSubType dataType() const                     { return (DataSubType)attrs -> value(JSON_TYPE_DATA_SUB_TYPE, dt_none).toInt(); }

        void setStates(int flags) {
            ItemState::setStates(flags);
            int res = flags & 15;
            if (res > 0) setState(fieldStatePart(saveStates()));

        }
        inline void setState(const QVariant & newState)         { attrs -> operator[](JSON_TYPE_STATE) = newState; }
        inline int state() const                                { return attrs -> value(JSON_TYPE_STATE, flag_new_item).toInt(); }
//


        void openLocation();
        bool removePhysicalObject();
        bool isExist() const;

//        inline bool setShareable(const bool able)               { attrs[JSON_TYPE_IS_SHAREABLE] = able; }
        bool isShareable() const;

//        inline bool setRemote(const bool able)                  { attrs[JSON_TYPE_IS_REMOTE] = able; }
        bool isRemote() const;

        QString toUid();

        QString fullPath() const;
        QUrl toUrl() const;

        inline void setParted(const QVariant & isParted)        { attrs -> operator[](JSON_TYPE_PARTIAL) = isParted; }
        inline bool isParted() const                            { return attrs -> value(JSON_TYPE_PARTIAL, false).toBool(); }

        QVariantHash toHash();

        virtual void toJson(QJsonObject & obj);
        QVariantHash toInnerAttrs(int itemType) const; // remove later
    protected:
        QVariantHash * attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
