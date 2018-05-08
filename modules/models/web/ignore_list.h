#ifndef IGNORE_LIST_H
#define IGNORE_LIST_H

#include <qvariant.h>
#include <qjsonobject.h>

namespace Models {
    class IgnoreList : public QVariantHash {
        QVariantHash list;
        inline QString jsonName() const { return QLatin1String("deleted"); }
    public:
        IgnoreList(QJsonObject * obj = 0);
        inline virtual ~IgnoreList() {}

        void ignoreListAddUid(const QString & uid);
        void ignoreListAddUid(const QStringList & uids);
        void ignoreListRemoveUid(const QString & uid);
        bool ignoreListContainUid(const QString & uid);
        inline bool ignoreListIsEmpty() { return list.isEmpty(); }
        void ignoreListClear();

        QJsonObject & ignoreListToJson(QJsonObject &);
        void ignoreListFromJson(const QJsonObject &);
    };
}

#endif // IGNORE_LIST_H
