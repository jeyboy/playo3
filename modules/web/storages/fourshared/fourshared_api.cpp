#include "fourshared_api.h"

namespace Fourshared { // for auth required oauth 1
    Api * Api::self = 0;

    Api * Api::instance() {
        if(!self) self = new Api();
        return self;
    }

    Api * Api::instance(const Json::Obj & obj) {
        if(!self) self = new Api(obj);
        else Api::instance() -> fromJson(obj);
        return self;
    }

    QString Api::authUrl() {
        QUrl url(base_url % QStringLiteral("oauth/initiate"));

//        QUrlQuery query = genDefaultParams();
//        setParam(query, QStringLiteral("response_type"), QStringLiteral("code"));
//        setParam(query, QStringLiteral("scope"), QStringLiteral("non-expiring"));
//        setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
//        setParam(query, QStringLiteral("display"), QStringLiteral("popup"));

//        url.setQuery(query);
        return url.toString();
    }

    void Api::fromJson(const Json::Obj & hash) {
        TeuAuth::fromJson(hash);
        WebApi::fromJson(hash);
    }
    Json::Obj Api::toJson() {
        Json::Obj root;

        TeuAuth::toJson(root);
        WebApi::toJson(root);

        return root;
    }

    ///////////////////////////////////////////////////////////
    /// AUTH
    ///////////////////////////////////////////////////////////
    void Api::proceedAuthResponse(const QUrl & /*url*/) {
//        QUrlQuery query(url.query());

//        if (query.hasQueryItem(QStringLiteral("error"))) {
//            error = query.queryItemValue(QStringLiteral("error_description"));
//            emit responseReady(QStringLiteral("reject"));
//        } else if (query.hasQueryItem(QStringLiteral("code"))) {
//            QNetworkRequest request(authTokenUrl());
//            request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
//            Json::Obj doc = WebManager::stock() -> postToJson(request, authTokenUrlParams(query.queryItemValue(QStringLiteral("code"))));

//            if (doc.contains(QStringLiteral("access_token"))) {
//                QString newToken = doc.value(QStringLiteral("access_token")).toString();

//                QNetworkRequest request(confirmAuthUrl(newToken));
//                doc = WebManager::stock() -> getToJson(request);

//                setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), "");
//                emit authorized();
//                emit responseReady(QStringLiteral("accept"));
//            }
//            else emit responseReady(QStringLiteral("reject"));
//        }
//        else emit responseReady("");
    }
}

