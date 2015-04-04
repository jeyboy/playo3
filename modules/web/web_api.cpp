#include "web_api.h"

WebApi::WebApi(QObject * parent) : QObject(parent) {
    netManager = createManager();
    captchaDialog = new CaptchaDialog((QWidget *)parent);
}

WebApi::~WebApi() {
    delete netManager;
    delete captchaDialog;
}

QPixmap WebApi::openRemoteImage(QString url) { // error proc needed
    QUrl uri(url);
    QByteArray ar;

    QNetworkReply * reply = netManager -> openUrl(uri);
    netManager -> synchronizeRequest(reply);
    ar = reply -> readAll();
    reply -> close();
    reply -> deleteLater();

    QImage image;
    image.loadFromData(ar);
    return QPixmap::fromImage(image);
}

void WebApi::clearData() {
    friends.clear();
    groups.clear();
}

void WebApi::fromJson(QJsonObject & hash) {
    QJsonObject::iterator it = hash.find("friends");

    while(it != hash.end())
        addFriend(it.key(), it++.value().toString());

    it = hash.find("groups");

    while(it != hash.end())
        addGroup(it.key(), it++.value().toString());
}

void WebApi::toJson(QJsonObject & hash) {
    QJsonObject friendsJson;
    for(QHash<QString, QString>::iterator i = friends.begin(); i != friends.end(); ++i)
        friendsJson.insert(i.key(), QJsonValue(i.value()));
    hash.insert("friends", friendsJson);

    QJsonObject groupsJson;
    for(QHash<QString, QString>::iterator i = groups.begin(); i != groups.end(); ++i)
        friendsJson.insert(i.key(), QJsonValue(i.value()));
    hash.insert("groups", groupsJson);
}

QJsonObject WebApi::responseToJson(QByteArray data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}

void WebApi::showingCaptcha() {
    captchaDialog -> clearText();
    captchaDialog -> exec();
}