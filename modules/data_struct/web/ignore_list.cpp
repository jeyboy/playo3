#include "ignore_list.h"

using namespace Playo3;

IgnoreList::IgnoreList(Json::Obj & obj) {
    if (obj)
        ignoreListFromJson(obj -> take(jsonName()).toArray());
}

void IgnoreList::ignoreListAddUid(QVariant uid) {
    list.append(uid);
}

void IgnoreList::ignoreListAddUids(QVariantList & uids) {
    list.append(uids);
}

void IgnoreList::ignoreListRemoveUid(QVariant uid) {
    list.removeAll(uid);
}
bool IgnoreList::ignoreListContainUid(QVariant uid) {
    return list.contains(uid);
}

Json::Obj & IgnoreList::ignoreListToJson(Json::Obj & obj) {
    obj.insert(jsonName(), Json::Arr::fromVariantList(list));
    return obj;
}
void IgnoreList::ignoreListFromJson(Json::Arr & arr) {
    list = arr.toVariantList();
}
