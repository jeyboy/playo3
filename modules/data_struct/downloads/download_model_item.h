#ifndef DOWNLOAD_MODEL_ITEM_H
#define DOWNLOAD_MODEL_ITEM_H

#include <QVariant>
#include <QVector>

class DownloadModelItem {
public:
    DownloadModelItem(const QVector<QVariant> & data, DownloadModelItem * parent = 0);
    ~DownloadModelItem();

    DownloadModelItem * child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant & value);
    void appendChilds(QList<DownloadModelItem *> *);

    DownloadModelItem * parent();
    QList<DownloadModelItem *> childList();

private:
    QList<DownloadModelItem *> childItems;
    QVector<QVariant> itemData;
    DownloadModelItem * parentItem;
};

#endif //HOTKEY_MODEL_ITEM_H
