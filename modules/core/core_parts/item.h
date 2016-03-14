#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>

#include "part_mixes/item_fields.h"

#include "modules/core/web/apis/social/soundcloud_api.h"
#include "modules/core/misc/file_utils/icon_provider.h"

#define REMOTE_DND_URL QUrl::fromLocalFile("REMOTE:/")

namespace Core {
    class DataItem;
    class Playlist;

    class IItem : public ItemFields {
    public:
        IItem(Playlist * parent = 0, int initState = DEFAULT_ITEM_STATE);
        IItem(Playlist * parent, QVariantHash & hash, int pos = -1);
        IItem(Playlist * parent, QJsonObject * hash);
        IItem(Playlist * parent, const QString & title, int pos = -1, int initState = DEFAULT_ITEM_STATE);
        IItem(const QString & sourceUid, Playlist * parent, int pos = -1);

        virtual ~IItem();

        virtual void toJson(QJsonObject & obj);

        inline int column() const { return 0; }
        int row() const;

        inline virtual int itemsCountInBranch() const { return 1; }
        inline virtual bool isContainer() const { return false; }

        inline bool isPlayable() const {
            if (isContainer()) return false;

            bool showBatch = Settings::obj().isCheckboxShow();
            return !showBatch || (showBatch && has(flag_checked));
        }

        QString buildTreePath() const;
        QString buildTreeStr() const; // used for tree sorting on removing

        virtual QVariant data(int column) const;
        bool setData(int column, const QVariant & value);

        inline Playlist * parent() { return _parent; }

        inline virtual int columnCount() const { return 1; }

        inline virtual void updateCheckedState(bool setChecked) {
            if (setChecked) set(flag_checked); else unset(flag_checked);
        }

        inline virtual bool updateCheckedStateByPredicate(ItemStateFlag pred_state) {
            bool valid = has(pred_state);
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

        inline QStringList sources() const { return _sources; }
        inline void fixSourceLimit() { activeSourceIndexLimit = activeSourceIndex; }
        bool addSource(const QString & sourceUid, bool setAsMain = false, bool checkExistance = true);
        bool useNextSource();
    protected:
        // this should be checked on global source registration
//        bool eqlByLocation(IItem * itm) { return path() == itm -> path() && refresh_path() == itm -> refresh_path(); }

        int activeSourceIndex, activeSourceIndexLimit; // maybe this should be in ItemFields ?
        Playlist * _parent;
        QStringList _sources; // maybe this should be in ItemFields ?

        QString relationStr() const;
        QString errorStr(QString & key_name) const;
    };
}

#endif // ITEM_INTERFACE

