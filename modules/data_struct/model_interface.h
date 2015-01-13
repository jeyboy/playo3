#ifndef MODEL_INTERFACE
#define MODEL_INTERFACE

#include <qabstractitemmodel.h>
#include <qvariant.h>
#include "item_interface.h"
#include <QFileInfo>

namespace Playo3 {
    class ModelInterface : public QAbstractItemModel {
        Q_OBJECT
    public:
        ModelInterface(const QStringList &headers, const QString &data, QObject * parent);
        ModelInterface(QJsonObject * hash, QObject * parent);
        ~ModelInterface();

        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

        bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
        bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    private:
        void setupModelData(const QStringList &lines, ItemInterface * parent);
        ItemInterface * getItem(const QModelIndex &index) const;

        ItemInterface * rootItem;
    };
}

#endif // MODEL_INTERFACE

