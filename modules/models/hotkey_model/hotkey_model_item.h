#ifndef HOTKEY_MODEL_ITEM_H
#define HOTKEY_MODEL_ITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class HotkeyModelItem {
public:
    HotkeyModelItem(const QVector<QVariant> & data, HotkeyModelItem * parent = 0);
    ~HotkeyModelItem();

    HotkeyModelItem * child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant & value);
    void appendChilds(QList<HotkeyModelItem *> *);

    HotkeyModelItem * parent();
    QList<HotkeyModelItem *> childList();

private:
    QList<HotkeyModelItem *> childItems;
    QVector<QVariant> itemData;
    HotkeyModelItem * parentItem;
};

#endif //HOTKEY_MODEL_ITEM_H
