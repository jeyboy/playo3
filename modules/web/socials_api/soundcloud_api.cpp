#include "soundcloud_api.h"

SoundcloudApi * SoundcloudApi::self = 0;

SoundcloudApi * SoundcloudApi::instance() {
    if(!self)
        self = new SoundcloudApi();
    return self;
}

SoundcloudApi * SoundcloudApi::instance(QJsonObject obj) {
    if(!self)
        self = new SoundcloudApi(obj);
    else
        SoundcloudApi::instance() -> fromJson(obj);
    return self;
}

void SoundcloudApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject SoundcloudApi::toJson() {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    return root;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
void SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        emit responseReady("reject");
    } else if (query.hasQueryItem("code")) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QJsonObject doc = CustomNetworkAccessManager::manager() -> postToJson(request, authTokenUrlParams(query.queryItemValue("code")));

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request(confirmAuthUrl(newToken));
            doc = CustomNetworkAccessManager::manager() -> getToJson(request);

            setParams(newToken, QString::number(doc.value("id").toInt()), "");
            emit authorized();
            emit responseReady("accept");
        }
        else emit responseReady("reject");
    }
    else emit responseReady("");
}
