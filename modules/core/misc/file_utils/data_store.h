#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>

#include <qfile.h>
#include <qstringlist.h>
#include <qvariant.h>

namespace Core {
    class DataStore {
        public:
            DataStore(const QString & name);
            ~DataStore();

            inline QJsonObject & obj() { return json; }

            bool load();
            bool save();

            QJsonValue read(QString key);
            void write(const QString & key, const double & value);
            void write(const QString & key, const QJsonArray & value);
            void write(const QString & key, const QJsonObject & value);
            void write(const QString & key, const QString & value);
            void write(const QString & key, const int & value);
            void write(const QString & key, const QVariant & value);
            void append(const QString & key, const QString & subkey, const QString & value);
            void clear();
            QStringList keys();

            bool state;
        private:
            QString filename;
            QJsonObject json;
    };
}

#endif // DATA_STORE_H
