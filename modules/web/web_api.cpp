#include "web_api.h"

WebApi::WebApi(QObject * parent) : Async(parent) {
    actionDialog = new UserActionDialog((QWidget *)parent);
}

WebApi::~WebApi() {
    delete actionDialog;
}

void WebApi::clearData() {
    friends.clear();
    groups.clear();
}

//void WebApi::fromJson(Json::Obj & hash) {
//    Json::Obj obj;
//    Json::Arr friends = hash.arr("friends");
//    Json::Arr::Iterator it = friends.begin();

//    for(; it != friends.end(); it++) {
//        obj = (*it).toObject();
//        addFriend(it.key(), it.value().toString());
//    }

//    it = hash.find("groups");

//    for(;it != hash.end(); it++)
//        addGroup(it.key(), it.value().toString());
//}

//void WebApi::toJson(Json::Obj & hash) {
//    qDebug() << "API SAVE " << friends;

//    Json::Obj friendsJson;
//    for(QHash<QString, QString>::iterator i = friends.begin(); i != friends.end(); ++i)
//        friendsJson.insert(i.key(), QJsonValue(i.value()));
//    qDebug() << "----------------------";
//    qDebug() << friendsJson;
//    hash.insert("friends", friendsJson);

//    Json::Obj groupsJson;
//    for(QHash<QString, QString>::iterator i = groups.begin(); i != groups.end(); ++i)
//        friendsJson.insert(i.key(), QJsonValue(i.value()));
//    hash.insert("groups", groupsJson);
//}

void WebApi::fromJson(const Json::Obj & hash) { //TODO: replace foreach with for
    Json::Obj ar = hash.cobj(QStringLiteral("friends"));
    foreach(QString key, ar.keys())
        addFriend(key, ar.str(key));

    ar = hash.cobj(QStringLiteral("groups"));
    foreach(QString key, ar.keys())
        addGroup(key, ar.str(key));
}
void WebApi::toJson(Json::Obj & root) {
    Json::Obj friendsJson;
    foreach(QString key, friends.keys())
        friendsJson.insert(key, QJsonValue(friends.value(key)));

    root.insert(QStringLiteral("friends"), friendsJson);

    Json::Obj groupsJson;
    foreach(QString key, groups.keys())
        groupsJson.insert(key, QJsonValue(groups.value(key)));

    root.insert(QStringLiteral("groups"), groupsJson);
}

void WebApi::showingCaptcha(const QUrl & pict_url, QString & result) {
    actionDialog -> buildCaptchaForm(WebManager::manager() -> getImage(pict_url));
    executingDialog();
    result = actionDialog -> getValue(actionDialog -> captcha_key);
}

void WebApi::showingLogin(QString & login, QString & pass) {
    actionDialog -> buildLoginForm();
    executingDialog();

    login = actionDialog -> getValue(actionDialog -> login_key);
    pass = actionDialog -> getValue(actionDialog -> pass_key);
}
