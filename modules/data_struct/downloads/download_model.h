#ifndef DOWNLOAD_MODEL_H
#define DOWNLOAD_MODEL_H

#include <QAbstractItemModel>
#include "download_model_item.h"

class DownloadModel : public QAbstractItemModel {
    Q_OBJECT

public:
    DownloadModel(QObject * parent = 0);
    ~DownloadModel();

    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex & index) const;
    QModelIndex parent(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

    int itemsCount() const;

    int columnCount(const QModelIndex & parent = QModelIndex()) const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void appendRow(const QVector<QVariant> & data);

    void refreshItem(DownloadModelItem * item);

    DownloadModelItem * item(const QModelIndex & index) const;
    DownloadModelItem * root() const;
    QList<DownloadModelItem *> toplevelItems();

protected:
    DownloadModelItem * rootItem;
};

#endif // DOWNLOAD_MODEL_H
