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

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
    Sociable::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    Sociable::toJson(root);

    hash.insert(name(), root);
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

void Api::openTab() {
    if (connectUser(connection_manual))
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(userID())),
            Models::Params(siteType(), userID()), 0, true, true
        );
}

void Api::openRelationTab() {
    RelationsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
            uidStr(dialog.getId())),
            Models::Params(siteType(), dialog.getId(), rel_user), 0, true, true
        );
}


///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connectUser(const ConnectionType & /*conType*/) {
    if (isConnected()) return true;

    QUrl auth_url = authUrl();
    QUrl form_url = auth_url;
    Response * resp = Manager::prepare() -> followedGet(form_url);
    QHash<QString, QString> vals;

    while(true) {
        QString err;
        Html::Document html = resp -> toHtml();

        Html::Set forms = html.find("form.authorize-token");

        if (forms.isEmpty()) {
            Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
            return false;
        }

        Html::Tag * form = forms.first();

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
        resp = Manager::prepare() -> followedForm(form_url, headers);

        QUrlQuery query(resp->toUrl(false).query());

        if (query.hasQueryItem(tkn_error)) {
            error = query.queryItemValue(tkn_error_description);
            qDebug() << "ERRRRRRRRRR" << error;
            resp -> deleteLater();
            return false;
        } else if (query.hasQueryItem(tkn_code)) {
            QJsonObject doc = Web::Manager::prepare() -> followedForm(authTokenUrl(), authTokenUrlParams(query.queryItemValue(tkn_code))) -> toJson();

            if (doc.contains(tkn_access_token)) {
                QString newToken = doc.value(tkn_access_token).toString();
                doc = Web::Manager::prepare() -> getJson(confirmAuthUrl(newToken));

                setParams(newToken, QString::number(doc.value(tkn_id).toInt()), QString());
                emit authorized();
                resp -> deleteLater();
                return true;
            }
        }
    }
}
