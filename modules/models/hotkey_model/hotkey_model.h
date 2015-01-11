#ifndef HOTKEY_MODEL_H
#define HOTKEY_MODEL_H

#include <QAbstractItemModel>
#include "hotkey_model_item.h"

class HotkeyModel : public QAbstractItemModel {
    Q_OBJECT

public:
    HotkeyModel(QList<HotkeyModelItem *> * toplevelchilds = 0, QObject *parent = 0);
    ~HotkeyModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(HotkeyModelItem * item) const;

    int itemsCount() const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void appendRow(HotkeyModelItem * item);

    void refreshItem(HotkeyModelItem * item);

    HotkeyModelItem *getItem(const QModelIndex &index) const;
    HotkeyModelItem * root() const;
    QList<HotkeyModelItem *> toplevelItems();

protected:
    HotkeyModelItem * rootItem;
};

#endif // HOTKEY_MODEL_H
