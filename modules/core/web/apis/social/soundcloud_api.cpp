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
    WebApi::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    hash.insert(name(), root);
}

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

void Api::getGroupInfo(QString uid, QJsonObject & object) {
//    uid = "101";
    object.insert(Soundcloud::audio_list_key, groupAudio(uid));
    object.insert(Soundcloud::playlist_key, groupPlaylists(uid));
}

void Api::getUserInfo(QString & uid, QJsonObject & object) {
    object.insert(Soundcloud::audio_list_key, userAudio(uid));
    object.insert(Soundcloud::playlist_key, userPlaylists(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(Soundcloud::followings_key, userFollowings(uid)); // return bad request error
    object.insert(Soundcloud::followers_key, userFollowers(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(Soundcloud::groups_key, userGroups(uid));
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
bool Api::connection() { //TODO: not finished
    if (isConnected()) return true;

    QUrl auth_url = authUrl();
    QUrl form_url = auth_url;
    Response * resp = Manager::prepare() -> followedGet(form_url);
    QHash<QString, QString> vals;

    while(true) {
        qDebug() << "----------------------------------------------------";
        QString err;
        Html::Document html = resp -> toHtml();

        html.output();

        Html::Set forms = html.find("form.authorize-token");

        if (forms.isEmpty()) {
            Logger::obj().write("Soundcloud auth", QStringLiteral("Auth form did not found"), true);
            return false;
        }

        Html::Tag * form = forms.first();

        if (form -> has("input[name='password']")) { // if user not authorized
            err = html.find(".warning").text();
            if (!showingLogin(vals[QStringLiteral("username")], vals[QStringLiteral("password")], err))
                return false;           

//            script ||| [(type : text/javascript)(src : https://www.google.com/recaptcha/api/challenge?k=6LeABsUSAAAAABLOEF92U0unfhlGLynYlhvJRFue)]

//            Html::Set captcha_set = form -> find("script[src^'https://www.google.com/recaptcha/api/challenge']");
            Html::Set captcha_set = form -> find(QString("script[src^'https://www.google.com/recaptcha/api/challenge']").toUtf8().data());
            if (!captcha_set.isEmpty()) {
                QString captcha_src = captcha_set.first() -> value("src");
                qDebug() << "CAPTCHA" << captcha_src;

                if (!showingCaptcha(Recaptcha::V1::obj().takeImageUrl(captcha_src, vals["recaptcha_challenge_field"]), vals["recaptcha_response_field"]))
                    return false;
            }
        }

        form_url = form -> serializeFormToUrl(vals, true);

        if (form_url.isRelative())
            form_url = auth_url.resolved(form_url);

        qDebug() << "FORM_URL" << form_url;

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






//    QUrl url(Web::Soundcloud::Api::obj().authUrl());
//    qDebug() <<  url.toString();


//    QHash<QString, QString> vals;
//    Soundcloud::Api::obj().showingLogin(vals[QStringLiteral("username")], vals[QStringLiteral("password")]);
//    QUrl form_url;

//////    for(int i = 0; i < 2; i++) {
//    Web::Response * res = Web::Manager::prepare() -> followedGet(url);

//    // repeat this

//    Html::Document doc = res -> toHtml(false);
//    doc.output();


//    Html::Set set = doc.find("form.log-in");
//    if (set.isEmpty()) {
//        set = doc.find("form.authorize-token");
//    }

//    if (!set.isEmpty()) {
//        Html::Set captcha_set = doc.find("#recaptcha_image");
//        if (!captcha_set.isEmpty()) {
//            QString captcha_src = captcha_set.find("img").value("src");

//            if (!Web::Soundcloud::Api::obj().showingCaptcha(QUrl(captcha_src), vals["recaptcha_response_field"]))
//                return;
//        }

//        form_url = set.first() -> serializeFormToUrl(vals);
//        if (form_url.isRelative())
//            form_url = url.resolved(form_url);
//        Web::Response * resp = Web::Manager::prepare() -> followedForm(form_url);

//        qDebug() << "SOSO $$$" << resp -> toUrl(false);
//    }
//    else qDebug() << "PPPPPPPIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII";

//    // if url is not contain access_token


//    res -> deleteLater();









//    QUrlQuery query(url.query());

//    if (query.hasQueryItem(QStringLiteral("error"))) {
//        error = query.queryItemValue(QStringLiteral("error_description"));
//        emit responseReady(QStringLiteral("reject"));
//    } else if (query.hasQueryItem(QStringLiteral("code"))) {
//        QJsonObject doc = Web::Manager::prepare() -> followedForm(authTokenUrl(), authTokenUrlParams(query.queryItemValue(QStringLiteral("code")))) -> toJson();

//        if (doc.contains(QStringLiteral("access_token"))) {
//            QString newToken = doc.value(QStringLiteral("access_token")).toString();
//            doc = Web::Manager::prepare() -> getJson(confirmAuthUrl(newToken));

//            setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), QString());
//            emit authorized();
//            emit responseReady(QStringLiteral("accept"));
//        }
//        else emit responseReady(QStringLiteral("reject"));
//    }
//    else emit responseReady("");
}
