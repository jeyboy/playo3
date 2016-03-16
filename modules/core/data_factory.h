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

        void fromJson(const QJsonArray & arr) {
            for(QJsonArray::ConstIterator it = arr.constBegin(); it != arr.constEnd(); it++) {
                DataItem * item = DataItem::fromJson((*it).toObject());
                insert(item -> databaseId(), item);
            }

            qDebug() << "IN BASE " << size();
        }

        void toJson(QJsonArray & arr) {
            for(QHash<QString, DataItem *>::Iterator it = this -> begin(); it != this -> end(); it++)
                (*it) -> toJson(arr);
        }

        void registerSource(const QString & source_uid, const QVariantHash & source_attrs) {
            if (!contains(source_uid))
                insert(source_uid, new DataItem(source_attrs, 0));
            else
                operator[](source_uid) -> iterateRelationsAmount();
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
                newItem -> setDatabaseId(uid);
                insert(uid, newItem);
                return newItem;
            }
        }
        void unregisterDataItem(const QString & uid, const QStringList & sources) {
            DataItem * data = operator[](uid);

            if (!data) {
                qDebug() << "data item missed";
                return;
            }

            if (data -> relationsAmount() == 1) {
                for(QStringList::ConstIterator source = sources.cbegin(); source != sources.cend(); source++)
                    unregisterSource(*source);

                delete take(uid);
            }
            else data -> deiterateRelationsAmount();
        }

        DataItem * dataItem(const QString & uid) { return value(uid, 0); }
    };
}

#endif // DATA_FACTORY
