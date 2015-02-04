#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"
#include <qdir>
#include <qdatetime.h>

namespace Playo3 {
    class FolderItem : public ItemInterface {
    public:
        FolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QJsonObject * hash, FolderItem * parent = 0);
        FolderItem(QString folderPath, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        ~FolderItem();

        inline int itemsCountInBranch() const { return inBranchCount; }

        inline void openLocation() { QDesktopServices::openUrl(toUrl()); }
        bool removePhysicalObject();

        bool isExist() const;
        inline bool isContainer() const { return true; }

        QJsonObject toJson();

        inline int childRow(ItemInterface * child) { return children.indexOf(child); }
        inline ItemInterface * child(int row) {
            if (row >= children.size())  row = children.size() - 1; // return last item
            return row != -1 ? children.value(row) : 0;
        }
        virtual inline int childCount() const { return children.count(); }
        inline void declareChild(ItemInterface * child) { children.append(child); }
        inline void declareChildren(QList<ItemInterface *> & items) { children.append(items); }
        inline void declareChild(int pos, ItemInterface * item) { children.insert(pos, item); }
        bool removeChildren(int position, int count);

        void shuffle();

//        int childTreeCount() const;

        void dropExpandProceedFlags();
        void updateCheckedState(bool checked);

        FolderItem * createFolderPath(QString path);
        FolderItem * createFolder(QString name, QStringList * list = 0, int pos = -1);
        FolderItem * findNearestFolder(QStringList * list);
        inline void declareFolder(QString name, FolderItem * folder) { folders.insert(name, folder); }
        inline int undeclareFolder(QString name) { return folders.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
        inline FolderItem * folderItem(QString name) { return folders.value(name); }
    protected:
        QHash<QString, FolderItem *> folders;
        QList<ItemInterface *> children;
        int inBranchCount; // executable items count
    };
}

#endif // FOLDER_ITEM_H
