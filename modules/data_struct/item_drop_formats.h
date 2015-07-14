#ifndef ITEM_DROP_FORMATS
#define ITEM_DROP_FORMATS

#include <qabstractitemmodel.h>
#include "modules/data_struct/model_item_parts/json_fields.h"

#define DROP_OUTER_FORMAT "text/uri-list"
#define DROP_INNER_FORMAT "application/x-qabstractitemmodeldatalist"

struct InnerData {
    QUrl url;
    QVariantMap attrs;
    QModelIndex eIndex;
    int eRow, dRow;
};

struct DropData {
    QModelIndex eIndex;
    int eRow, limitRow;
};

#endif // ITEM_DROP_FORMATS
