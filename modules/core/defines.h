#ifndef DEFINES
#define DEFINES

#include <qjsonobject.h>
#include <qjsonarray.h>

#define LSTR(str) QStringLiteral(str)

#define JSON_HAS_KEY(json, key) json.contains(key)

#define JSON_CONV_STR(json_val) (json_val.isString() ? json_val.toString() : QString::number((qint64)json_val.toDouble()))

#define JSON_STR_CAT(json, key1, separator, key2) QString(JSON_STR(json, key1) % separator % JSON_STR(json, key2))

#define JSON_VAL(json, key) json.value(key)

#define JSON_STR(json, key) JSON_CONV_STR(json.value(key))
#define JSON_STR_DEF(json, key, def) json.value(key).toString(def)
#define JSON_INT(json, key) (qint64)json.value(key).toDouble()
#define JSON_BOOL(json, key) json.value(key).toBool()

#define JSON_OBJ(json, key) json.value(key).toObject()
#define JSON_ARR(json, key) json.value(key).toArray()

#define JSON_OBJ2(json, key1, key2) JSON_OBJ(JSON_OBJ(json, key1), key2)
#define JSON_ARR2(json, key1, key2) JSON_ARR(JSON_OBJ(json, key1), key2)

#endif // DEFINES
