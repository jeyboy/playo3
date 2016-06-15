#include "od_api.h"
#include "dockbars.h"

using namespace Core::Web::Od;

QString Api::refresh(const QString & refresh_page) { // here refresh_page must by eq to track id
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

QToolButton * Api::initButton(QWidget * parent) {
    if (button == 0) {
        if (!parent) {
            qDebug() << "PIPEC" << name();
            return 0;
        }
        button = new QToolButton(parent);
    } else {
        button -> setMenu(0);
        disconnect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    if (isConnected()) {
        button -> setIcon(QIcon(QStringLiteral(":/add_od_on")));
        button -> setToolTip(QStringLiteral("Od(ok.ru)"));
        button -> setPopupMode(QToolButton::InstantPopup);

        QMenu * menu = new QMenu(button);
        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
//        menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(showSoundcloudRelTabDialog()));
        button -> setMenu(menu);
    } else {
        button -> setIcon(QIcon(QStringLiteral(":/add_od")));
        button -> setToolTip(QStringLiteral("Connect to Od(ok.ru)"));
        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    return button;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUserSite() {
    bool auth_res = formConnection();
    if (auth_res) {
        setParams(grabSID(), userID(), additional());
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
            setParams(QString(), grabUserId(doc), hash_key);
            break;
        }
        else err = doc.find(".anonym_e").text();
    }

    return true;
}
