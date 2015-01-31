#include "item_fields.h"

using namespace Playo3;

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

QString ItemFields::downloadTitle() const {
    QString ret = filenameFilter(title().toString());

    if (extension().isValid())
        ret = ret + '.' + extension().toString();

    ret.replace(QRegExp("\\s+"), " ");
    return ret;
}
