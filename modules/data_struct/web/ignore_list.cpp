#include "ignore_list.h"

using namespace Playo3;

IgnoreList::IgnoreList(Json::Obj * json) {
    if (json)
        ignoreListFromJson(json -> takeArr(jsonName()));
}

void IgnoreList::ignoreListAddUid(const QVariant & uid) {
    list.append(uid);
}

void IgnoreList::ignoreListAddUids(QVariantList & uids) {
    list.append(uids);
}

void IgnoreList::ignoreListRemoveUid(const QVariant & uid) {
    list.removeAll(uid);
}
bool IgnoreList::ignoreListContainUid(const QVariant & uid) {
    return list.contains(uid);
}

Json::Obj & IgnoreList::ignoreListToJson(Json::Obj & obj) {
    obj.insert(jsonName(), Json::Arr::fromVariantList(list));
    return obj;
}
void IgnoreList::ignoreListFromJson(const Json::Arr & arr) {
    list = arr.toVariantList();
}
