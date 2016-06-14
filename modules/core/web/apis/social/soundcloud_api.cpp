#include "soundcloud_api.h"

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core::Web::Soundcloud;

QString Api::authUrl() {
    QUrl url(url_connect);

    QUrlQuery query = genDefaultParams();
    setParam(query, tkn_response_type, val_response_type);
    setParam(query, tkn_scope, val_scope);
    setParam(query, tkn_redirect_uri, url_redirect);
    setParam(query, tkn_display, val_display);

    url.setQuery(query);
    return url.toString();
}

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

void Api::getGroupInfo(QString uid, QJsonObject & object) {
    object.insert(tkn_audio_list, groupAudio(uid));
    object.insert(tkn_playlist, groupPlaylists(uid));
}

void Api::getUserInfo(QString & uid, QJsonObject & object) {
    object.insert(tkn_audio_list, userAudio(uid));
    object.insert(tkn_playlist, userPlaylists(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(tkn_followings, userFollowings(uid)); // return bad request error
    object.insert(tkn_followers, userFollowers(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(tkn_groups, userGroups(uid));
}


QJsonObject Api::objectInfo(QString & uid) {
    QJsonObject res;

    if (uid[0] == '-')
        getGroupInfo(uid.mid(1), res);
    else
        getUserInfo(uid, res);

    return res;
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
        button -> setIcon(QIcon(QStringLiteral(":/add_soundcloud_on")));
        button -> setToolTip(QStringLiteral("Soundcloud(soundcloud.com)"));
        button -> setPopupMode(QToolButton::InstantPopup);

        QMenu * menu = new QMenu(button);
        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
        menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(openRelationTab()));
        button -> setMenu(menu);
    } else {
        button -> setIcon(QIcon(QStringLiteral(":/add_soundcloud")));
        button -> setToolTip(QStringLiteral("Connect to Soundcloud(soundcloud.com)"));
        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    return button;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUserApi() {
    QUrl auth_url = authUrl();
    QUrl form_url = auth_url;
    Response * resp = Manager::prepare() -> getFollowed(form_url);
    QHash<QString, QString> vals;

    while(true) {
        QString err;
        Html::Document html = resp -> toHtml();

        Html::Tag * form = html.findFirst("form.authorize-token");

        if (!form) {
            Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
            return false;
        }

        if (form -> has("input[name='password']")) { // if user not authorized
            err = html.find(".warning").text();

            QString captcha_src;
            Html::Set captcha_set = form -> find(QString("script[src^'" + RECAPTCHA_BASE_URL + "']").toUtf8().data());

            if (!captcha_set.isEmpty())
                captcha_src = captcha_set.first() -> value(tkn_src);

            if (captcha_src.isEmpty()) {
                if (!showingLogin(val_auth_title, vals[tkn_username], vals[tkn_password], err))
                    return false;
            } else {
                if (!showingLoginWithCaptcha(
                    val_auth_title, Recaptcha::V1::obj().takeImageUrl(captcha_src, vals[tkn_recaptcha_challenge_field]),
                    vals[tkn_username], vals[tkn_password], vals[tkn_recaptcha_response_field], err
                )) return false;
            }
        }

        form_url = form -> serializeFormToUrl(vals, true);

        if (form_url.isRelative())
            form_url = auth_url.resolved(form_url);

        QHash<QString, QString> headers;
        headers.insert(tkn_referer, form_url.toString());
        resp = Manager::prepare() -> formFollowed(form_url, headers);

        QUrlQuery query(resp->toUrl(false).query());

        if (query.hasQueryItem(tkn_error)) {
            error = query.queryItemValue(tkn_error_description);
            qDebug() << "ERRRRRRRRRR" << error;
            resp -> deleteLater();
            return false;
        } else if (query.hasQueryItem(tkn_code)) {
            QJsonObject doc = Web::Manager::prepare() -> formFollowed(authTokenUrl(), authTokenUrlParams(query.queryItemValue(tkn_code))) -> toJson();

            if (doc.contains(tkn_access_token)) {
                QString newToken = doc.value(tkn_access_token).toString();
                doc = Web::Manager::prepare() -> jsonGet(confirmAuthUrl(newToken));

                setParams(newToken, QString::number(doc.value(tkn_id).toInt()), QString());
                resp -> deleteLater();
                return true;
            }
        }
    }
}
