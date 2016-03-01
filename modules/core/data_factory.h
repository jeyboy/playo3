#ifndef DATA_FACTORY
#define DATA_FACTORY

#include <qhash.h>

#include "data_item.h"

namespace Core {
    class DataFactory : public QHash<QString, DataItem *> {
    public:
        inline virtual ~DataFactory() {
            qDeleteAll(*this);
            clear();
        }

        void registerSource(const QString & uid, const QString & source_uid, const QVariantHash & source_attrs) {
            if (!contains(source_uid))
                insert(source_uid, new DataItem(source_attrs, 0));
            else
                operator[](source_uid) -> iterateRelationsAmount();

            operator[](uid) -> addSource(source_uid);
        }
        void unregisterSource(const QString & uid) {
            DataItem * data = operator[](uid);
            data -> deiterateRelationsAmount();
            if (data -> relationsAmount() == 0)
                delete take(uid);
        }

        inline bool hasDataItem(const QString & uid) { return contains(uid); }

        DataItem * registerDataItem(const QString & uid, const QVariantHash & attrs = QVariantHash()) {
            if (contains(uid)) {
                operator[](uid) -> iterateRelationsAmount();
                return 0;
            } else {
                DataItem * newItem = new DataItem(attrs, 1);
                insert(uid, newItem);
                return newItem;
            }
        }
        void unregisterDataItem(const QString & uid) {
            DataItem * data = operator[](uid);

            if (data -> relationsAmount() == 1) {
                QStringList sources = data -> sources();
                for(QStringList::Iterator source = sources.begin(); source != sources.end(); source++)
                    unregisterSource(*source);

                delete take(uid);
            }
            else data -> deiterateRelationsAmount();
        }

        DataItem * dataItem(const QString & uid) { return value(uid, 0); }
    };
}

#endif // DATA_FACTORY
