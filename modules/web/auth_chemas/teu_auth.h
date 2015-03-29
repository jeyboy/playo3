#ifndef TEU_H
#define TEU_H

#include <QString>
#include <QJsonObject>

class TeuAuth {
public:
    TeuAuth();
    TeuAuth(QJsonObject & hash);

    void setParams(QString accessToken, QString userID, QString expiresIn);

    inline QString getToken() { return token; }
    inline QString getExpire() { return expires_in; }
    inline QString getUserID() { return user_id; }

    void fromJson(QJsonObject & hash);
    QJsonObject & toJson(QJsonObject & root);

private:
    QString token;
    QString expires_in;
    QString user_id;
};

#endif // TEU_H
