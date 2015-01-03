#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qvariant.h>
#include <qvector.h>

#include "../item_types.h"
#include "../item_fields.h"

namespace Playo3 {
    class ItemInterface : public ItemFields {
    public:
        explicit ItemInterface(const QVector<QVariant> &data, ItemInterface *parent = 0);
        ~ItemInterface();

        inline ItemInterface * child(int number) { return childItems.value(number); }
        inline int childCount() const { return childItems.count(); }
        inline int columnCount() const { return itemData.count(); }
        inline QVariant data(int column) const { return itemData.value(column); }
        inline ItemInterface * parent() { return parentItem; }

        bool insertChildren(int position, int count, int columns);
        bool insertColumns(int position, int columns);
        bool removeChildren(int position, int count);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        bool setData(int column, const QVariant &value);

    private:
        QList<ItemInterface *> childItems;
        QVector<QVariant> itemData;
        ItemInterface * parentItem;
    };
}

#endif // ITEM_INTERFACE

