#include "ignore_list.h"
#include <qdebug.h>

using namespace Playo3;

IgnoreList::IgnoreList(QJsonObject * obj) {
    if (obj)
        ignoreListFromJson(obj -> take(jsonName()).toArray());
}

IgnoreList::~IgnoreList() {}

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
    qDebug() << "IN IGNORE";
    return list.contains(uid);
}

QJsonObject & IgnoreList::ignoreListToJson(QJsonObject & obj) {
    obj.insert(jsonName(), QJsonArray::fromVariantList(list));
    return obj;
}
void IgnoreList::ignoreListFromJson(QJsonArray array) {
    list = array.toVariantList();
}
