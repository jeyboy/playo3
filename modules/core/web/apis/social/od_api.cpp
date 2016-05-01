#include "od_api.h"

using namespace Core::Web::Od;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();

    TeuAuth::fromJson(obj);
    Sociable::fromJson(obj);

    Manager::loadCookies(obj);

    if (!connectUser(true)) disconnectUser();
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    Sociable::toJson(root);

    Manager::saveCookies(root, QUrl(url_root));

    hash.insert(name(), root);
}

QString Api::refresh(const QString & refresh_page) { // here refresh_page must by eq to track id
    QJsonObject obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
    if (hasError(obj)) {
        connectUser();
        obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
        qDebug() << "RECONECTION";
    }
    QUrl url(obj.value(tkn_play).toString());
    QUrlQuery query = QUrlQuery(url.query());
    query.addQueryItem(tkn_client_hash, calcMagicNumber(query.queryItemValue(tkn_md5)));
    url.setQuery(query);
    return url.toString();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUser(const ConnectionType & conType) {
    if (isConnected()) return true;

    if (conType == connection_restore) return false;

    bool auth_res = formConnection();
    if (auth_res) {
        setParams(grabSID(), userID(), additional());
        emit authorized();
    }

    return auth_res;
}

//bool Api::hashConnection(bool onlyAuto) {
//    Manager::prepare() -> followedGet(initUrl(), initHeaders()) -> deleteLater();

//    if (!sessionIsValid())
//        if (!onlyAuto && !formConnection())
//            return false;

//    setParams(grabSID(), userID(), additional());
//    return true;
//}

bool Api::formConnection() {
    QString err, authE, authP;

    while(true) {
        if (!showingLogin(val_login_title, authE, authP, err)) return false;

        Response * reply = Manager::prepare() -> unfollowedForm(authRequestUrl(authE, authP), initHeaders());
        QUrl url = reply -> toRedirectUrl();
        QString hash_key = Manager::paramVal(url, tkn_httpsdata); // not used anywhere at this moment

        reply = Manager::prepare() -> followedGet(url, initHeaders());
        err = reply -> paramVal(tkn_form_error);
        if (!err.isEmpty()) {
            reply -> deleteLater();
            continue;
        }

        Html::Document doc = reply -> toHtml();
        checkSecurity(doc);

        if (!Manager::cookie(tkn_authcode).isEmpty()) {
            setParams(QString(), grabUserId(doc), hash_key);
            break;
        }
        else err = doc.find(".anonym_e").text();
    }

    return true;
}
