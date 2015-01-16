#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"

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

        QJsonObject toJSON();

        inline ItemInterface * child(int row) { return childItems.value(row); }
        virtual inline int childCount() const { return childItems.count(); }
        inline void appendChild(ItemInterface * child) { childItems.append(child); }
        inline bool insertChildren(int pos, ItemInterface * item) { childItems.insert(pos, item); }
        inline bool insertChildren(int position, int count, int columns);
        bool removeChildren(int position, int count);

        void shuffle();

//        int childTreeCount() const;

        void dropExpandProceedFlags();
        void updateCheckedState(bool checked);

        inline void declareFolder(QString name, FolderItem * folder) { folders.insert(name, folder); }
        inline int undeclareFolder(QString name) { return folders.remove(name); }
    protected:
        QHash<QString, FolderItem *> folders;
        QList<ItemInterface *> childItems;
        int inBranchCount; // executable items count
    };
}

#endif // FOLDER_ITEM_H
