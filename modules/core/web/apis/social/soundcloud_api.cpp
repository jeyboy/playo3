#include "soundcloud_api.h"

using namespace Core::Web::Soundcloud;

QString Api::authUrl() {
    QUrl url(QStringLiteral("https://soundcloud.com/connect"));

    QUrlQuery query = genDefaultParams();
    setParam(query, QStringLiteral("response_type"), QStringLiteral("code"));
    setParam(query, QStringLiteral("scope"), QStringLiteral("non-expiring"));
    setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
    setParam(query, QStringLiteral("display"), QStringLiteral("popup"));

    url.setQuery(query);
    return url.toString();
}

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
    Friendable::fromJson(obj);
    Groupable::fromJson(obj);
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

void Api::getGroupInfo(QString uid, QJsonObject & object) {
    object.insert(audio_list_key, groupAudio(uid));
    object.insert(playlist_key, groupPlaylists(uid));
}

void Api::getUserInfo(QString & uid, QJsonObject & object) {
    object.insert(audio_list_key, userAudio(uid));
    object.insert(playlist_key, userPlaylists(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(followings_key, userFollowings(uid)); // return bad request error
    object.insert(followers_key, userFollowers(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(groups_key, userGroups(uid));
}


QJsonObject Api::objectInfo(QString & uid) {
    QJsonObject res;

    if (uid[0] == '-')
        getGroupInfo(uid.mid(1), res);
    else
        getUserInfo(uid, res);

    return res;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::connection() {
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
            Logger::obj().write("Soundcloud auth", QStringLiteral("Auth form did not found"), true);
            return false;
        }

        Html::Tag * form = forms.first();

        if (form -> has("input[name='password']")) { // if user not authorized
            err = html.find(".warning").text();

            QString captcha_src;
            Html::Set captcha_set = form -> find(QString("script[src^'" + RECAPTCHA_BASE_URL + "']").toUtf8().data());

            if (!captcha_set.isEmpty())
                captcha_src = captcha_set.first() -> value("src");

            if (captcha_src.isEmpty()) {
                if (!showingLogin(QStringLiteral("Soundcloud auth"), vals[QStringLiteral("username")], vals[QStringLiteral("password")], err))
                    return false;
            } else {
                if (!showingLoginWithCaptcha(
                    QStringLiteral("Soundcloud auth"), Recaptcha::V1::obj().takeImageUrl(captcha_src, vals["recaptcha_challenge_field"]),
                    vals[QStringLiteral("username")], vals[QStringLiteral("password")], vals["recaptcha_response_field"], err
                )) return false;
            }
        }

        form_url = form -> serializeFormToUrl(vals, true);

        if (form_url.isRelative())
            form_url = auth_url.resolved(form_url);

        QHash<QString, QString> headers;
        headers.insert(QStringLiteral("Referer"), form_url.toString());
        resp = Manager::prepare() -> followedForm(form_url, headers);

        QUrlQuery query(resp->toUrl(false).query());

        if (query.hasQueryItem(QStringLiteral("error"))) {
            error = query.queryItemValue(QStringLiteral("error_description"));
            qDebug() << "ERRRRRRRRRR" << error;
            resp -> deleteLater();
            return false;
        } else if (query.hasQueryItem(QStringLiteral("code"))) {
            QJsonObject doc = Web::Manager::prepare() -> followedForm(authTokenUrl(), authTokenUrlParams(query.queryItemValue(QStringLiteral("code")))) -> toJson();

            if (doc.contains(QStringLiteral("access_token"))) {
                QString newToken = doc.value(QStringLiteral("access_token")).toString();
                doc = Web::Manager::prepare() -> getJson(confirmAuthUrl(newToken));

                setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), QString());
                emit authorized();
                resp -> deleteLater();
                return true;
            }
        }
    }
}
