#ifndef SOUNDCLOUD_REQUESTS
#define SOUNDCLOUD_REQUESTS

#include "api/requests.h"
#include "site/requests.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/interfaces/iuser_interaction.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Requests : public ISource, public IUserInteraction, public Sociable, public Api::Requests, public Site::Requests {
                void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        QString full_name = obj.value(tkn_full_name).toString();
                        QString perma = obj.value(tkn_permalink).toString();

                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            full_name.isEmpty() ? obj.value(tkn_username).toString() : full_name,
                            perma,
                            obj.value(tkn_avatar_url).toString()
                        );
                    }
                }

                void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            obj.value(QStringLiteral("name")).toString(),
                            obj.value(tkn_permalink).toString(),
                            obj.value(tkn_artwork_url).toString()
                        );
                    }
                }
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | sf_auth_site_has | sf_site_offline_credentials_req |
                        sf_content_audio_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable |
                        sf_groups_serachable | sf_tags_serachable | sf_genres_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_charteable |
                        sf_recomendable_by_item | sf_newable | sf_taggable | sf_genreable |
                        sf_api_auth_mandatory |
                        sf_site_recomendations_auth_only | sf_site_user_content_auth_only
                    );
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api_def: return url_api_base % predicate % val_default_format;
                        case qst_site_def: return url_api_base % predicate;
                        case qst_site_alt1: return url_api_base2 % predicate;
                    }
                }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    return response.value(tkn_response).toArray().size() < arg -> per_request_limit;
                }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & json, int & code, QString & message) {
                    QJsonObject stat_obj = json.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
                    message = stat_obj.value(tkn_error_message).toString();
                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                }

//                inline QHash<QString, QString> customsList() { return QHash<QString, QString>(); }
//                inline QJsonValue procCustom(const QString & /*custom_params*/) { return QJsonArray(); }

                bool connectUserApi() {
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

                bool connectUserSite() {
                //    curl 'https://sign-in.soundcloud.com/sign-in/password?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1467192015' -X POST -H
                //    'DNT: 1' -H 'Host: sign-in.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com'

                //    payload:

                //        client_id
                //            "02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea"

                //        credentials
                //            Object { identifier="jeyboy1985@gmail.com",  password="5675675675"}

                //        identifier
                //            "jeyboy1985@gmail.com"

                //        password
                //            "5675675675"

                //        recaptcha_response
                //            ""

                //        scope
                //            "fast-connect non-expiring purchase signup upload"



                //    respond:

                //        {"session":{"access_token":"1-138878-99021496-..."},"sign_in":{"user_urn":"soundcloud:users:9902","method":"password"}}

                    return false;
                }

                bool hasOfflineCredentials()     { return !siteToken().isEmpty(); }
                bool takeOfflineCredentials()    {
                    if (hasOfflineCredentials()) return true;

                    QString html = Manager::prepare() -> getFollowed(url_site_base) -> toText();
                    QRegularExpression reg("sc_version = \"(\\d+)\"");
                    QRegularExpressionMatch match = reg.match(html);

                    if (match.hasMatch()) {
                        QString app_id = match.captured(1);
                        setSiteHash(app_id);

                        QRegularExpression js_reg("(http[:\\/\\w\\.\\-]+app-[\\w-]+\\.js)");
                        match = js_reg.match(html);
                        if (match.hasMatch()) {
                            QString js_url = match.captured(1);
                            QString js_content = Manager::prepare() -> getFollowed(js_url) -> toText();

                            QRegularExpression id("client_id:\"(\\w+)\"");
                            match = id.match(js_content);
                            if (match.hasMatch()) {
                                QString token = match.captured(1);
                                setSiteToken(token);
                                return true;
                            }
                        }
                    }

                    return false;
                }

                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    takeOfflineCredentials();
                }
                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

            public:
                inline virtual ~Requests() {}

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_json_def) {
                    switch(stype) {
                        case qst_json_def:
                            return QUrlQuery(tkn_client_id % val_id_tkn);
                        case qst_html_def:
                        case qst_html_alt1:
                            return QUrlQuery(tkn_client_id % siteToken() % QStringLiteral("app_version=") % siteHash());
                        default: return QUrlQuery();
                    }
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = userById(uid);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name, 400);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupById(uid);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name, 400);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUESTS
