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
                    int i = 0;

//                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
//                        QJsonObject obj = (*obj_iter).toObject();
//                        QString title = JSON_STR(obj, tkn_title);
//                        if (title.isEmpty())
//                            title = JSON_STR_CAT(obj, LSTR("first_name"), ' ', LSTR("last_name"));

//                        linkables << Linkable(
//                            JSON_CSTR(obj, tkn_id),
//                            title,
//                            JSON_STR(obj, tkn_screen_name),
//                            JSON_STR(obj, tkn_photo)
//                        );
//                    }
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

//                    jsonToUsers(linkables, EXTRACT_ITEMS(usersByIdOrPerma(uid).toObject()));

                    return linkables;
                }

                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    //TODO: write me
//                    jsonToUsers(linkables, EXTRACT_ITEMS(usersByName(name).toObject()));

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
                    return videoSearch(limits);

//                    QueriableResponse response = pRequest(
//                        baseUrlStr(qst_api, path_search, {
//                            {tkn_part, tkn_snippet},
//                            {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
//                            {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
//                            {LSTR("safeSearch"), LSTR("none")},
//                            {tkn_video_embedable, const_true}, // any // true
//                            {tkn_type, LSTR("video")}, // channel // playlist // video
//                        //  Your request can also use the Boolean NOT (-) and OR (|) operators to exclude videos or to find videos that are associated with one of several
//                        //  search terms. For example, to search for videos matching either "boating" or "sailing", set the q parameter value to boating|sailing.
//                        //  Similarly, to search for videos matching either "boating" or "sailing" but not "fishing", set the q parameter value to boating|sailing -fishing.
//                        //  Note that the pipe character must be URL-escaped when it is sent in your API request. The URL-escaped value for the pipe character is %7C.
//                            {tkn_q, limits.predicate},
//                        }),

////                                setOrder(query, hottest ? LSTR("rating") : LSTR("relevance"));
////                                setMusicVideoCategory(query);

////                                else if (!relatedVideoId.isEmpty())
////                                    setParam(query, LSTR("relatedToVideoId"), relatedVideoId);
////                        searchUrl(limits.predicate, limits.genre, limits.by_popularity()),
//                        call_type_json,
//                        rules(QString(), limits.items_limit)
//                    );

//                    initDuration(response.content);
//                    return prepareBlock(dmt_video, cmd_mtd_unknown, response, limits);
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
