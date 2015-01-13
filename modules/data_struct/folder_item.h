#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "model/model_item.h"
#include "model/file_item.h"

class FolderItem : public ModelItem {
public:
    FolderItem(int initState = STATE_DEFAULT);
    FolderItem(QJsonObject * hash, ModelItem *parent = 0);
    FolderItem(const QString folderPath, QString folderTitle = "", ModelItem *parent = 0, int initState = STATE_UNPROCESSED | STATE_CHECKED);
    ~FolderItem();

    void openLocation();
    bool removePhysicalObject();

    bool isExist() const;
    bool isFolder() const;

    QJsonObject toJSON();

    ModelItem *child(int row);
    int childTreeCount() const;
    int childCount() const;
    void insertChild(int pos, ModelItem *item);
    void appendChild(ModelItem *child);
    bool removeChildren(int position, int count);

    void dropExpandProceedFlags();
    void changeCheckedState(bool checked);

    QHash<QString, ModelItem *> * foldersList() const;
    int removeFolder(QString name);
protected:
    QList<ModelItem *> * childItemsList();
    QHash<QString, ModelItem *> *folders;
    QList<ModelItem *> childItems;
};


#endif // FOLDER_ITEM_H
