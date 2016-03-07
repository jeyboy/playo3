#ifndef MODEL_ITEM_FIELDS_H
#define MODEL_ITEM_FIELDS_H

#include <qjsonobject.h>
#include <qurl.h>
#include <qdesktopservices.h>
#include <qstringbuilder.h>

#include "item_state.h"
#include "item_field_defines.h"
#include "json_fields.h"
#include "item_errors.h"
#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/data_sub_types.h"
#include "modules/core/misc/format.h"
#include "modules/core/web/apis/social/soundcloud_api.h"

namespace Core {
    class ItemFields : public ItemState {
    public:
        static inline QString toUid(const QVariant & owner, const QVariant & id) {
            return /*owner.isValid() &&*/ id.isValid() ? owner.toString() % QStringLiteral("_") % id.toString() : QString();
        }

        inline ItemFields() {}
        inline virtual ~ItemFields() { delete attrs; }
        inline void connectFields(ItemFields * newSource) { attrs = newSource -> attrs; }

        ItemFields(const QVariantHash & hash);
        ItemFields(QJsonObject * hash);
        ItemFields(const QString & title, int initState = DEFAULT_ITEM_STATE);
        ItemFields(int state = DEFAULT_ITEM_STATE);

        // const info
        inline void setGenreID(const QVariant & newGenreID)     { attrs -> operator[](JSON_TYPE_GENRE_ID) = newGenreID; }
        inline QVariant genreID() const                         { return attrs -> value(JSON_TYPE_GENRE_ID, -1); }

        inline void setBpm(const QVariant & newBeat)            { attrs -> operator[](JSON_TYPE_BPM) = newBeat; }
        inline QVariant bpm() const                             { return attrs -> value(JSON_TYPE_BPM, 0); }

        inline void setError(const QVariant & error)            {
            if (error.toInt() == err_none)
                attrs -> remove(JSON_TYPE_ERROR);
            else
                attrs -> operator[](JSON_TYPE_ERROR) = error;
        }
        inline QVariant error() const                           { return attrs -> value(JSON_TYPE_ERROR); }

        inline void setActiveSourceIndex(int index)             { attrs -> operator[](JSON_TYPE_ACTIVE_SOURCE) = index; }
        inline int activeSourceIndex() const                    { return attrs -> value(JSON_TYPE_ACTIVE_SOURCE, 0).toInt(); }

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


        void openLocation() {
            QFileInfo info(fullPath());
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
        }
        bool removePhysicalObject() {
            switch(dataType()) {
                case dt_local:
                    return QFile::remove(fullPath());
                case dt_local_cue: // this required on some additional checks
                default: return false;
            }


        }
        bool isExist() const {
            switch(dataType()) {
                case dt_local:
                case dt_local_cue:
                    return QFile::exists(fullPath());
                default: return true;
            }
        }

//        inline bool setShareable(const bool able)               { attrs[JSON_TYPE_IS_SHAREABLE] = able; }
        bool isShareable() const                         {
//            return attrs.value(JSON_TYPE_IS_SHAREABLE, false).toBool();
            switch(dataType()) {
                case dt_site_od:
                case dt_site_vk:
                case dt_site_sc:
                    return true;
                default: return false;
            }
        }

//        inline bool setRemote(const bool able)                  { attrs[JSON_TYPE_IS_REMOTE] = able; }
        bool isRemote() const                            {
//            return attrs.value(JSON_TYPE_IS_REMOTE, false).toBool();
            switch(dataType()) {
                case dt_local:
                case dt_local_cue:
                case dt_playlist_local:
                case dt_playlist_cue:
                    return false;
                default: return true;
            }
        }

        QString toUid() {
            switch(dataType()) {
                case dt_site_vk:
//            case dt_playlist_vk:
                    return toUid(owner(), id());
                case dt_site_sc:
                case dt_site_od:
                    return id().toString();

                default: return QString();
            }
        }

        QString fullPath() const {
            QString path_buff = path().toString();
            #ifdef Q_OS_LINUX
                path_buff = '/' % path_buff;
            #endif
            return path_buff;
        }

        QUrl toUrl() const {
            if (isRemote()) {
                QUrl url = QUrl(path().toString());
                switch(dataType()) {
                    case dt_site_sc: { url.setQuery(Web::Soundcloud::Api::obj().genDefaultParams()); break;}
                    default: ;
                }
                return url;
            } else
                return QUrl::fromLocalFile(fullPath());
        }

        inline bool isParted() const                            { return attrs -> value(JSON_TYPE_PARTIAL, false).toBool(); }

        QVariantHash toHash() {
            QVariantHash a(*attrs);
            a.insert(JSON_TYPE_STATE, saveStates());
            return a;
        }

        virtual QJsonObject toJson();
        QVariantHash toInnerAttrs(int itemType) const; // remove later
    protected:
        inline void setParted(const QVariant & isParted)        { attrs -> operator[](JSON_TYPE_PARTIAL) = isParted; }

        QVariantHash * attrs;
    };
}

#endif // MODEL_ITEM_FIELDS_H
