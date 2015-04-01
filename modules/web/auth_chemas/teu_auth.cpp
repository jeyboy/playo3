#include "teu_auth.h"

TeuAuth::TeuAuth() {}
TeuAuth::TeuAuth(QJsonObject & hash) {
    fromJson(hash);
}

void TeuAuth::setParams(QString accessToken, QString userID, QString expiresIn) {
    token = accessToken;
    user_id = userID;
    expires_in = expiresIn;
}

void TeuAuth::fromJson(QJsonObject & hash) {
    user_id = hash.value("_u_").toString();
    token = hash.value("_t_").toString();
    expires_in = hash.value("_e_").toString();
}
void TeuAuth::toJson(QJsonObject & hash) {
    hash["_u_"] = getUserID();
    hash["_t_"] = getToken();
    hash["_e_"] = getExpire();
}
