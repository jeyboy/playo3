#include "item_fields.h"

using namespace Core;

ItemFields::ItemFields(const QVariantHash & hash) : ItemState(hash.value(JSON_TYPE_STATE).toInt()) {
    attrs = new QVariantHash(hash);
}

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> value(JSON_TYPE_STATE).toInt()) {
    attrs = new QVariantHash(hash -> toVariantHash());
}

ItemFields::ItemFields(const QString & title, int initState) : ItemState(initState) {
    attrs = new QVariantHash();
    attrs -> operator[](JSON_TYPE_TITLE) = title;
}

ItemFields::ItemFields(int state) : ItemState(state) {
}

QStringList ItemFields::info() const {
    QStringList list;
    QVariant i = infoVar();

    if (!i.isValid())
        list.append(QStringLiteral("Wait on proc..."));
    else
        list.append(i.toString());
    list.append(duration().toString());

    return list;
}

QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject::fromVariantHash(*attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantHash ItemFields::toInnerAttrs(int /*itemType*/) const {
    QVariantHash a(*attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
//    a.insert(JSON_TYPE_ITEM_TYPE, itemType);
    return a;
}
