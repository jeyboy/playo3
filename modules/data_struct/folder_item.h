#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"
#include <qdir>
#include <qdatetime.h>

namespace Playo3 {
    class FolderItem : public IItem {
    public:
        FolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QJsonObject * hash, FolderItem * parent = 0);
        FolderItem(QString folderPath, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QString folderTitle, FolderItem * parent, QString uid, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        ~FolderItem();

        void accumulateUids(QHash<QVariant, IItem *> & store);
        QVariantList childrenUids(int position, int count);

        inline int itemsCountInBranch() const { return inBranchCount; }
        inline void updateItemsCountInBranch(int offset) { inBranchCount += offset; }
        void backPropagateItemsCountInBranch(int offset);

        QVariant data(int column) const;

        inline void openLocation() { QDesktopServices::openUrl(toUrl()); }
        inline int itemType() const { return PLAYLIST; }
        bool removePhysicalObject();

        bool isExist() const;
        inline bool isContainer() const { return true; }

        QJsonObject toJson();

        inline int childRow(IItem * child) { return children.indexOf(child); }
        inline IItem * child(int row) { return (row < 0 || row >= children.size()) ? 0 : children.value(row); }
        virtual inline int childCount() const { return children.count(); }
        inline void declareChild(IItem * child) { children.append(child); }
        inline void declareChildren(QList<IItem *> & items) { children.append(items); }
        inline void declareChild(int pos, IItem * item) { children.insert(pos, item); }
        int removeChildren(int position, int count);

        void shuffle();

        void propagateFolderSetFlag(ItemStateFlag flag);
        void propagateFolderUnsetFlag(ItemStateFlag flag);
        void propagateCheckedState(bool checked);

        FolderItem * createFolderPath(QString path);
        template<class T> T * createFolder(QString uid, QString name, int pos = -1);
        FolderItem * createFolder(QString name, QStringList * list = 0, int pos = -1);
        FolderItem * findNearestFolder(QStringList * list);
        inline void declareFolder(QString name, FolderItem * folder) { folders.insert(name, folder); }
        inline int undeclareFolder(QString name) { return folders.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
        inline FolderItem * folderItem(QString name) { return folders.value(name); }
    protected:
        inline QString folderUid() const { return folderUid(title().toString(), uid().toString()); }
        inline QString folderUid(QString name, QString uid) const { return name + "*" + uid; }

        QHash<QString, FolderItem *> folders;
        QList<IItem *> children;
        int inBranchCount; // executable items count
    };
}

#endif // FOLDER_ITEM_H
