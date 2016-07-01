#include "soundcloud_api.h"

#include "dialogs/relations_dialog.h"

using namespace Core::Web::Soundcloud;

bool Api::takeOfflineCredentials()    {
    if (hasOfflineCredentials()) return true;

    QString html = Manager::prepare() -> getFollowed(url_site_base) -> toText();
    QRegularExpression reg("sc_version = \"(\\d+)\"");
    QRegularExpressionMatch match = reg.match(html);

    if (match.hasMatch()) {
        QString app_id = match.captured(1);
        setSiteHash(app_id);

        QRegularExpression js_reg("(http[:\\/\\w\\.\\-]+app-[\\w-]+\\.js)");
        match = js_reg.match(html);
        if (match.hasMatch()) {
            QString js_url = match.captured(1);
            QString js_content = Manager::prepare() -> getFollowed(js_url) -> toText();

            QRegularExpression id("client_id:\"(\\w+)\"");
            match = id.match(js_content);
            if (match.hasMatch()) {
                QString token = match.captured(1);
                setSiteToken(token);
                return true;
            }
        }
    }

    return false;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUserApi() {
    QUrl auth_url = authUrl();
    QUrl form_url = auth_url;
    Response * resp = Manager::prepare() -> getFollowed(form_url);
    QHash<QString, QString> vals;

    while(true) {
        QString err;
        Html::Document html = resp -> toHtml();

        Html::Tag * form = html.findFirst("form.authorize-token");

        if (!form) {
            Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
            return false;
        }

        if (form -> has("input[name='password']")) { // if user not authorized
            err = html.find(".warning").text();

            QString captcha_src;
            Html::Set captcha_set = form -> find(QString("script[src^'" + RECAPTCHA_BASE_URL + "']").toUtf8().data());

            if (!captcha_set.isEmpty())
                captcha_src = captcha_set.first() -> value(tkn_src);

            if (captcha_src.isEmpty()) {
                if (!showingLogin(val_auth_title, vals[tkn_username], vals[tkn_password], err))
                    return false;
            } else {
                if (!showingLoginWithCaptcha(
                    val_auth_title, Recaptcha::V1::obj().takeImageUrl(captcha_src, vals[tkn_recaptcha_challenge_field]),
                    vals[tkn_username], vals[tkn_password], vals[tkn_recaptcha_response_field], err
                )) return false;
            }
        }

        form_url = form -> serializeFormToUrl(vals, true);

        if (form_url.isRelative())
            form_url = auth_url.resolved(form_url);

        resp = Manager::prepare() -> formFollowed(form_url, {{tkn_referer, form_url.toString()}});

        QUrlQuery query(resp->toUrl(false).query());

        if (query.hasQueryItem(tkn_error)) {
            error = query.queryItemValue(tkn_error_description);
            qDebug() << "ERRRRRRRRRR" << error;
            resp -> deleteLater();
            return false;
        } else if (query.hasQueryItem(tkn_code)) {
            QJsonObject doc = Web::Manager::prepare() -> formFollowed(authTokenUrl(), authTokenUrlParams(query.queryItemValue(tkn_code))) -> toJson();

            if (doc.contains(tkn_access_token)) {
                QString newToken = doc.value(tkn_access_token).toString();
                doc = Web::Manager::prepare() -> jsonGet(confirmAuthUrl(newToken));

                setApiToken(newToken);
                setApiUserID(QString::number(doc.value(tkn_id).toInt()));
//                setParams(newToken, QString::number(doc.value(tkn_id).toInt()), QString());
                resp -> deleteLater();
                return true;
            }
        }
    }
}

bool Api::connectUserSite() {
//    curl 'https://sign-in.soundcloud.com/sign-in/password?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1467192015' -X POST -H
//    'DNT: 1' -H 'Host: sign-in.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com'

//    payload:

//        client_id
//            "02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea"

//        credentials
//            Object { identifier="jeyboy1985@gmail.com",  password="5675675675"}

//        identifier
//            "jeyboy1985@gmail.com"

//        password
//            "5675675675"

//        recaptcha_response
//            ""

//        scope
//            "fast-connect non-expiring purchase signup upload"



//    respond:

//        {"session":{"access_token":"1-138878-99021496-..."},"sign_in":{"user_urn":"soundcloud:users:9902","method":"password"}}

    return false;
}
