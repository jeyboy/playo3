#ifndef GROUPABLE
#define GROUPABLE

#include <qhash.h>
#include <qjsonobject.h>

namespace Core {
    namespace Web {
        class Groupable {
            QHash<QString, QString> groups;

            QString jsonToken() const { return QStringLiteral("groups"); }
        public:
            inline void addGroup(const QString & uid, const QString & name) {
                if (!uid.isEmpty() && !name.isEmpty())
                    groups.insert(uid, name);
            }

            inline QHash<QString, QString> groupsList() const { return groups; }
            inline clearGroups() { groups.clear(); }

            void fromJson(const QJsonObject & hash) {
                QJsonObject ar = hash.value(jsonToken()).toObject();
                foreach(QString key, ar.keys())
                    addGroup(key, ar.value(key).toString());
            }
            void toJson(QJsonObject & hash) {
                QJsonObject groupsJson;

                for(QHash<QString, QString>::Iterator group = groups.begin(); group != groups.end(); group++)
                    groupsJson.insert(group.key(), QJsonValue(group.value()));

                hash.insert(jsonToken(), groupsJson);
            }
        };
    }
}


#endif // GROUPABLE
