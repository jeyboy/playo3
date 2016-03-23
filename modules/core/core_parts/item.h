#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>

#include "part_mixes/item_fields.h"

#include "modules/core/misc/file_utils/icon_provider.h"

namespace Core {
    class Playlist;

    class IItem : public ItemFields {
    public:
        IItem(Playlist * parent, QJsonObject * hash);
        IItem(Playlist * parent, QVariantMap & hash, int pos = -1);
        IItem(const DataSubType & subType, Playlist * parent = 0, int initState = DEFAULT_ITEM_STATE);
        IItem(const DataSubType & subType, Playlist * parent, const QString & title, int pos = -1, int initState = DEFAULT_ITEM_STATE);

        virtual ~IItem();

        virtual QJsonObject toJson() { return ItemFields::toJson(); }

        inline virtual int columnCount() const { return 1; }
        inline int column() const { return 0; }
        int row() const;

        virtual inline int itemsCountInBranch() const { return 1; }
        inline virtual bool isContainer() const { return false; }
        inline bool isPlayable() const {
            bool showBatch = Settings::obj().isCheckboxShow();
            return !isContainer() && (!showBatch || (showBatch && is(flag_checked)));
        }

        QString buildTreePath() const;
        QString buildTreeStr() const; // used for tree sorting on removing

        virtual QVariant data(int column) const;
        bool setData(int column, const QVariant & value);

        inline virtual void updateCheckedState(bool setChecked) {
            if (setChecked) set(flag_checked); else unset(flag_checked);
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

        inline Playlist * parent() { return _parent; }
        virtual void setParent(Playlist * pNode, int pos = -1);
    protected:
        Playlist * _parent;

        QString relationStr() const;
        QString errorStr(QString & key_name) const;
    };
}

#endif // ITEM_INTERFACE

