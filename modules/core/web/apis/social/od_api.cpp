#include "od_api.h"

using namespace Core::Web::Od;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();

    TeuAuth::fromJson(obj);
    Friendable::fromJson(obj);
    Groupable::fromJson(obj);

    if (!sessionIsValid())
        connection(true);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    Friendable::toJson(root);
    Groupable::toJson(root);

    hash.insert(name(), root);
}

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

//    void Api::getGroupInfo(QString uid, QJsonObject & object) {
//    //    uid = "101";
//        object.insert(Soundcloud::audio_list_key, groupAudio(uid));
//        object.insert(Soundcloud::playlist_key, groupPlaylists(uid));
//    }

//    void Api::getUserInfo(QString & uid, QJsonObject & object) {
//        object.insert(Soundcloud::audio_list_key, userAudio(uid));
//        object.insert(Soundcloud::playlist_key, userPlaylists(uid));
//        QThread::msleep(REQUEST_DELAY);
//        object.insert(Soundcloud::followings_key, userFollowings(uid));
//        object.insert(Soundcloud::followers_key, userFollowers(uid));
//        QThread::msleep(REQUEST_DELAY);
//        object.insert(Soundcloud::groups_key, userGroups(uid));
//    }


//    QJsonObject Api::objectInfo(QString & uid) {
//        QJsonObject res;

//        if (uid[0] == '-')
//            getGroupInfo(uid.mid(1), res);
//        else
//            getUserInfo(uid, res);

//        return res;
//    }

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connection(bool onlyAuto) {
    bool res = !onlyAuto || (onlyAuto && !additional().isEmpty()); // TODO: need to check

    if (res) {
        res &= hashConnection(onlyAuto);
        if (res) emit authorized();
    }

    return res;
}

bool Api::hashConnection(bool onlyAuto) {
    Manager::prepare() -> followedGet(initUrl(), initHeaders()) -> deleteLater();

    if (!sessionIsValid())
        if (!onlyAuto && !formConnection())
            return false;

    setParams(grabSID(), userID(), additional());
    return true;
}

bool Api::formConnection() {
    QString err, authE, authP;

    while(true) {
        if (!showingLogin(QStringLiteral("Odnoklassniki auth"), authE, authP, err)) return false;

        Response * reply = Manager::prepare() -> unfollowedForm(authRequestUrl(authE, authP), initHeaders());
        QUrl url = reply -> toRedirectUrl();
        QString hash_key = Manager::paramVal(url, QStringLiteral("httpsdata"));

        reply = Manager::prepare() -> followedGet(url, initHeaders());
        err = reply -> paramVal(QStringLiteral("st.error"));
        if (!err.isEmpty()) {
            reply -> deleteLater();
            continue;
        }

        Html::Document doc = reply -> toHtml();
        checkSecurity(doc);

        if (!Manager::cookie(QStringLiteral("AUTHCODE")).isEmpty()) {
            setParams(QString(), grabUserId(doc), hash_key);
            break;
        }
        else err = doc.find(".anonym_e").text();
    }

    return true;
}
