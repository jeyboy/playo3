#ifndef TEU_H
#define TEU_H

#include <qstring.h>
#include <qjsonobject.h>

namespace Core {
    namespace Web {
        class TeuAuth {
            QString _token;
            QString _expires_in;
            QString _user_id;
        public:
            inline TeuAuth() {}
            inline TeuAuth(const QJsonObject & hash) { fromJson(hash); }
            inline virtual ~TeuAuth() {}

            void setParams(QString accessToken, QString userID, QString expiresIn);

            inline QString token() const { return _token; }
            inline QString expire() const { return _expires_in; }
            inline QString userID() const { return _user_id; }

            void fromJson(const QJsonObject & hash);
            void toJson(QJsonObject & hash);
        };
    }
}

#endif // TEU_H
