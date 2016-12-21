#ifndef YANDEX_REQUESTS_H
#define YANDEX_REQUESTS_H

#include "yandex_layer.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Requests : public Layer {

                inline bool endReached(QJsonObject & response, QueriableArg * /*arg*/) {
                    QJsonObject pager_obj = JSON_OBJ(response, tkn_pager);
                    return (JSON_INT(pager_obj, tkn_page) + 1) * JSON_INT(pager_obj, LSTR("perPage")) >= JSON_INT(pager_obj, LSTR("total"));
                }
                inline bool extractStatus(QueriableArg * arg, QJsonObject & json, int & /*code*/, QString & /*message*/) {
                    while(true) { // INFO: check captcha validation
                        if (JSON_HAS_KEY(json, JSON_ERR_FIELD)) {
                            Html::Document doc(JSON_STR(json, JSON_ERR_FIELD));

                            Html::Tag * form_tag = doc.findFirst(".form form");

                            if (!form_tag) {
                                qCritical() << "Captcha form is not found";
                                return false;
                            }

                            QString captcha_img = form_tag -> findFirst("img") -> src();

                            QHash<QString, QString> fields;
                            showingCaptcha(QUrl(captcha_img), fields[tkn_rep]);
                            if (fields[tkn_rep].isEmpty())
                                return false;

                            QUrl captcha_proc_url = form_tag -> serializeFormToUrl(fields);
                            if(captcha_proc_url.isRelative())
                                captcha_proc_url = QUrl(baseUrlStr(qst_site_base)).resolved(captcha_proc_url);
                            json = Manager::prepare() -> jsonGet(captcha_proc_url, arg -> headers, arg -> post_proc & proc_json_wrap);
                        }
                        else return true;
                    }
                }

                void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
//                    if (arr.size() == 0 || (arr.size() == 1 && arr[0].toObject().contains(LSTR("errors")))) return;

                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        QString perma = JSON_STR(obj, tkn_login);

                        linkables << Linkable(
                            perma/*JSON_CSTR(obj, tkn_uid)*/,
                            JSON_STR(obj, tkn_name),
                            perma,
                            JSON_STR(obj, tkn_coverUri)
                        );
                    }
                }
            protected:
                Requests() {
//                    setSociableLimitations(true, true, true, true);
                }
                virtual ~Requests() {}

                inline QString siteAdditionalToken() {
                    if (ISource::siteAdditionalToken().isEmpty())
                        setSiteAdditionalToken(sign());

                    return ISource::siteAdditionalToken();
                }

                void saveAdditionals(QJsonObject & obj) {
                    setSiteAdditionalToken(QString());
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(baseUrlStr(qst_site_base)));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(QUrl(baseUrlStr(qst_site_base)));
                }

                bool connectUserSite() {
                    QString user_id;

                    if (siteConnection(user_id, error)) {
                        setSiteUserID(user_id);

                        return true;
                    }
                    else return false;
                }

                bool hasOfflineCredentials()     { return false; /*!siteToken().isEmpty();*/ }
                bool takeOfflineCredentials() {
                    if (hasOfflineCredentials()) return true;

//                    QString html = Manager::prepare() -> getFollowed(url_site_base) -> toText();
//                    QRegularExpression reg("sc_version = \"(\\d+)\"");
//                    QRegularExpressionMatch match = reg.match(html);

//                    if (match.hasMatch()) {
//                        QString app_id = match.captured(1);
//                        setSiteHash(app_id);

//                        QRegularExpression js_reg("(http[:\\/\\w\\.\\-]+app-[\\w-]+\\.js)");
//                        match = js_reg.match(html);
//                        if (match.hasMatch()) {
//                            QString js_url = match.captured(1);
//                            QString js_content = Manager::prepare() -> getFollowed(js_url) -> toText();

//                            QRegularExpression id("client_id:\"(\\w+)\"");
//                            match = id.match(js_content);
//                            if (match.hasMatch()) {
//                                QString token = match.captured(1);
//                                setSiteToken(token);
//                                return true;
//                            }
//                        }
//                    }

                    return false;
                }


                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate = QString()) {
                    QString locale = siteLocale(const_default_locale); // FIXME: need to correctly determinate locale for not authed users

                    //FIXME: if user opens page with logged in account in browser - our cookies is automatically expired

//                    QString user_id;
//                    if (checkConnection(user_id))
//                        setSiteUserID(user_id);
//                    else {
//                        qCritical() << "YANDEX AUTH MISSED";
//                    }

                    switch(stype) {
                        case qst_site: {        return url_site_v1.arg(locale) % predicate; }
                        case qst_site_alt1: {   return url_site_v2.arg(locale) % predicate; }
                        case qst_site_alt2: {   return url_site_v2_1.arg(locale) % predicate; }
                        default: {              return url_root.arg(locale) % predicate; }
                    }
                }
                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api) {
                    switch(stype) {
                        case qst_site:
                        case qst_site_alt1:
                        case qst_site_alt2:
                            return QUrlQuery(
                                QString(
                                    LSTR("external-domain=") % LSTR("music.yandex.ua") %
                                    LSTR("&overembed=") % const_false %
                                    LSTR("&lang=") % siteLocale(const_default_locale)
                                )
                            );

                        default: return QUrlQuery();
                    }
                }
            public:
                QString refresh(const QString & id, const DataMediaType & itemMediaType) {
                    switch(itemMediaType) {
                        case dmt_audio: return audioUrl(id);
                        case dmt_video: return videoUrl(id);
                        default: return QString();
                    }
                }

                QJsonValue popular(const SearchLimit & limits) {
                    return setByType(set_popular_tracks, limits);
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonArray blocks;

                        if (limits.include_audio()) {
                            if (limits.by_artists())
                                blocks << artistsSearch(limits);

                            if (limits.by_songs_name() || limits.by_titles())
                                blocks << audioSearch(limits);

                            if (limits.by_sets()) {
                                blocks << albumsSearch(limits);
                                blocks << playlistsSearch(limits);
                            }
                        }

                        if (limits.include_video())
                            blocks << videoSearch(limits);

                        return blocks;
                    }
                }

                QJsonValue userInfo(const QString & user_id) {
                    SourceFlags perm = permissions(sf_user_by_id);

                    if (perm > sf_none) {
                        if (user_id == userID()) {
                            clearFriends();
                            jsonToUsers(Friendable::linkables, EXTRACT_ITEMS(userFollowers(user_id).toObject()));
                            jsonToUsers(Friendable::linkables, EXTRACT_ITEMS(userFollowings(user_id).toObject()));
                        }

                        return userMedia(user_id);
                    }

                    return QJsonArray();
                }

                QJsonValue userMedia(const QString & user_id) {
                    SourceFlags perm = permissions(sf_audio_by_user);
                    QJsonArray blocks;

                    if (perm > sf_none) {
                        QJsonObject tracks = audioByUser(user_id).toObject();
                        tracks.insert(tkn_dir_name, LSTR("Tracks"));
                        blocks << tracks;

                        QJsonObject playlists = playlistsByUser(user_id).toObject();
                        playlists.insert(tkn_dir_name, LSTR("Playlists"));
                        blocks << playlists;
                    }

                    return blocks;
                }
            };
        }
    }
}

#endif // YANDEX_REQUESTS_H
