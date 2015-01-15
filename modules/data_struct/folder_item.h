#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"

namespace Playo3 {
    class FolderItem : public ItemInterface {
    public:
        FolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE);
        FolderItem(QJsonObject * hash, FolderItem * parent = 0);
        FolderItem(const QString folderPath, QString folderTitle = "", FolderItem * parent = 0, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        ~FolderItem();

        void openLocation();
        bool removePhysicalObject();

        bool isExist() const;
        bool isContainer() const;

        QJsonObject toJSON();

        ItemInterface * child(int row);
        int childTreeCount() const;
        int childCount() const;
        void insertChild(int pos, ModelItem *item);
        void appendChild(ModelItem *child);
        bool removeChildren(int position, int count);

        void dropExpandProceedFlags();
        void changeCheckedState(bool checked);

        QHash<QString, FolderItem *> * foldersList() const;
        int removeFolder(QString name);
    protected:
        QList<ItemInterface *> * childItemsList();
        QHash<QString, ItemInterface *> folders;
        QList<ItemInterface *> childItems;
    };
}

#endif // FOLDER_ITEM_H
