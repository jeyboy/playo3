#include "web_api.h"

WebApi::WebApi(QObject * parent) : QObject(parent) {
    captchaDialog = new CaptchaDialog((QWidget *)parent);
}

WebApi::~WebApi() {
    delete captchaDialog;
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

void WebApi::fromJson(QJsonObject & hash) {
    QJsonObject ar = hash.value("friends").toObject();
    foreach(QString key, ar.keys())
        addFriend(key, ar.value(key).toString());

    ar = hash.value("groups").toObject();
    foreach(QString key, ar.keys())
        addGroup(key, ar.value(key).toString());
}
void WebApi::toJson(QJsonObject & root) {
    QJsonObject friendsJson;
    foreach(QString key, friends.keys())
        friendsJson.insert(key, QJsonValue(friends.value(key)));

    root.insert("friends", friendsJson);

    QJsonObject groupsJson;
    foreach(QString key, groups.keys())
        groupsJson.insert(key, QJsonValue(groups.value(key)));

    root.insert("groups", groupsJson);
}

void WebApi::showingCaptcha() {
    captchaDialog -> clearText();
    captchaDialog -> exec();
}

void WebApi::startApiCall(QFuture<ApiFuncContainer *> feature) {
    QFutureWatcher<ApiFuncContainer *> * initiator = new QFutureWatcher<ApiFuncContainer *>();
    connect(initiator, SIGNAL(finished()), this, SLOT(apiCallFinished()));
    initiator -> setFuture(feature);
}

void WebApi::apiCallFinished() {
    QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *)sender();
    ApiFuncContainer * func = initiator -> result();
    connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
    emit routineFinished(func -> result);
    disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
    delete func;
}
