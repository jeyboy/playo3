#include "deleted_list.h"
#include <QDebug>

DeletedList::DeletedList() {

}
DeletedList::~DeletedList() {

}

void DeletedList::addRemovedUID(QVariant uid) {
    list.append(uid);
}
void DeletedList::removeUID(QVariant uid) {
    list.removeAll(uid);
}
bool DeletedList::containsUID(QVariant uid) {
    return list.contains(uid);
}

QJsonArray DeletedList::deletedToJson() {
    qDebug() << "SAVE " << list;
    return QJsonArray::fromVariantList(list);
}
void DeletedList::deletedFromJson(QJsonArray array) {
    foreach(QJsonValue value, array) {
        list.append(value.toVariant());
    }
    qDebug() << "LOAD " << list;
}
