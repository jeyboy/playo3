#include "vk_api.h"

using namespace Core::Web::Vk;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
    WebApi::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    hash.insert(name(), root);
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connection() {
    if (isConnected()) return true;

    QUrl form_url = authUrl();

    while(true) {
        Response * resp = Manager::prepare() -> followedGet(form_url);
        QString err;
        Html::Document html = resp -> toHtml(false);

        if (html.has("input[name='pass']")) { // if user not authorized
            QHash<QString, QString> vals;
            err = html.find(".service_msg_warning").text();
            if (!showingLogin(vals[QStringLiteral("email")], vals[QStringLiteral("pass")], err))
                return false;

            form_url = html.find("form").first() -> serializeFormToUrl(vals);
            resp = Manager::prepare() -> followedForm(form_url);
        }

        form_url = resp -> toUrl();
        QUrlQuery query(form_url.fragment());

        if (query.hasQueryItem(QStringLiteral("error"))) {
            error = query.queryItemValue(QStringLiteral("error_description"));
            return false;
        } else if (query.hasQueryItem(QStringLiteral("access_token"))) {
            setParams(
                query.queryItemValue(QStringLiteral("access_token")),
                query.queryItemValue(QStringLiteral("user_id")),
                query.queryItemValue(QStringLiteral("expires_in"))
            );
            emit authorized();
            return true;
        }
    }
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool Api::extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) {
    QJsonObject stat_obj = response.value(error_key).toObject();
    message = stat_obj.value(error_msg_key).toString();
    code = stat_obj.value(error_code_key).toInt();

    if (code == 14)
        return captchaProcessing(response, url);
    else return code == 0;
}

QUrl Api::buildUrl(QUrl tUrl, int offset, int limit) {
    QString urlStr = tUrl.toString();
    urlStr = urlStr.replace(predef1_key, QString::number(offset)).replace(predef2_key, QString::number(limit));
    return QUrl(urlStr);
}

bool Api::captchaProcessing(QJsonObject & response, QUrl & url) {
    QJsonObject stat_obj = response.value(error_key).toObject();

    QString captchaText;
    showingCaptcha(QUrl(stat_obj.value(captcha_img_key).toString()), captchaText);
    if (captchaText.isEmpty())
        return false;

    QUrlQuery query(url.query());
    query.removeQueryItem(captcha_sid_key);
    query.removeQueryItem(captcha_key);

    query.addQueryItem(captcha_sid_key, stat_obj.value(captcha_sid_key).toString());
    query.addQueryItem(captcha_key, captchaText);

    url.setQuery(query);

    return sQuery(url, response);
}
