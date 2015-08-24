#ifndef TEU_H
#define TEU_H

#include <qstring.h>
#include <qjsonobject.h>

class TeuAuth {
public:
    inline TeuAuth() {}
    inline TeuAuth(QJsonObject & hash) { fromJson(hash); }
    inline virtual ~TeuAuth() {}

    void setParams(QString accessToken, QString userID, QString expiresIn);

    inline QString token() const { return _token; }
    inline QString expire() const { return _expires_in; }
    inline QString userID() const { return _user_id; }

    void fromJson(QJsonObject & hash);
    void toJson(QJsonObject & hash);

private:
    QString _token;
    QString _expires_in;
    QString _user_id;
};

#endif // TEU_H
