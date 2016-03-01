#ifndef DATA_ITEM
#define DATA_ITEM

#include <qdebug.h>
#include <qvariant.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qstringbuilder.h>

#include "data_item_errors.h"
#include "settings.h"
#include "modules/core/web/web_sub_types.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/core_parts/part_mixes/item_state.h"
#include "modules/core/core_parts/part_mixes/item_fields.h"

#include "modules/core/misc/format.h"

namespace Core {
    class DataItem : public ItemState {
        QVariantHash attrs;
        int relation_amount;
        int activeSourceIndexLimit;
    public:
        DataItem(const QVariantHash & attrs, int rel_amount)
            : ItemState(attrs.contains(JSON_TYPE_STATE) ? const_cast<QVariantHash &>(attrs).take(JSON_TYPE_STATE).toInt() : DEFAULT_ITEM_STATE),
              attrs(attrs), relation_amount(rel_amount) {
            activeSourceIndexLimit = attrs.value(JSON_TYPE_ACTIVE_SOURCE, 0).toInt();
        }

        virtual ~DataItem() {}

        static DataItem * fromJson(const QJsonObject & obj) {
            int rel_amount = const_cast<QJsonObject &>(obj).take(JSON_TYPE_REL_AMOUNT).toInt();
            return new DataItem(obj.toVariantHash(), rel_amount);
        }

        void toJson(QJsonArray & arr) {
            QJsonObject obj = QJsonObject::fromVariantHash(attrs);
            obj.insert(JSON_TYPE_REL_AMOUNT, relation_amount);
            arr << obj;
        }

        void fixSourceLimit() { activeSourceIndexLimit = activeSourceIndex(); }
        bool addSource(const QString & uid, bool useIt = false) {
            QStringList uids = attrs[JSON_TYPE_SOURCES].toStringList();
            for(QStringList::Iterator source = uids.begin(); source != uids.end(); source++)
                if (*source == uid) return false;

            attrs[JSON_TYPE_SOURCES] = uids << uid;
            if (useIt) setActiveSourceIndex(uids.length() - 1);

            return true;
        }
        QString nextSource() { return sources()[activeSourceIndex()]; }
        bool useNextSource() {
            int sourcesAmount = sources().length();
            if (sourcesAmount == 1) return false;

            int currSourceIndex = activeSourceIndex() + 1;
            if (currSourceIndex >= sourcesAmount)
                currSourceIndex = 0;

            setActiveSourceIndex(currSourceIndex);
            return currSourceIndex != activeSourceIndexLimit;
        }
        inline QStringList sources() const { return attrs.value(JSON_TYPE_SOURCES).toStringList(); }


        int relationsAmount() const { return relation_amount; }
        void iterateRelationsAmount() { relation_amount++; }
        void deiterateRelationsAmount() { relation_amount--; }

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

        inline void setParted(const QVariant & isParted)        { attrs[JSON_TYPE_PARTIAL] = isParted; }
        inline bool isParted() const                            { return attrs.value(JSON_TYPE_PARTIAL, false).toBool(); }


        inline void setTitle(const QVariant & newTitle)         { attrs[JSON_TYPE_TITLE] = newTitle; }
        inline QVariant title() const                           { return attrs.value(JSON_TYPE_TITLE); }

        inline void setTitlesCache(const QVariant & newTitles)  { attrs[JSON_TYPE_TITLE_CACHES] = newTitles; }
        inline QVariant titlesCache() const                     { return attrs.value(JSON_TYPE_TITLE_CACHES); }

        inline void setVideoPath(const QVariant & videoPath)    { attrs[JSON_TYPE_VIDEO_PATH] = videoPath; } // maybe need move to sources if source site is supported ?

        inline void setDuration(const QVariant & newDuration)   { attrs[JSON_TYPE_DURATION] = newDuration; }
        inline QVariant duration() const                        { return attrs.value(JSON_TYPE_DURATION); }
        inline quint64 durationMillis() const                   { return attrs.value(JSON_TYPE_DURATION).isValid() ? Duration::toMillis(attrs.value(JSON_TYPE_DURATION).toString()) : 0; }

        inline void setId(const QVariant & newId)               { attrs[JSON_TYPE_ID] = newId; }
        inline QVariant id() const                              { return attrs.value(JSON_TYPE_ID); }

        inline void setOwner(const QVariant & newOwner)         { attrs[JSON_TYPE_OWNER_ID] = newOwner; }
        inline QVariant owner() const                           { return attrs.value(JSON_TYPE_OWNER_ID); }

        inline void addArtistUid(const QString & id)            { attrs[JSON_TYPE_ARTIST_UIDS] = attrs[JSON_TYPE_ARTIST_UIDS].toStringList() << id; }
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
        QStringList info() const {
            QStringList list;
            QVariant i = infoVar();

            if (!i.isValid())
                list.append(QStringLiteral("Wait on proc..."));
            else
                list.append(i.toString());
            list.append(duration().toString());

            return list;
        }
        inline QVariant infoVar() const                         { return attrs.value(JSON_TYPE_INFO); }
        inline bool hasInfo() const                             { return !Settings::obj().isShowInfo() || (Settings::obj().isShowInfo() && infoVar().isValid()); }

        inline void setSize(const QVariant & newSize)           { attrs[JSON_TYPE_BYTES_SIZE] = newSize; }
        inline QVariant size() const                            { return attrs.value(JSON_TYPE_BYTES_SIZE, -1); }

        inline void setSubtype(Web::SubType subType)            { attrs[JSON_TYPE_SUB_TYPE] = subType; }
        inline Web::SubType subtipe() const                     { return (Web::SubType)attrs.value(JSON_TYPE_SUB_TYPE, Web::site_none).toInt(); }

//        inline QString toUid()                                  { return attrs[JSON_TYPE_SUB_TYPE].toString(); }
//        QVariantMap ItemFields::toInnerAttrs(int itemType) const {
//            QVariantMap a(attrs);
//            a.insert(JSON_TYPE_STATE, saveStates());
//            a.insert(JSON_TYPE_ITEM_TYPE, itemType);
//            return a;
//        }
    };
}

#endif // DATA_ITEM
