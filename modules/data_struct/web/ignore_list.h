#ifndef IGNORE_LIST_H
#define IGNORE_LIST_H

#include <QVariantList>
#include <QJsonObject>
#include <QJsonArray>

namespace Playo3 {
    class IgnoreList {
    public:
        IgnoreList(QJsonObject * obj = 0);
        ~IgnoreList();

        void ignoreListAddUid(QVariant uid);
        void ignoreListAddUids(QVariantList & uids);
        void ignoreListRemoveUid(QVariant uid);
        bool ignoreListContainUid(QVariant uid);

        QJsonObject & ignoreListToJson(QJsonObject & obj);
        void ignoreListFromJson(QJsonArray);
    private:
        inline QString jsonName() const { return "deleted"; }

        QVariantList list;
    };
}

#endif // IGNORE_LIST_H
