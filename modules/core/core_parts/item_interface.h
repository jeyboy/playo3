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
        IItem(Playlist * parent, QString title, int pos = -1, int initState = DEFAULT_FILE_STATE);

        inline virtual ~IItem() {}

        virtual QJsonObject toJson();

        virtual QString fullPath() const;

        void openLocation();
        virtual bool removePhysicalObject() = 0;
        virtual int itemType() const = 0;

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
    protected:
        Playlist * _parent;
    };
}

#endif // ITEM_INTERFACE

