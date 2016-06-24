#include "vk_api.h"

#include <qmenu.h>

using namespace Core::Web::Vk;

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
        button -> setIcon(QIcon(QStringLiteral(":/add_vk_on")));
        button -> setPopupMode(QToolButton::InstantPopup);
        button -> setToolTip(QStringLiteral("VKontakte(vk.com)"));

        QMenu * menu = new QMenu(button);
        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
        menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(openRelationTab()));
        menu -> addAction(QStringLiteral("Open recommendations"), this, SLOT(openRecomendations()));
        button -> setMenu(menu);
    } else {
        button -> setIcon(QIcon(QStringLiteral(":/add_vk")));
        button -> setToolTip(QStringLiteral("Connect to VKontakte(vk.com)"));
        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    return button;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUserApi() {
    QUrl form_url = authUrl();
    qDebug() << form_url;

    while(true) {
        Response * resp = Manager::prepare() -> getFollowed(form_url);
        QString err;
        Html::Document html = resp -> toHtml(false);
        html.output();


        if (html.has("input[name='pass']")) { // if user not authorized
            resp -> deleteLater();
            QHash<QString, QString> vals;
            err = html.find(".service_msg_warning").text();

            Html::Tag * form = html.findFirst("form");

            if (!form) {
                Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
                return false;
            }

            QString captcha_src;
            Html::Set captcha_set = form -> find("img#captcha");

            if (!captcha_set.isEmpty())
                captcha_src = captcha_set.first() -> value("src");

            if (captcha_src.isEmpty()) {
                if (!showingLogin(val_auth_title, vals[tkn_email], vals[tkn_password], err))
                    return false;
            } else {
                if (!showingLoginWithCaptcha(val_auth_title, captcha_src,
                    vals[tkn_email], vals[tkn_password], vals[tkn_captcha], err
                )) return false;
            }

            form_url = form -> serializeFormToUrl(vals);
            qDebug() << form_url;
            resp = Manager::prepare() -> formFollowed(form_url);
            html.output();
        } else return false; // something went wrong

        form_url = resp -> toUrl();
        QUrlQuery query(form_url.fragment());

        if (query.hasQueryItem(tkn_error)) {
            error = query.queryItemValue(tkn_error_description);
            return false;
        } else if (query.hasQueryItem(tkn_access_token)) {
            setApiToken(query.queryItemValue(tkn_access_token));
            setApiUserID(query.queryItemValue(tkn_user_id));
            setApiExpiration(query.queryItemValue(tkn_expires_in));

//            setParams(
//                query.queryItemValue(tkn_access_token),
//                query.queryItemValue(tkn_user_id),
//                query.queryItemValue(tkn_expires_in)
//            );
            return true;
        }
        else form_url = authUrl();
    }
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool Api::extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
    QJsonObject stat_obj = json.value(tkn_error).toObject();
    message = stat_obj.value(tkn_error_msg).toString();
    code = stat_obj.value(tkn_error_code).toInt();

    if (code == 14) {
        if (Settings::obj().isIgnoreCaptcha())
            return false;
        else
            return captchaProcessing(json, arg -> request_url);
    } else return code == 0;
}

bool Api::captchaProcessing(QJsonObject & json, QString & url_str) {
    QJsonObject stat_obj = json.value(tkn_error).toObject();

    QString captchaText;
    showingCaptcha(QUrl(stat_obj.value(tkn_captcha_img).toString()), captchaText);
    if (captchaText.isEmpty())
        return false;

    QUrl url(url_str);
    QUrlQuery query(url.query());
    query.removeQueryItem(tkn_captcha_sid);
    query.removeQueryItem(tkn_captcha);

    query.addQueryItem(tkn_captcha_sid, stat_obj.value(tkn_captcha_sid).toString());
    query.addQueryItem(tkn_captcha, captchaText);

    url.setQuery(query);

    return sRequest(url.toString(), json, call_type_json);
//    return sQuery(url, json);
}
