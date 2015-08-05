#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qstringbuilder.h>
#include <qdebug.h>

#include "misc/file_utils/icon_provider.h"
#include "model_item_parts/item_types.h"
#include "model_item_parts/item_fields.h"

#define REMOTE_DND_URL QUrl::fromLocalFile("REMOTE:/")

namespace Playo3 {
    class FolderItem;

    class IItem : public ItemFields {
    public:       
        IItem(FolderItem * parent = 0, int initState = DEFAULT_MODEL_ITEM_STATE);
        IItem(FolderItem * parent, QVariantMap & hash, int pos = -1);
        IItem(FolderItem * parent, QJsonObject * hash);
        IItem(FolderItem * parent, QString title, int pos = -1, int initState = DEFAULT_MODEL_ITEM_STATE);

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

        virtual bool isExist() const = 0;
        inline bool isPlayable() const {
            bool showBatch = Settings::instance() -> isCheckboxShow();
            return !isContainer() && (!showBatch || (showBatch && is(checked)));
        }

        virtual QUrl toUrl() const { return QUrl::fromLocalFile(fullPath()); }
        QString buildTreePath() const;
        QString buildTreeStr() const; // used for tree sorting on removing

        virtual QVariant data(int column) const;
        bool setData(int column, const QVariant & value);

        inline FolderItem * parent() { return _parent; }

        inline virtual int columnCount() const { return 1; }

        inline virtual void updateCheckedState(bool setChecked) {
            qDebug() << "CHECK" << setChecked << title();
            if (setChecked) set(checked); else unset(checked);
        }

        inline virtual void updateCheckedStateByPredicate(ItemStateFlag pred_state) {
            updateCheckedState(is(pred_state));
        }

        virtual void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);

        inline bool respondTo(QString & predicate) {
//            QRegularExpression regex(predicate, QRegularExpression::CaseInsensitiveOption);
//            QRegularExpressionMatch m = regex.match(it.data().toString(), 0, QRegularExpression::PartialPreferFirstMatch);
//            return m.hasMatch();

            return /*predicate.isEmpty() ||*/ title().toString().contains(predicate, Qt::CaseInsensitive);
        }

        inline void setParent(FolderItem * pNode) {
//            if (_parent != 0) qDebug() << "Parent already setted";
            _parent = pNode;
        }
    protected:
        FolderItem * _parent;
    };
}

#endif // ITEM_INTERFACE

