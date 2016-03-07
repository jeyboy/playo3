#ifndef DATA_ITEM
#define DATA_ITEM

#include <qdebug.h>
#include <qvariant.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qstringbuilder.h>

#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/core_parts/part_mixes/item_fields.h"

#include "modules/core/misc/format.h"

namespace Core {
    class DataItem : public ItemFields {
        int relation_amount;
    public:
        DataItem(const QVariantHash & attrs, int rel_amount) : ItemFields(attrs), relation_amount(rel_amount) { }

        static DataItem * fromJson(const QJsonObject & obj) {
            QVariantHash hash = obj.toVariantHash();
            return new DataItem(hash, hash.take(JSON_TYPE_REL_AMOUNT).toInt());
        }

        void toJson(QJsonArray & arr) {
            QJsonObject obj = QJsonObject::fromVariantHash(*attrs);
            obj.insert(JSON_TYPE_REL_AMOUNT, relation_amount);
            arr << obj;
        }

        int relationsAmount() const { return relation_amount; }
        void iterateRelationsAmount() { relation_amount++; }
        void deiterateRelationsAmount() { relation_amount--; }
    };
}

#endif // DATA_ITEM
