#include "teu_auth.h"

using namespace Core::Web;

void TeuAuth::setParams(const QString & accessToken, const QString & userID, const QString & additional) {
    _token = accessToken;
    _user_id = userID;
    _additional = additional;
}

void TeuAuth::fromJson(const QJsonObject & hash) {
    _user_id = hash.value(userIdJsonKey()).toString();
    _token = hash.value(tokenJsonKey()).toString();
    _additional = hash.value(additionalJsonKey()).toString();
}
void TeuAuth::toJson(QJsonObject & hash) {
    hash[userIdJsonKey()] = _user_id;
    hash[tokenJsonKey()] = _token;
    hash[additionalJsonKey()] = _additional;
}
