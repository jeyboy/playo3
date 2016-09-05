#ifndef DEFINES
#define DEFINES

#include <qjsonobject.h>
#include <qjsonarray.h>

#define STR(str) QStringLiteral(str)

#define JSON_OBJ(json, key) json.value(key).toObject()
#define JSON_ARR(json, key) json.value(key).toArray()

#define JSON_OBJ2(json, key1, key2) json.value(key1).toObject().value(key2).toObject()
#define JSON_ARR2(json, key1, key2) json.value(key1).toObject().value(key2).toArray()

#endif // DEFINES
