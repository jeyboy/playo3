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

QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject::fromVariantMap(attrs);
    root[JSON_TYPE_STATE] = saveStateVal();
    return root;
}

QString ItemFields::downloadTitle() const {
    QString ret = filenameFilter(title().toString());

    if (extension().isValid())
        ret = ret + '.' + extension().toString();

    ret.replace(QRegExp("\\s+"), " ");
    return ret;
}
