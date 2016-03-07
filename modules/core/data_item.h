#ifndef DATA_ITEM
#define DATA_ITEM

#include <qdebug.h>
#include <qvariant.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qstringbuilder.h>

#include "data_item_errors.h"
#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/core_parts/part_mixes/item_fields.h"

#include "modules/core/misc/format.h"

namespace Core {
    class DataItem : public ItemFields {
        int relation_amount;
    public:
        DataItem(const QVariantHash & attrs, int rel_amount) : ItemFields(attrs), relation_amount(rel_amount) { }
        virtual ~DataItem() {}

        static DataItem * fromJson(const QJsonObject & obj) {
            int rel_amount = const_cast<QJsonObject &>(obj).take(JSON_TYPE_REL_AMOUNT).toInt();
            return new DataItem(obj.toVariantHash(), rel_amount);
        }

        void toJson(QJsonArray & arr) {
            QJsonObject obj = QJsonObject::fromVariantHash(attrs);
            obj.insert(JSON_TYPE_REL_AMOUNT, relation_amount);
            arr << obj;
        }

        int relationsAmount() const { return relation_amount; }
        void iterateRelationsAmount() { relation_amount++; }
        void deiterateRelationsAmount() { relation_amount--; }
    };
}

#endif // DATA_ITEM
