#include "item_fields.h"

using namespace Core;

ItemFields::ItemFields(QVariantHash & hash) : ItemState(hash.take(JSON_TYPE_STATE).toInt()) {
    attrs = QVariantHash(hash);
}

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> take(JSON_TYPE_STATE).toInt()) {
    attrs = hash -> toVariantHash();
}

ItemFields::ItemFields(const QString & title, int initState) : ItemState(initState) {
    attrs[JSON_TYPE_TITLE] = title;
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
    QJsonObject root = QJsonObject::fromVariantHash(attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantHash ItemFields::toInnerAttrs(int /*itemType*/) const {
    QVariantHash a(attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
//    a.insert(JSON_TYPE_ITEM_TYPE, itemType);
    return a;
}
