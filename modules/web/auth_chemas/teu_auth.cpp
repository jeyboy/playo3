#include "teu_auth.h"

void TeuAuth::setParams(QString accessToken, QString userID, QString expiresIn) {
    _token = accessToken;
    _user_id = userID;
    _expires_in = expiresIn;
}

void TeuAuth::fromJson(const QJsonObject & hash) {
    _user_id = hash.value(QStringLiteral("_u_")).toString();
    _token = hash.value(QStringLiteral("_t_")).toString();
    _expires_in = hash.value(QStringLiteral("_e_")).toString();
}
void TeuAuth::toJson(QJsonObject & hash) {
    hash[QStringLiteral("_u_")] = _user_id;
    hash[QStringLiteral("_t_")] = _token;
    hash[QStringLiteral("_e_")] = _expires_in;
}
