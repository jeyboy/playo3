#ifndef DEFINES
#define DEFINES

#include <qstring.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

//typedef QMap<QString, QVariant> QVariantMap;
//class StrInitList : public std::initializer_list<std::pair<QString, QString> > {

//};

//class VarInitList : public std::initializer_list<std::pair<QString, QVariant> > {

//};

#define LSTR(str) QStringLiteral(str)

#define JSON_HAS_KEY(json, key) json.contains(key)

#define JSON_CONV_STR(json_val) (json_val.isString() ? json_val.toString() : QString::number((qint64)json_val.toDouble()))

#define JSON_STR_CAT(json, key1, separator, key2) QString(JSON_STR(json, key1) % separator % JSON_STR(json, key2))
/*
#define JSON_ARR_STR_CAT(json, key, separator) \
    QString cat_str; \
    for(QJsonArray::Iterator item = json.begin(); item != json.end(); item++) { \
        QJsonObject item_obj = (*item).toObject(); \
        cat_str = cat_str % (cat_str.isEmpty() ? QString() : separator) % JSON_STR(item_obj, key); \
    }\
    cat_str
*/

#define JSON_VAL(json, key) json.value(key)

#define JSON_CSTR(json, key) JSON_CONV_STR(json.value(key))
#define JSON_STR(json, key) json.value(key).toString()
#define JSON_STR_DEF(json, key, def) json.value(key).toString(def)

#define JSON_INT(json, key) (qint64)json.value(key).toDouble()
#define JSON_BOOL(json, key) json.value(key).toBool()
#define JSON_OBJ(json, key) json.value(key).toObject()
#define JSON_ARR(json, key) json.value(key).toArray()

//#define JSON_INT(json, key1, key2) JSON_INT(JSON_OBJ(json, key1), key2)
#define JSON_CSTR2(json, key1, key2) JSON_CSTR(JSON_OBJ(json, key1), key2)
#define JSON_OBJ2(json, key1, key2) JSON_OBJ(JSON_OBJ(json, key1), key2)
#define JSON_ARR2(json, key1, key2) JSON_ARR(JSON_OBJ(json, key1), key2)

#endif // DEFINES
