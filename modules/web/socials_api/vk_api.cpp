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

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        emit responseReady("reject");
    } else if (query.hasQueryItem("access_token")) {
        setParams(
            query.queryItemValue("access_token"),
            query.queryItemValue("user_id"),
            query.queryItemValue("expires_in")
        );
        emit authorized();
        emit responseReady("accept");
    }
    else emit responseReady("");
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool VkApi::extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) {
    QJsonObject stat_obj = response.value("error").toObject();
    message = stat_obj.value("error_msg").toString();
    code = stat_obj.value("error_code").toInt();

    if (code == 14)
        return captchaProcessing(response, url);
    else return code == 0;
}

QUrl VkApi::buildUrl(QUrl tUrl, int offset, int limit) {
    QString urlStr = tUrl.toString();
    urlStr.replace("%%1", QString::number(offset)).replace("%%2", QString::number(limit));
    return QUrl(urlStr);
}


bool VkApi::captchaProcessing(QJsonObject & response, QUrl & url) {
    QUrl image_url(response.value("captcha_img").toString());

    CustomNetworkAccessManager * manager = 0;
    bool isNew = CustomNetworkAccessManager::validManager(manager);
    captchaDialog -> setImage(manager -> openImage(image_url));
    emit showCaptcha();
    if (isNew) delete manager;

    QString captchaText = captchaDialog -> captchaText();
    if (captchaText.isEmpty())
        return false;

    QUrlQuery query(url.query());
    query.removeQueryItem("captcha_sid");
    query.removeQueryItem("captcha_key");

    query.addQueryItem("captcha_sid", response.value("captcha_sid").toString());
    query.addQueryItem("captcha_key", captchaText);

    url.setQuery(query);

    return sQuery(url, response);
}

///////////////////////////////////////////////////////////
/// SLOTS
///////////////////////////////////////////////////////////


//void VkApi::audioCountRequest() {}
//void VkApi::audioSearchRequest() {}
//void VkApi::audioCopyRequest() {}
//void VkApi::audioRemoveRequest() {}

//void VkApi::audioSaveServerRequest() {}
//void VkApi::audioSave() {}

//void VkApi::audioAlbumsRequest() {}
//void VkApi::audioAlbumAddRequest() {}
//void VkApi::audioAlbumEditRequest() {}
//void VkApi::audioAlbumRemoveRequest() {}
//void VkApi::audioAlbumMoveToRequest() {}
