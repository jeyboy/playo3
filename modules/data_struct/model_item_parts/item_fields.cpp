#include "item_fields.h"

using namespace Playo3;

ItemFields::ItemFields(QVariantMap & hash) : ItemState(hash.take(JSON_TYPE_STATE).toInt()) {
    attrs = QVariantMap(hash);
}

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> take(JSON_TYPE_STATE).toInt()) {
    attrs = hash -> toVariantMap();
}

ItemFields::ItemFields(QString title, int initState) : ItemState(initState) {
    attrs[JSON_TYPE_TITLE] = title;
}

ItemFields::ItemFields(int state) : ItemState(state) {
}

QStringList ItemFields::info() const {
    QStringList list;
    QVariant i = _info();

    if (!i.isValid())
        list.append("Processing ...");
    else
        list.append(i.toString());
    list.append(duration().toString());

    return list;
}

QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject::fromVariantMap(attrs);
    root[JSON_TYPE_STATE] = saveStates();
    return root;
}

QVariantMap ItemFields::toInnerAttrs(int itemType) const {
    QVariantMap a(attrs);
    a.insert(JSON_TYPE_STATE, saveStates());
    a.insert(JSON_TYPE_ITEM_TYPE, itemType);
    return a;
}

QString ItemFields::downloadTitle() const {
    QString ret = filenameFilter(title().toString());

    if (extension().isValid())
        ret = ret + '.' + extension().toString();

    ret.replace(QRegExp("\\s+"), " "); // TODO: remove empty brackets
    return ret;
}
