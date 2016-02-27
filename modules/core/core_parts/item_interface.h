#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qstringbuilder.h>

#include "core_part_types.h"
#include "part_mixes/item_fields.h"

#include "modules/core/misc/file_utils/icon_provider.h"

#define REMOTE_DND_URL QUrl::fromLocalFile("REMOTE:/")

namespace Core {
    class Playlist;

    class IItem : public ItemFields {
    public:
        IItem(Playlist * parent = 0, int initState = DEFAULT_FILE_STATE);
        IItem(Playlist * parent, QVariantMap & hash, int pos = -1);
        IItem(Playlist * parent, QJsonObject * hash);
        IItem(Playlist * parent, const QString & title, int pos = -1, int initState = DEFAULT_FILE_STATE);

        inline virtual ~IItem() {}

        virtual QJsonObject toJson();

        virtual QString fullPath() const;

        void openLocation();
        virtual bool removePhysicalObject() = 0;
        virtual int itemType() const = 0;
        virtual bool isPartial() { return false; }

        inline int column() const { return 0; }
        int row() const;

        virtual inline int itemsCountInBranch() const { return 1; }

        inline virtual bool isRemote() const { return false; }
        inline virtual bool isContainer() const { return false; }
        inline virtual bool isShareable() const { return false; }

        virtual bool isExist() const = 0;
        inline bool isPlayable() const {
            bool showBatch = Settings::obj().isCheckboxShow();
            return !isContainer() && (!showBatch || (showBatch && is(checked)));
        }

        virtual QUrl toUrl() const { return QUrl::fromLocalFile(fullPath()); }
        QString buildTreePath() const;
        QString buildTreeStr() const; // used for tree sorting on removing

        virtual QVariant data(int column) const;
        bool setData(int column, const QVariant & value);

        inline Playlist * parent() { return _parent; }

        inline virtual int columnCount() const { return 1; }

        inline virtual void updateCheckedState(bool setChecked) {
            if (setChecked) set(checked); else unset(checked);
        }

        inline virtual bool updateCheckedStateByPredicate(ItemStateFlag pred_state) {
            bool valid = is(pred_state);
            updateCheckedState(valid);
            return valid;
        }

        void removeYouself();
        virtual void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);

//        inline bool respondTo(QString & predicate) {
////            QRegularExpression regex(predicate, QRegularExpression::CaseInsensitiveOption);
////            QRegularExpressionMatch m = regex.match(it.data().toString(), 0, QRegularExpression::PartialPreferFirstMatch);
////            return m.hasMatch();

//            return /*predicate.isEmpty() ||*/ title().toString().contains(predicate, Qt::CaseInsensitive);
//        }

        virtual void setParent(Playlist * pNode, int pos = -1);

        void addSource(QJsonObject * hash);
        void addSource(IItem * newSource, bool setAsMain = false);
        inline IItem * activeSourceItem() const {
            if (isContainer()) return const_cast<IItem *>(this);

            if (sources.isEmpty()) {
                IItem * self = const_cast<IItem *>(this);
                QJsonArray arr = const_cast<QVariantMap &>(attrs).take(JSON_TYPE_SOURCES).toJsonArray();
                if (arr.isEmpty()) // root node is always empty // so it shpuld be source for yourself
                    self -> addSource(self);
                else
                    for(QJsonArray::ConstIterator it = arr.constBegin(); it != arr.constEnd(); it++){
                        QJsonObject obj = (*it).toObject();
                        self -> addSource(&obj);
                    }
            }

            return sources[activeSource()];
        }

        inline void setTitle(const QVariant & newTitle)         { activeSourceItem() -> _setTitle(newTitle); }
        inline QVariant title() const                           { return activeSourceItem() -> _title(); }

        inline void setTitlesCache(const QVariant & newTitles)  { activeSourceItem() -> _setTitlesCache(newTitles); }
        inline QVariant titlesCache() const                     { return activeSourceItem() -> _titlesCache(); }

        inline void setVideoPath(const QVariant & videoPath)    { activeSourceItem() -> _setVideoPath(videoPath); }

        inline void setDuration(const QVariant & newDuration)   { activeSourceItem() -> _setDuration(newDuration); }
        inline QVariant duration() const                        { return activeSourceItem() -> _duration(); }
        inline quint64 durationMillis() const                   { return activeSourceItem() -> _durationMillis(); }

        // per source info
        inline void setUid(const QVariant & newId)              { activeSourceItem() -> _setUid(newId); }
        inline QVariant uid() const                             { return activeSourceItem() -> _uid(); }

        inline void setOwner(const QVariant & newOwner)         { activeSourceItem() -> _setOwner(newOwner); }
        inline QVariant owner() const                           { return activeSourceItem() -> _owner(); }

        inline void addArtistUid(const QString & id)            { activeSourceItem() -> _addArtistUid(id); }
        inline void setArtistUids(const QStringList & ids)      { activeSourceItem() -> _setArtistUids(ids); }
        inline QVariant artistUids() const                      { return activeSourceItem() -> _artistUids(); }

        inline void setSongUid(const QVariant & newSongId)      { activeSourceItem() -> _setSongUid(newSongId); }
        inline QVariant songUid() const                         { return activeSourceItem() -> _songUid(); }

        inline void setPath(const QVariant & newPath)           { activeSourceItem() -> _setPath(newPath); }
        inline QVariant path() const                            { return activeSourceItem() -> _path(); }

        inline void setRefreshPath(const QVariant & newPath)    { activeSourceItem() -> _setRefreshPath(newPath); }
        virtual inline QString refresh_path()                   { return activeSourceItem() -> _refresh_path(); }

        inline void setExtension(const QVariant & newExtension) { activeSourceItem() -> _setExtension(newExtension); }
        inline QVariant extension() const                       { return activeSourceItem() -> _extension(); }

        inline void setStartPos(const QVariant & newStartPos)   { activeSourceItem() -> _setStartPos(newStartPos); }
        inline QVariant startPos() const                        { return activeSourceItem() -> _startPos(); }
        inline qint64 startPosMillis() const                    { return activeSourceItem() -> _startPosMillis(); }

        inline void setInfo(const QVariant & newInfo)           { activeSourceItem() -> _setInfo(newInfo); }
        QStringList info() const                                { return activeSourceItem() -> _info(); }
        QVariant infoVar() const                                { return activeSourceItem() -> _infoVar(); }
        inline bool hasInfo() const                             { return activeSourceItem() -> _hasInfo();}

        inline void setSize(const QVariant & newSize)           { activeSourceItem() -> _setSize(newSize); }
        inline QVariant size() const                            { return activeSourceItem() -> _size(); }

        inline void setSubtype(Web::SubType subType)            { activeSourceItem() -> _setSubtype(subType); }
        inline Web::SubType subtipe() const                     { return activeSourceItem() -> _subtipe(); }
    protected:
        Playlist * _parent;
        QList<IItem *> sources;

        QString relationStr() const;
        QString errorStr(QString & key_name) const;
    };
}

#endif // ITEM_INTERFACE

