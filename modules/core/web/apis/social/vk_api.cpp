#include "vk_api.h"

using namespace Core::Web::Vk;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
    Sociable::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    Sociable::toJson(root);

    hash.insert(name(), root);
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUser(const ConnectionType & /*conType*/) {
    if (isConnected()) return true;

    QUrl form_url = authUrl();

    while(true) {
        Response * resp = Manager::prepare() -> followedGet(form_url);
        QString err;
        Html::Document html = resp -> toHtml(false);

        if (html.has("input[name='pass']")) { // if user not authorized
            resp -> deleteLater();
            QHash<QString, QString> vals;
            err = html.find(".service_msg_warning").text();

            Html::Set forms = html.find("form");

            if (forms.isEmpty()) {
                Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
                return false;
            }
            Html::Tag * form = forms.first();

            QString captcha_src;
            Html::Set captcha_set = form -> find("img#captcha");

            if (!captcha_set.isEmpty())
                captcha_src = captcha_set.first() -> value("src");

            if (captcha_src.isEmpty()) {
                if (!showingLogin(val_auth_title, vals[tkn_email], vals[tkn_password], err))
                    return false;
            } else {
                if (!showingLoginWithCaptcha(val_auth_title, captcha_src,
                    vals[tkn_email], vals[tkn_password], vals[tkn_captcha], err
                )) return false;
            }

            form_url = form -> serializeFormToUrl(vals);
            resp = Manager::prepare() -> followedForm(form_url);
        }

        form_url = resp -> toUrl();
        QUrlQuery query(form_url.fragment());

        if (query.hasQueryItem(tkn_error)) {
            error = query.queryItemValue(tkn_error_description);
            return false;
        } else if (query.hasQueryItem(tkn_access_token)) {
            setParams(
                query.queryItemValue(tkn_access_token),
                query.queryItemValue(tkn_user_id),
                query.queryItemValue(tkn_expires_in)
            );
            emit authorized();
            initButton();
            return true;
        }
    }
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool Api::extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) {
    QJsonObject stat_obj = response.value(tkn_error).toObject();
    message = stat_obj.value(tkn_error_msg).toString();
    code = stat_obj.value(tkn_error_code).toInt();

    if (code == 14) {
        if (Settings::obj().isIgnoreCaptcha())
            return false;
        else
            return captchaProcessing(response, url);
    } else return code == 0;
}

QUrl Api::buildUrl(QUrl tUrl, int offset, int limit, const QJsonObject & /*prev_response*/) {
    QString urlStr = tUrl.toString();
    urlStr = urlStr.replace(tkn_predef1, QString::number(offset)).replace(tkn_predef2, QString::number(limit));
    return QUrl(urlStr);
}

bool Api::captchaProcessing(QJsonObject & response, QUrl & url) {
    QJsonObject stat_obj = response.value(tkn_error).toObject();

    QString captchaText;
    showingCaptcha(QUrl(stat_obj.value(tkn_captcha_img).toString()), captchaText);
    if (captchaText.isEmpty())
        return false;

    QUrlQuery query(url.query());
    query.removeQueryItem(tkn_captcha_sid);
    query.removeQueryItem(tkn_captcha);

    query.addQueryItem(tkn_captcha_sid, stat_obj.value(tkn_captcha_sid).toString());
    query.addQueryItem(tkn_captcha, captchaText);

    url.setQuery(query);

    return sQuery(url, response);
}
