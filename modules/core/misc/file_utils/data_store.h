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
            DataStore(QString name);
            ~DataStore();

            inline QJsonObject & obj() { return json; }

            bool load();
            bool save();

            QJsonValue read(QString key);
            void write(QString key, double value);
            void write(QString key, QJsonArray value);
            void write(QString key, QJsonObject value);
            void write(QString key, QString value);
            void write(QString key, int value);
            void write(QString key, QVariant value);
            void append(QString key, QString subkey, QString value);
            void clear();
            QStringList keys();

            bool state;
        private:
            QString filename;
            QJsonObject json;
    };
}

#endif // DATA_STORE_H
