#ifndef IGNORE_LIST_H
#define IGNORE_LIST_H

#include <qvariant.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

namespace Playo3 {
    class IgnoreList {
    public:
        IgnoreList(QJsonObject * obj = 0);
        inline virtual ~IgnoreList() {}

        void ignoreListAddUid(QVariant uid);
        void ignoreListAddUids(QVariantList & uids);
        void ignoreListRemoveUid(QVariant uid);
        bool ignoreListContainUid(QVariant uid);

        QJsonObject & ignoreListToJson(QJsonObject & obj);
        void ignoreListFromJson(QJsonArray);
    private:
        inline QString jsonName() const { return QStringLiteral("deleted"); }

        QVariantList list;
    };
}

#endif // IGNORE_LIST_H
