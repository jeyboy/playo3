#ifndef YOUTUBE_REQUEST_API
#define YOUTUBE_REQUEST_API

#include "youtube_auth.h"
#include "youtube_playlist.h"
#include "youtube_video.h"

//#include <qregexp.h>
////#include "modules/core/web/interfaces/teu_auth.h"
////#include "modules/core/web/apis/service/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Requests : public Auth, public Playlist, public Video {
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
//                    setSociableLimitations(true, true, true, true);

                    flags = {
                        {sf_endpoint, (SourceFlags) (
                            sf_is_primary | sf_is_content_shareable | sf_video | sf_sociable |
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
//                        {sf_user_by_title,              sf_api},
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
                    QString token, user_id, expiration;

                    if (connectApi(token, user_id, expiration, error)) {
                        setApiToken(token);
                        setApiUserID(user_id);
                        setApiExpiration(expiration);
                        return true;
                    }

                    return false;
                }
//                bool connectUserSite() {
//                    setSiteUserID(apiUserID()); // use api user id
//                    return true; /*return connectSite(error);*/
//                }

                void saveAdditionals(QJsonObject & obj) {
//                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_api_base));
                }
                void loadAdditionals(QJsonObject & obj) {
//                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
//                    clearFriends();
//                    clearGroups();
                    Manager::removeCookies(url_api_base);
                }

//                void fromJson(const QJsonObject & hash);
//                void toJson(QJsonObject & hash);

//                bool connectUser() { return true; }

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
//                            {tkn_video_embedable, LSTR("true")}, // any // true
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
            };
        }
    }
}

#endif // YOUTUBE_REQUEST_API
