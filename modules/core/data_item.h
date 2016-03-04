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
#include "modules/core/core_parts/item_interface.h"

#include "modules/core/misc/format.h"

namespace Core {
    class DataItem : public IItem {
//        QVariantHash attrs;
        int relation_amount;
//        int activeSourceIndexLimit;
    public:
        DataItem(const QVariantHash & attrs, int rel_amount)
            : IItem(0), relation_amount(rel_amount) {
//            activeSourceIndexLimit = attrs.value(JSON_TYPE_ACTIVE_SOURCE, 0).toInt();
        }

        virtual ~DataItem() {}

        static DataItem * fromJson(const QJsonObject & obj) {
//            int rel_amount = const_cast<QJsonObject &>(obj).take(JSON_TYPE_REL_AMOUNT).toInt();
//            return new DataItem(obj.toVariantHash(), rel_amount);
        }

        void toJson(QJsonArray & arr) {
//            QJsonObject obj = QJsonObject::fromVariantHash(attrs);
//            obj.insert(JSON_TYPE_REL_AMOUNT, relation_amount);
//            arr << obj;
        }

//        void fixSourceLimit() { activeSourceIndexLimit = activeSourceIndex(); }
//        bool addSource(const QString & uid, bool useIt = false) {
//            QStringList uids = attrs[JSON_TYPE_SOURCES].toStringList();
//            for(QStringList::Iterator source = uids.begin(); source != uids.end(); source++)
//                if (*source == uid) return false;

//            attrs[JSON_TYPE_SOURCES] = uids << uid;
//            if (useIt) setActiveSourceIndex(uids.length() - 1);

//            return true;
//        }
//        QString nextSource() { return sources()[activeSourceIndex()]; }
//        bool useNextSource() {
//            int sourcesAmount = sources().length();
//            if (sourcesAmount == 1) return false;

//            int currSourceIndex = activeSourceIndex() + 1;
//            if (currSourceIndex >= sourcesAmount)
//                currSourceIndex = 0;

//            setActiveSourceIndex(currSourceIndex);
//            return currSourceIndex != activeSourceIndexLimit;
//        }
//        inline QStringList sources() const { return attrs.value(JSON_TYPE_SOURCES).toStringList(); }


//        int relationsAmount() const { return relation_amount; }
//        void iterateRelationsAmount() { relation_amount++; }
//        void deiterateRelationsAmount() { relation_amount--; }

//        inline void setActiveSourceIndex(int index)             { attrs[JSON_TYPE_ACTIVE_SOURCE] = index; }
//        inline int activeSourceIndex() const                    { return attrs.value(JSON_TYPE_ACTIVE_SOURCE, 0).toInt(); }
    };
}

#endif // DATA_ITEM
