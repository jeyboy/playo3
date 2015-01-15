#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qvariant.h>
#include <qvector.h>
#include <qurl>

//#include <qjsonobject.h>
//#include <qfile>
//#include <qdir>
//#include <qdesktopservices.h>

//#include "misc/filename_conversions.h"

////    #include "model_item.h"
////    #include "media/library.h"
////    #include "media/player.h"
////    #include "misc/rand.h"
////    #include <algorithm>

#include "model_item_parts/item_types.h"
#include "model_item_parts/item_fields.h"

namespace Playo3 {
    class ItemInterface : public ItemFields {
    public:
        explicit ItemInterface(const QVector<QVariant> &data, ItemInterface * parent = 0);
        ItemInterface(ItemInterface * parent = 0, int initState = DEFAULT_MODEL_ITEM_STATE);
        ItemInterface(ItemInterface * parent, QJsonObject * hash);
        ItemInterface(ItemInterface * parent, QString path, QString title = "", QString extension = "", int size = -1);

        virtual ~ItemInterface();

        virtual bool removePhysicalObject() = 0;

        int column() const;
        int row() const;

        inline virtual bool isRemote() const { return false; }
        inline virtual bool isContainer() const { return false; }
        virtual bool isExist() const = 0;
        inline bool isPlayable() {
            bool showBatch = Settings::instance() -> isCheckboxShow();
            return !isContainer() && (!showBatch || (showBatch && is(checked)));
        }

        QString downloadTitle() const;
        QString fullPath() const;

        //            BaseItem * toBaseItem();
        inline QString toPath() const { return parentItem ? parentItem -> toPath() + " " + QString::number(row()) : ""; }

        inline QVariant data(int column) const { return itemData.value(column); }
        bool setData(int column, const QVariant &value);

        inline ItemInterface * parent() { return parentItem; }

        inline int columnCount() const { return itemData.count(); }
        bool insertColumns(int position, int columns);
        bool removeColumns(int position, int columns);

        inline ItemInterface * child(int number) { return childItems.value(number); }
        virtual inline int childCount() const { return childItems.count(); }
        bool insertChildren(int position, int count, int columns);
        bool removeChildren(int position, int count);
    private:
        QList<ItemInterface *> childItems;
        QVector<QVariant> itemData;
        ItemInterface * parentItem;
        int execCount; // executable items count
    };

    //            inline static QString buildUid(QString owner, QString id) { return owner + "_" + id;}

    //            inline bool isPlayable() const {
    ////                bool showBatch = Settings::instance() -> isCheckboxShow();
    ////                return !isFolder() && (!showBatch || (showBatch && getState() -> isChecked()));
    //            }

    //            void openLocation();
    //            virtual bool removePhysicalObject() = 0;

    //            void accumulateUids(QHash<BaseItem*, QString> & store);
    //            BaseItem * parent() const;

    ////            virtual BaseItem *child(int row);  // stub
    ////            virtual int childTreeCount() const;  // stub
    ////            virtual int childCount() const;  // stub
    ////            virtual void insertChild(int pos, BaseItem *item);  // stub
    ////            virtual void appendChild(BaseItem *child);  // stub
    ////            virtual bool removeChildren(int position, int count);  // stub

    //            void setState(int new_state, bool append_to_library = true);

    ////            virtual void dropExpandProceedFlags();  // stub
    ////            virtual void changeCheckedState(bool checked);  // stub

    //            bool cacheIsPrepared() const;
    //            void setCache(QList<QString> *newCache);
    //            void addToCache(QString title);
    //            QList<QString> * getTitlesCache() const;

    //            virtual QHash<QString, BaseItem *> * foldersList() const; // stub
    //            virtual int removeFolder(QString name);  // stub

    //            virtual QList<BaseItem *> * childItemsList(); // stub

    //            void shuffle();
    //        protected:

    //            QList<QString> * titlesCache;

    //            BaseItem * parentItem;
    //            Fields * fields;



    ////            State * state;

    ////            QString path;
    ////            QString title;
    ////            QString extension;

    ////            QString info;
    ////            QString duration;

    ////            qint16 bpm;
    ////            qint16 genreID;

    ////            int size;
}

#endif // ITEM_INTERFACE

