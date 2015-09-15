#include "vk_api.h"

namespace Vk {
    Api * Api::self = 0;

    Api * Api::instance() {
        return self;
    }

    Api * Api::instance(QObject * parent, const Json::Obj & obj) {
        if(!self)
            self = new Api(parent, obj);
        else
            Api::instance() -> fromJson(obj);
        return self;
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
    void Api::proceedAuthResponse(const QUrl & url) {
        QUrlQuery query(url.fragment());

        if (query.hasQueryItem(QStringLiteral("error"))) {
            error = query.queryItemValue(QStringLiteral("error_description"));
            emit responseReady(QStringLiteral("reject"));
        } else if (query.hasQueryItem(QStringLiteral("access_token"))) {
            setParams(
                query.queryItemValue(QStringLiteral("access_token")),
                query.queryItemValue(QStringLiteral("user_id")),
                query.queryItemValue(QStringLiteral("expires_in"))
            );
            emit authorized();
            emit responseReady(QStringLiteral("accept"));
        }
        else emit responseReady("");
    }

    ///////////////////////////////////////////////////////////
    /// PROTECTED
    ///////////////////////////////////////////////////////////

    bool Api::extractStatus(QUrl & url, Json::Obj & response, int & code, QString & message) {
        Json::Obj stat_obj = response.obj(error_key);
        message = stat_obj.str(error_msg_key);
        code = stat_obj.intVal(error_code_key);

        if (code == 14)
            return captchaProcessing(response, url);
        else return code == 0;
    }

    QUrl Api::buildUrl(QUrl tUrl, int offset, int limit) {
        QString urlStr = tUrl.toString();
        urlStr = urlStr.replace(predef1_key, QString::number(offset)).replace(predef2_key, QString::number(limit));
        return QUrl(urlStr);
    }

    bool Api::captchaProcessing(Json::Obj & response, QUrl & url) {
        Json::Obj stat_obj = response.value(error_key).toObject();

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
}
