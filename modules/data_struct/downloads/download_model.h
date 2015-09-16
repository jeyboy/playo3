#ifndef DOWNLOAD_MODEL_H
#define DOWNLOAD_MODEL_H

#include <qabstractitemmodel.h>
#include <qsize.h>

#include "download_model_item.h"

class DownloadModel : public QAbstractItemModel {
    Q_OBJECT
public:
    inline DownloadModel(const Json::Obj & hash = Json::Obj(), QObject * parent = 0) : QAbstractItemModel(parent) { rootItem = new DownloadModelItem(hash); }
    inline ~DownloadModel() { delete rootItem; }

    inline Json::Obj toJson() { return rootItem -> toJson(); }

    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex & index) const;
    QModelIndex parent(const QModelIndex & index) const;
    QModelIndex index(DownloadModelItem * node) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    int itemsCount() const;

    inline int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const { return rootItem -> columnCount(); }

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QModelIndex appendRow(const QVariantMap & data);
    bool removeRows(int position, int rows, const QModelIndex & parent);
    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count,
                          const QModelIndex & destinationParent, int destinationChild);

    DownloadModelItem * item(const QModelIndex & index) const;
    DownloadModelItem * root() const;
    QList<DownloadModelItem *> toplevelItems();

protected:
    DownloadModelItem * rootItem;
};

#endif // DOWNLOAD_MODEL_H
