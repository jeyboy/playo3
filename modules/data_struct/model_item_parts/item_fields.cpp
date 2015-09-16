#include "item_fields.h"

using namespace Playo3;

ItemFields::ItemFields(QVariantMap & hash) : ItemState(hash.take(JSON_TYPE_STATE).toInt()) {
    attrs = QVariantMap(hash);
}

ItemFields::ItemFields(Json::Obj * hash) : ItemState(hash -> takeInt(JSON_TYPE_STATE)) {
    attrs = hash.toVariantMap();
}

ItemFields::ItemFields(const QString & title, int initState) : ItemState(initState) {
    attrs[JSON_TYPE_TITLE] = title;
}

ItemFields::ItemFields(int state) : ItemState(state) {
}

QStringList ItemFields::info() const {
    QStringList list;
    QVariant i = _info();

    if (!i.isValid())
        list.append(QStringLiteral("Processing ..."));
    else
        list.append(i.toString());
    list.append(duration().toString());

    return list;
}

Json::Obj ItemFields::toJson() {
    Json::Obj root = Json::Obj::fromVariantMap(attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantMap ItemFields::toInnerAttrs(int itemType) const {
    QVariantMap a(attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
    a.insert(JSON_TYPE_ITEM_TYPE, itemType);
    return a;
}
