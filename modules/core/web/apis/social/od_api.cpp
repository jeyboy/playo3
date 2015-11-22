#include "od_api.h"

using namespace Core::Web::Od;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();

    TeuAuth::fromJson(obj);
    Sociable::fromJson(obj);

    Manager::loadCookies(obj);

    if (!connection(true)) disconnect();
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    Sociable::toJson(root);

    Manager::saveCookies(root, QUrl(base_url));

    hash.insert(name(), root);
}

QString Api::refresh(QString refresh_page) { // here refresh_page must by eq to track id
    QJsonObject obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
    if (hasError(obj)) {
        connection(true);
        obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
        qDebug() << "RECONECTION";
    }
    QUrl url(obj.value(QStringLiteral("play")).toString());
    QUrlQuery query = QUrlQuery(url.query());
    query.addQueryItem(QStringLiteral("clientHash"), calcMagicNumber(query.queryItemValue(QStringLiteral("md5"))));
    url.setQuery(query);
    return url.toString();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connection(bool onlyAuto) {
    if (!additional().isEmpty())
        setParams(grabSID(), userID(), additional());

    if (sessionIsValid()) // maybe use grabSID() ?
        return true;

    if (onlyAuto) return false;

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
        if (!showingLogin(QStringLiteral("Odnoklassniki auth"), authE, authP, err)) return false;

        Response * reply = Manager::prepare() -> unfollowedForm(authRequestUrl(authE, authP), initHeaders());
        QUrl url = reply -> toRedirectUrl();
        QString hash_key = Manager::paramVal(url, QStringLiteral("httpsdata")); // not used anywhere at this moment

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
