#include "vk_api.h"

namespace Vk {
    Api * Api::self = 0;

    Api * Api::instance() {
        return self;
    }

    Api * Api::instance(QObject * parent, QJsonObject obj) {
        if(!self)
            self = new Api(parent, obj);
        else
            Api::instance() -> fromJson(obj);
        return self;
    }

    void Api::fromJson(QJsonObject hash) {
        TeuAuth::fromJson(hash);
        WebApi::fromJson(hash);
    }
    QJsonObject Api::toJson() {
        QJsonObject root;

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
        QUrl image_url(stat_obj.value(captcha_img_key).toString());

        WebManager * manager = 0;
        bool isNew = WebManager::valid(manager);
        captchaDialog -> setImage(manager -> openImage(image_url));
        emit showCaptcha();
        if (isNew) delete manager;

        QString captchaText = captchaDialog -> captchaText();
        if (captchaText.isEmpty())
            return false;

        QUrlQuery query(url.query());
        query.removeQueryItem(captcha_sid_key);
        query.removeQueryItem(captcha_key);

        query.addQueryItem(captcha_sid_key, stat_obj.value(captcha_sid_key).toString());
        query.addQueryItem(captcha_key, captchaText);

        url.setQuery(query);

        return sQuery(url, response, wrap_extract);
    }
}
