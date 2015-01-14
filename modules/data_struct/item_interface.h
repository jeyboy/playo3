#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qvariant.h>
#include <qvector.h>

//#include <qjsonobject.h>
//#include <qfile>
//#include <qdir>
//#include <qurl>
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
        explicit ItemInterface(const QVector<QVariant> &data, ItemInterface *parent = 0);
        ~ItemInterface();

        inline ItemInterface * child(int number) { return childItems.value(number); }
        inline int childCount() const { return childItems.count(); }
        inline int columnCount() const { return itemData.count(); }
        inline QVariant data(int column) const { return itemData.value(column); }
        inline ItemInterface * parent() { return parentItem; }

        bool insertChildren(int position, int count, int columns);
        bool insertColumns(int position, int columns);
        bool removeChildren(int position, int count);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        bool setData(int column, const QVariant &value);

    private:
        QList<ItemInterface *> childItems;
        QVector<QVariant> itemData;
        ItemInterface * parentItem;
    };

    //            inline static QString buildUid(QString owner, QString id) { return owner + "_" + id;}

    //            inline virtual bool isRemote() const { return false; }
    //            inline virtual bool isContainer() const { return false; }
    //            virtual bool isExist() const = 0;

    //            inline bool isPlayable() const {
    ////                bool showBatch = Settings::instance() -> isCheckboxShow();
    ////                return !isFolder() && (!showBatch || (showBatch && getState() -> isChecked()));
    //            }

    //            inline QString title() const { return fields -> title; }

    //            BaseItem(int initState = DEFAULT_MODEL_ITEM_STATE);
    //            BaseItem(QJsonObject * hash, BaseItem * parent = 0);
    //            BaseItem(const Fields * settings, BaseItem * parent = 0);
    //            virtual ~BaseItem();


    //            QString fullPath() const;
    //            QString downloadTitle() const;


    //            void openLocation();
    //            virtual bool removePhysicalObject() = 0;

    ////            inline bool hasInfo() const {return !Settings::instance() -> isShowInfo() || (Settings::instance() -> isShowInfo() && !info.isEmpty());}
    //            inline void setInfo(QString newInfo) {info = newInfo;}

    //            inline void setBpm(int newBeat) { bpm = newBeat; }
    //            inline void setDuration(QString newDuration) {duration = newDuration;}
    //            inline void setGenre(int newGenreID) {genreID = newGenreID;}
    //            inline void setPath(QString newPath) {path = newPath;}

    //            QStringList getInfo() const;

    //            void accumulateUids(QHash<BaseItem*, QString> & store);

    //            virtual QUrl toUrl();
    //            virtual QJsonObject toJSON();
    //            BaseItem * toBaseItem();
    //            QString toPath() const;
    //            virtual QString toUID() { return ""; }

    //            BaseItem * parent() const;

    ////            virtual BaseItem *child(int row);  // stub
    ////            virtual int childTreeCount() const;  // stub
    ////            virtual int childCount() const;  // stub
    ////            virtual void insertChild(int pos, BaseItem *item);  // stub
    ////            virtual void appendChild(BaseItem *child);  // stub
    ////            virtual bool removeChildren(int position, int count);  // stub

    //            int column() const;
    //            int columnCount() const;

    //            int row() const;

    //            QVariant data(int column) const;
    //            bool setData(int column, const QVariant &value);

    //            State *getState() const;
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

