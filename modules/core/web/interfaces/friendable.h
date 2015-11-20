#ifndef FRIENDABLE
#define FRIENDABLE

#include <qhash.h>
#include <qjsonobject.h>

namespace Core {
    namespace Web {
        class Friendable {
            QHash<QString, QString> friends;

            QString jsonToken() const { return QStringLiteral("friends"); }
        public:
            inline void addFriend(const QString & uid, const QString & name) {
                if (!uid.isEmpty() && !name.isEmpty())
                    friends.insert(uid, name);
            }

            inline QHash<QString, QString> friendsList() const { return friends; }
            inline void clearFriends() { friends.clear(); }

            void fromJson(const QJsonObject & hash) {
                QJsonObject ar = hash.value(jsonToken()).toObject();
                foreach(QString key, ar.keys())
                    addFriend(key, ar.value(key).toString());
            }
            void toJson(QJsonObject & hash) {
                QJsonObject friendsJson;

                for(QHash<QString, QString>::Iterator frend = friends.begin(); frend != friends.end(); frend++)
                    friendsJson.insert(frend.key(), QJsonValue(frend.value()));

                hash.insert(jsonToken(), friendsJson);
            }
        };
    }
}

#endif // FRIENDABLE
