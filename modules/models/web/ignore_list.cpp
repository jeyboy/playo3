#include "ignore_list.h"

#include <qdebug.h>

using namespace Models;

IgnoreList::IgnoreList(QJsonObject * obj) {
    if (obj)
        ignoreListFromJson(obj -> take(jsonName()).toObject());
}

void IgnoreList::ignoreListAddUid(const QString & uid) { list.insert(uid, true); }
void IgnoreList::ignoreListAddUid(const QStringList & uids) {
    for(QStringList::ConstIterator uid = uids.constBegin(); uid != uids.constEnd(); uid++)
        list.insert(*uid, true);
}

void IgnoreList::ignoreListRemoveUid(const QString & uid) { list.remove(uid); }
bool IgnoreList::ignoreListContainUid(const QString & uid) { return list.contains(uid); }

QJsonObject & IgnoreList::ignoreListToJson(QJsonObject & obj) {
    obj.insert(jsonName(), QJsonObject::fromVariantHash(list));
    return obj;
}
void IgnoreList::ignoreListFromJson(const QJsonObject & obj) {  list = obj.toVariantHash(); qDebug() << "IGNORE LIST" << list; }
