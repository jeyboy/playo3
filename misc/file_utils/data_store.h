#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "misc/web_utils/json.h"

#include <qfile.h>
#include <qstringlist.h>
//#include <qvariant.h>

namespace Playo3 {
    class DataStore {
        public:
            DataStore(const QString & name);
            ~DataStore();

            bool load();
            bool save();


            void append(const QString & key, const QString & subkey, const QString & value);
            inline Json::Cell read(const QString & key) { return json.value(key); } // need override
            inline void write(const QString & key, const auto & value) { json[key] = value; }
            inline void write(const QString & key, const QVariant & value) { json.insert(key, QJsonValue::fromVariant(value)); }
            inline void clear() { json = Json::Obj(); }
            inline QStringList keys() { return json.keys(); }

            inline bool state() const { return state; }
        private:
            bool _state;
            QString filename;
            Json::Obj json;
    };
}

#endif // DATA_STORE_H
