#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qvariant.h>
#include <qvector.h>
#include <qdesktopservices.h>
#include <qurl>

////    #include "media/library.h"
////    #include "media/player.h"

#include "model_item_parts/item_types.h"
#include "model_item_parts/item_fields.h"

namespace Playo3 {
    class ItemInterface : public ItemFields {
    public:
        ItemInterface(ItemInterface * parent = 0, int initState = DEFAULT_MODEL_ITEM_STATE);
        ItemInterface(ItemInterface * parent, QJsonObject * hash);
        ItemInterface(ItemInterface * parent, QString path, QString title = "", QString extension = "", int size = -1, int initState = DEFAULT_MODEL_ITEM_STATE);

        virtual ~ItemInterface();

        QString fullPath() const;

        void openLocation();
        virtual bool removePhysicalObject() = 0;

        int column() const;
        int row() const;

        inline int itemsCountInBranch() const { return 0; }

        inline virtual bool isRemote() const { return false; }
        inline virtual bool isContainer() const { return false; }
        virtual bool isExist() const = 0;
        inline bool isPlayable() {
            bool showBatch = Settings::instance() -> isCheckboxShow();
            return !isContainer() && (!showBatch || (showBatch && is(checked)));
        }

        inline virtual QUrl toUrl() { return QUrl::fromLocalFile(fullPath()); }
        inline QString buildTreePath() const { return parentItem ? parentItem -> buildTreePath() + " " + QString::number(row()) : ""; }

        inline QVariant data(int column) const { return itemData.value(column); }
        bool setData(int column, const QVariant & value);

        inline ItemInterface * parent() { return parentItem; }

        inline int columnCount() const { return itemData.count(); }
//        bool insertColumns(int position, int columns);
//        bool removeColumns(int position, int columns);

        inline virtual void updateCheckedState(bool setChecked) {
            if (setChecked) set(checked); else unset(checked);
        }
    private:
        QVector<QVariant> itemData;
        ItemInterface * parentItem;
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

