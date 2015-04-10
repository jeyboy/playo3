#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qdesktopservices.h>
#include <qurl>

////    #include "media/library.h"
////    #include "media/player.h"

#include "misc/file_utils/icon_provider.h"
#include "model_item_parts/item_types.h"
#include "model_item_parts/item_fields.h"

namespace Playo3 {
    class FolderItem;

    class IItem : public ItemFields {
    public:
        IItem(FolderItem * parent = 0, int initState = DEFAULT_MODEL_ITEM_STATE);
        IItem(FolderItem * parent, QVariantMap & hash, int pos = -1);
        IItem(FolderItem * parent, QJsonObject * hash);
        IItem(FolderItem * parent, QString title, int pos = -1, int initState = DEFAULT_MODEL_ITEM_STATE);

        virtual ~IItem();

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
            if (setChecked) set(checked); else unset(checked);
        }
    protected:
        FolderItem * _parent;
    };

    //            inline static QString buildUid(QString owner, QString id) { return owner + "_" + id;}
    //            void accumulateUids(QHash<BaseItem*, QString> & store);

    //            void setState(int new_state, bool append_to_library = true);

    //            bool cacheIsPrepared() const;
    //            void setCache(QList<QString> *newCache);
    //            void addToCache(QString title);
    //            QList<QString> * getTitlesCache() const;

    //        protected:

    //            QList<QString> * titlesCache;
}

#endif // ITEM_INTERFACE

