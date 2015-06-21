#ifndef JSON_H
#define JSON_H

#include <qjsonobject.h>
#include <qjsonarray.h>

//NOT USED

class Json : public QJsonObject {
public:
    inline QString toStr(QString key) { return value(key).toString(); }
    inline Json toObj(QString key) { return (Json)value(key).toObject(); }
    inline bool toBool(QString key) { return value(key).toBool(); }
};

#endif // JSON_H
