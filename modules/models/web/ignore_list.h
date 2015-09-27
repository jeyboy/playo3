#ifndef IGNORE_LIST_H
#define IGNORE_LIST_H

//#include <qvariant.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

namespace Models {
    class IgnoreList {
    public:
        IgnoreList(QJsonObject * obj = 0);
        inline virtual ~IgnoreList() {}

        void ignoreListAddUid(const QVariant & uid);
        void ignoreListAddUids(QVariantList & uids);
        void ignoreListRemoveUid(const QVariant & uid);
        bool ignoreListContainUid(const QVariant & uid);

        QJsonObject & ignoreListToJson(QJsonObject &);
        void ignoreListFromJson(const QJsonArray &);
    private:
        inline QString jsonName() const { return QStringLiteral("deleted"); }

        QVariantList list;
    };
}

#endif // IGNORE_LIST_H
