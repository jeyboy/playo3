#include "web_api.h"

using namespace Core;

WebApi::WebApi(QObject * parent) : Async() {
    actionDialog = new UserActionDialog((QWidget *)parent);
}

void WebApi::clearData() {
    friends.clear();
    groups.clear();
}

//void WebApi::fromJson(QJsonObject & hash) {
//    QJsonObject obj;
//    QJsonArray friends = hash.value("friends").toArray();
//    QJsonArray::Iterator it = friends.begin();

//    for(; it != friends.end(); it++) {
//        obj = (*it).toObject();
//        addFriend(it.key(), it.value().toString());
//    }

//    it = hash.find("groups");

//    for(;it != hash.end(); it++)
//        addGroup(it.key(), it.value().toString());
//}

//void WebApi::toJson(QJsonObject & hash) {
//    qDebug() << "API SAVE " << friends;

//    QJsonObject friendsJson;
//    for(QHash<QString, QString>::iterator i = friends.begin(); i != friends.end(); ++i)
//        friendsJson.insert(i.key(), QJsonValue(i.value()));
//    qDebug() << "----------------------";
//    qDebug() << friendsJson;
//    hash.insert("friends", friendsJson);

//    QJsonObject groupsJson;
//    for(QHash<QString, QString>::iterator i = groups.begin(); i != groups.end(); ++i)
//        friendsJson.insert(i.key(), QJsonValue(i.value()));
//    hash.insert("groups", groupsJson);
//}

void WebApi::fromJson(const QJsonObject & hash) { //TODO: replace foreach with for
    QJsonObject ar = hash.value(QStringLiteral("friends")).toObject();
    foreach(QString key, ar.keys())
        addFriend(key, ar.value(key).toString());

    ar = hash.value(QStringLiteral("groups")).toObject();
    foreach(QString key, ar.keys())
        addGroup(key, ar.value(key).toString());
}
void WebApi::toJson(QJsonObject & root) {
    QJsonObject friendsJson;
    foreach(QString key, friends.keys())
        friendsJson.insert(key, QJsonValue(friends.value(key)));

    root.insert(QStringLiteral("friends"), friendsJson);

    QJsonObject groupsJson;
    foreach(QString key, groups.keys())
        groupsJson.insert(key, QJsonValue(groups.value(key)));

    root.insert(QStringLiteral("groups"), groupsJson);
}

bool WebApi::showingCaptcha(const QUrl & pict_url, QString & result) {
    actionDialog -> buildCaptchaForm(Web::Manager::prepare() -> getImage(pict_url));
    bool res = actionDialog -> exec();
    result = actionDialog -> getValue(actionDialog -> captcha_key);
    return res;
}

bool WebApi::showingLogin(QString & login, QString & pass) {
    actionDialog -> buildLoginForm();
    bool res = actionDialog -> exec();

    login = actionDialog -> getValue(actionDialog -> login_key);
    pass = actionDialog -> getValue(actionDialog -> pass_key);

    return res;
}
