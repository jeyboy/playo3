#include "soundcloud_api.h"

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core::Web::Soundcloud;

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

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

        resp = Manager::prepare() -> formFollowed(form_url, {{tkn_referer, form_url.toString()}});

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

                setApiToken(newToken);
                setApiUserID(QString::number(doc.value(tkn_id).toInt()));
//                setParams(newToken, QString::number(doc.value(tkn_id).toInt()), QString());
                resp -> deleteLater();
                return true;
            }
        }
    }
}
