#include "fourshared_api.h"

using namespace Core::Web::Fourshared; // for auth required oauth 1

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);

    hash.insert(name(), root);
}
