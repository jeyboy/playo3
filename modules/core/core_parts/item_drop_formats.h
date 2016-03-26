#ifndef ITEM_DROP_FORMATS
#define ITEM_DROP_FORMATS

#include <qabstractitemmodel.h>
#include <qurl.h>
//#include "modules/core/core_parts/part_mixes/json_fields.h"

#define DROP_OUTER_FORMAT "text/uri-list"
#define DROP_INNER_FORMAT "application/x-qabstractitemmodeldatalist"

struct InnerData {
    QUrl url;
    QVariantHash attrs;
    QModelIndex eIndex;
    int eRow, dRow;
};

struct DropData {
    QModelIndex eIndex;
    int eRow, limitRow;
};

#endif // ITEM_DROP_FORMATS
