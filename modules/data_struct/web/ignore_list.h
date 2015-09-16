#ifndef IGNORE_LIST_H
#define IGNORE_LIST_H

#include "misc/web_utils/json.h"

namespace Playo3 {
    class IgnoreList {
    public:
        IgnoreList(const Json::Obj & obj = Json::Obj());
        inline virtual ~IgnoreList() {}

        void ignoreListAddUid(QVariant uid);
        void ignoreListAddUids(QVariantList & uids);
        void ignoreListRemoveUid(QVariant uid);
        bool ignoreListContainUid(QVariant uid);

        Json::Obj & ignoreListToJson(Json::Obj & obj);
        void ignoreListFromJson(Json::Arr & arr);
    private:
        inline QString jsonName() const { return QStringLiteral("deleted"); }

        QVariantList list;
    };
}

#endif // IGNORE_LIST_H
