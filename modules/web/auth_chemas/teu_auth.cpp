#include "teu_auth.h"

void TeuAuth::setParams(QString accessToken, QString userID, QString expiresIn) {
    _token = accessToken;
    _user_id = userID;
    _expires_in = expiresIn;
}

void TeuAuth::fromJson(QJsonObject & hash) {
    _user_id = hash.value("_u_").toString();
    _token = hash.value("_t_").toString();
    _expires_in = hash.value("_e_").toString();
}
void TeuAuth::toJson(QJsonObject & hash) {
    hash["_u_"] = _user_id;
    hash["_t_"] = _token;
    hash["_e_"] = _expires_in;
}
