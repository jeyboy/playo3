#ifndef HOTKEY_MODEL_H
#define HOTKEY_MODEL_H

#include <qabstractitemmodel.h>

#include "hotkey_model_item.h"

class HotkeyModel : public QAbstractItemModel {
    Q_OBJECT
public:
    HotkeyModel(QList<HotkeyModelItem *> * toplevelchilds = 0, QObject * parent = 0);
    inline ~HotkeyModel() { delete rootItem; }

    QVariant data(const QModelIndex & index, int role) const;
    bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex & index) const;
    QModelIndex parent(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex index(HotkeyModelItem * item) const;

    int itemsCount() const;

    inline int columnCount(const QModelIndex & /* parent */) const { return rootItem -> columnCount(); }
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    HotkeyModelItem * getItem(const QModelIndex &index) const;
    inline HotkeyModelItem * root() const { return rootItem; }
    inline QList<HotkeyModelItem * > toplevelItems() { return root() -> childList(); }
protected:
    HotkeyModelItem * rootItem;
};

#endif // HOTKEY_MODEL_H
