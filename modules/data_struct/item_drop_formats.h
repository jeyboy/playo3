#ifndef ITEM_DROP_FORMATS
#define ITEM_DROP_FORMATS

#include <qabstractitemmodel.h>

#define DROP_OUTER_FORMAT "text/uri-list"
#define DROP_INNER_FORMAT "application/x-qabstractitemmodeldatalist"

struct InnerData {
    InnerData() {}

    QUrl url;
    QVariantMap attrs;
    QModelIndex eIndex;
    int eRow, dRow;
};

struct DropData {
    DropData() {}

    QModelIndex eIndex;
    int eRow, limitRow;
};

#endif // ITEM_DROP_FORMATS
