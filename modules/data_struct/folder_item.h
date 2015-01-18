#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"
#include <qdir>
#include <qdatetime.h>

namespace Playo3 {
    class FolderItem : public ItemInterface {
    public:
        FolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QJsonObject * hash, ItemInterface * parent = 0);
        FolderItem(const QString folderPath, QString folderTitle = "", ItemInterface * parent = 0, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        ~FolderItem();

        inline int itemsCountInBranch() const { return inBranchCount; }

        inline void openLocation() { QDesktopServices::openUrl(toUrl()); }
        bool removePhysicalObject();

        bool isExist() const;
        inline bool isContainer() const { return true; }

        QJsonObject toJson();

        inline int childRow(ItemInterface * child) { return childItems.indexOf(child); }
        inline ItemInterface * child(int row) { return childItems.value(row); }
        virtual inline int childCount() const { return childItems.count(); }
        inline void declareChild(ItemInterface * child) { childItems.append(child); } // parent not setted here
        inline void declareChildren(QList<ItemInterface *> & items) { childItems.append(items); }
        inline void declareChild(int pos, ItemInterface * item) { childItems.insert(pos, item); }
//        inline bool insertChildren(int position, int count, int columns);
        bool removeChildren(int position, int count);

        void shuffle();

//        int childTreeCount() const;

        void dropExpandProceedFlags();
        void updateCheckedState(bool checked);

        FolderItem * createFolderPath(QString path);
        FolderItem * createFolder(QString name, QStringList * list = 0);
        inline void declareFolder(QString name, FolderItem * folder) { folders.insert(name, folder); }
        inline int undeclareFolder(QString name) { return folders.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
//        inline FolderItem * folderItem(QString name) { return folders.value(name); }
    protected:
        QHash<QString, FolderItem *> folders;
        QList<ItemInterface *> childItems;
        int inBranchCount; // executable items count
    };
}

#endif // FOLDER_ITEM_H
