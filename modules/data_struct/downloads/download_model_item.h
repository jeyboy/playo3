#ifndef DOWNLOAD_MODEL_ITEM_H
#define DOWNLOAD_MODEL_ITEM_H

#include <qjsonobject.h>
#include <qjsonarray.h>

#include <QVariant>
#include <QVector>

#define DOWNLOAD_TITLE 0
#define DOWNLOAD_FROM 1
#define DOWNLOAD_TO 2
#define DOWNLOAD_IS_REMOTE 3
#define DOWNLOAD_TYPE 4
#define DOWNLOAD_ID 5
#define DOWNLOAD_ERROR 998
#define DOWNLOAD_PROGRESS 999

class DownloadModelItem {
public:
    DownloadModelItem(const QVariantMap & data, DownloadModelItem * parent = 0);
    DownloadModelItem(QJsonObject * data = 0, DownloadModelItem * parent = 0);
    ~DownloadModelItem();

    DownloadModelItem * child(int number);
    bool removeChildren(int position, int count);
    int childCount() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant & value);

    int row() const;
    inline int column() const { return 0; }
    inline int columnCount() const { return 1; }

    DownloadModelItem * parent();
    QList<DownloadModelItem *> childList();

    QJsonObject toJson();

private:
    QList<DownloadModelItem *> childItems;
    QVariantMap itemData;
    DownloadModelItem * parentItem;
};

#endif //HOTKEY_MODEL_ITEM_H
