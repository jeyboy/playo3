#include "vk_api.h"

VkApi * VkApi::self = 0;

VkApi * VkApi::instance() {
    return self;
}

VkApi * VkApi::instance(QObject * parent, QJsonObject obj) {
    if(!self)
        self = new VkApi(parent, obj);
    else
        VkApi::instance() -> fromJson(obj);
    return self;
}

void VkApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject VkApi::toJson() {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    return root;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
void VkApi::proceedAuthResponse(const QUrl & url) {
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

bool VkApi::extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) {
    QJsonObject stat_obj = response.value(QStringLiteral("error")).toObject();
    message = stat_obj.value(QStringLiteral("error_msg")).toString();
    code = stat_obj.value(QStringLiteral("error_code")).toInt();

    if (code == 14)
        return captchaProcessing(response, url);
    else return code == 0;
}

QUrl VkApi::buildUrl(QUrl tUrl, int offset, int limit) {
    QString urlStr = tUrl.toString();
    urlStr = urlStr.replace(QStringLiteral("_1_"), QString::number(offset)).replace(QStringLiteral("_2_"), QString::number(limit));
    return QUrl(urlStr);
}

bool VkApi::captchaProcessing(QJsonObject & response, QUrl & url) {
    QJsonObject stat_obj = response.value(QStringLiteral("error")).toObject();
    QUrl image_url(stat_obj.value(QStringLiteral("captcha_img")).toString());

    WebManager * manager = 0;
    bool isNew = WebManager::valid(manager);
    captchaDialog -> setImage(manager -> openImage(image_url));
    emit showCaptcha();
    if (isNew) delete manager;

    QString captchaText = captchaDialog -> captchaText();
    if (captchaText.isEmpty())
        return false;

    QUrlQuery query(url.query());
    query.removeQueryItem(QStringLiteral("captcha_sid"));
    query.removeQueryItem(QStringLiteral("captcha_key"));

    query.addQueryItem(QStringLiteral("captcha_sid"), stat_obj.value(QStringLiteral("captcha_sid")).toString());
    query.addQueryItem(QStringLiteral("captcha_key"), captchaText);

    url.setQuery(query);

    return sQuery(url, response, wrap_extract);
}
