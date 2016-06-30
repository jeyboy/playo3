#include "od_api.h"

using namespace Core::Web::Od;

QString Api::refresh(const QString & refresh_page, const DataMediaType & /*itemMediaType*/) { // here refresh_page must by eq to track id
    QJsonObject obj = Manager::prepare() -> jsonGet(playAudioUrl(refresh_page));
    if (hasError(obj)) {
        connectUser();
        obj = Manager::prepare() -> jsonGet(playAudioUrl(refresh_page));
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
bool Api::formConnection() {
    QString err, authE, authP;

    while(true) {
        if (!showingLogin(val_login_title, authE, authP, err)) return false;

        Response * reply = Manager::prepare() -> form(authRequestUrl(authE, authP), initHeaders());
        QUrl url = reply -> toRedirectUrl();
        QString hash_key = Manager::paramVal(url, tkn_httpsdata); // not used anywhere at this moment

        reply = Manager::prepare() -> getFollowed(url, initHeaders());
        err = reply -> paramVal(tkn_form_error);
        if (!err.isEmpty()) {
            reply -> deleteLater();
            continue;
        }

        Html::Document doc = reply -> toHtml();
        checkSecurity(doc);

        if (!Manager::cookie(tkn_authcode).isEmpty()) {
            setSiteUserID(grabUserId(doc));
            setSiteHash(hash_key);
//            setParams(QString(), grabUserId(doc), hash_key);
            break;
        }
        else err = doc.find(".anonym_e").text();
    }

    return true;
}
