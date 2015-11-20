#ifndef TEU_H
#define TEU_H

#include <qstring.h>
#include <qjsonobject.h>

namespace Core {
    namespace Web {
        class TeuAuth {
            QString _token;
            QString _additional;
            QString _user_id;

            QString userIdJsonKey() const { return QStringLiteral("_u_"); }
            QString tokenJsonKey() const { return QStringLiteral("_t_"); }
            QString additionalJsonKey() const { return QStringLiteral("_a_"); }
        public:
            inline TeuAuth() {}
            inline TeuAuth(const QJsonObject & hash) { fromJson(hash); }
            inline virtual ~TeuAuth() {}

            void setParams(const QString & accessToken, const QString & userID, const QString & additional);
            void clearParams() { setParams(QString(), QString(), QString());}

            inline QString token() const { return _token; }
            inline QString additional() const { return _additional; }
            inline QString userID() const { return _user_id; }

            void fromJson(const QJsonObject & hash);
            void toJson(QJsonObject & hash);
        };
    }
}

#endif // TEU_H
