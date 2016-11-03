#ifndef YOUTUBE_REQUEST_API
#define YOUTUBE_REQUEST_API

#include "youtube_auth.h"
#include "youtube_channel.h"
#include "youtube_playlist.h"
#include "youtube_video.h"
#include "youtube_set.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

////#include "modules/core/web/interfaces/teu_auth.h"
////#include "modules/core/web/apis/service/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Requests : public Sociable, public Auth, public Playlist, public Video, public Channel, public Set {
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
                    QJsonObject error = json.value(LSTR("error")).toObject();
                    if (error.isEmpty()) {
                        arg -> prepareRequestUrlByToken(
                            json.value(LSTR("nextPageToken")).toString()
                        );
                        return true;
                    } else {
                        code = error.value(LSTR("code")).toInt();
                        message = error.value(LSTR("message")).toString();
                        return false;
                    }
                }
                bool endReached(QJsonObject & response, QueriableArg * /*arg*/) {
                    return JSON_STR(response, LSTR("nextPageToken")).isEmpty();
                }
            protected:
                Requests() {
                    setSociableLimitations(true, true, false, false);

                    flags = {
                        {sf_endpoint, (SourceFlags) (
                            sf_is_primary | sf_is_content_shareable | sf_video | sf_user |
                            sf_feed | sf_playlist | sf_compilation | sf_api | sf_api_connectable
                            /* | sf_site | sf_site_connectable*/
                         )
                        },

                        {sf_feed,                       sf_api},

                        {sf_feed_by_user,               sf_both_auth},

                        {sf_search,                     sf_api},

                        {sf_compilation,                sf_api},

//                        {sf_video_playlist_by_id,       sf_site},
                        {sf_video_playlist_by_user,     sf_both_auth},

                        {sf_video_by_id,                sf_api},
                        {sf_video_by_title,             sf_api},
                        {sf_video_by_user,              sf_both_auth},
                        {sf_video_by_category,          sf_api},
                        {sf_video_by_playlist,          sf_api},

                        {sf_user_sociable,              sf_api},

//                        {sf_user_by_id,                 sf_api},
                        {sf_user_by_title,              sf_api},
                    };
                }

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) {
                    return url_api_base % predicate;
                }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api) {
                    QUrlQuery query;
                    query.addQueryItem(tkn_key, val_client_tkn);
                    return query;
                }

                bool connectUserApi() {
                    QString token, refresh_token;
                    qint64 expiration;

                    if (connectApi(token, refresh_token, expiration, error)) {
                        setApiToken(token);
                        setApiRefreshToken(refresh_token);
                        setApiExpiration(expiration);
                        setApiUserID(LSTR("-1"));
                        return true;
                    }

                    return false;
                }
//                bool connectUserSite() {
//                    setSiteUserID(apiUserID()); // use api user id
//                    return true; /*return connectSite(error);*/
//                }

                void saveAdditionals(QJsonObject & obj) {
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_api_base));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
//                    clearGroups();
                    Manager::removeCookies(url_api_base);
                }
                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = JSON_OBJ((*obj_iter).toObject(), tkn_snippet);

                        QString id = JSON_CSTR2(obj, LSTR("resourceId"), LSTR("channelId"));
                        if (id.isEmpty()) id = JSON_CSTR(obj, LSTR("channelId"));

                        linkables << Linkable(
                            id,
                            JSON_STR(obj, LSTR("title")),
                            JSON_STR(obj, LSTR("description")),
                            JSON_STR(
                                JSON_OBJ2(obj, LSTR("thumbnails"), LSTR("default")),
                                LSTR("url")
                            )
                        );
                    }
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;
                    jsonToUsers(linkables, EXTRACT_ITEMS(videoChannelsById(uid).toObject()));
                    return linkables;
                }

                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;
                    jsonToUsers(linkables, EXTRACT_ITEMS(videoChannelsByName(name).toObject()));
                    return linkables;
                }
            public:               
                inline virtual ~Requests() {}

                QJsonValue popular(const SearchLimit & limits) {
                    QueriableResponse response;

                    response = pRequest(
                        videosUrl(),
                        call_type_json,
                        rules()
                    );

                    return prepareBlock(dmt_set, cmd_mtd_set_by_type, response, limits, {}/*, {{CMD_SET_TYPE, set_type}}*/);
                }

                QJsonValue searchProc(const SearchLimit & limits) { //count = 5
                    QJsonArray res;

                    res << videoSearch(limits);

                    return res;
                }

                inline QString refresh(const QString & path, const DataMediaType & /*itemMediaType*/) { return idToUrl(path); }

                QJsonValue userInfo(const QString & user_id) {
                    QJsonArray blocks;

                    if (user_id == apiUserID()) {
                        clearFriends();
                        QJsonObject subscriptions = videoChannelsByUser(user_id).toObject();
                        jsonToUsers(Friendable::linkables, EXTRACT_ITEMS(subscriptions));
                    }

                    blocks << videoByUser(user_id);

                    return blocks;
                }
            };
        }
    }
}

#endif // YOUTUBE_REQUEST_API
