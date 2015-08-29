#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "../iapi.h"
#include "misc/settings.h"
#include "misc/web_utils/html_parser.h"

#define OD_OFFSET_LIMIT 100

namespace Od {
    class RequestApi : public IApi {
    private:
//        inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, QStringLiteral("types"), QStringLiteral("original,remix,live,podcast")); }

//        // add to search
//        inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, QStringLiteral("types"), QStringLiteral("original,live")); }
//        inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, QStringLiteral("types"), QStringLiteral("remix")); }

//        inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, QStringLiteral("q"), predicate); }
//        inline void setIdsFilter(QUrlQuery & query, QStringList & uids) { setParam(query, QStringLiteral("ids"), uids.join(",")); }
//        inline void setGenreLimitation(QUrlQuery & query, QString & genre) { setParam(query, QStringLiteral("genres"), genre); }
//        inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, QStringLiteral("order"), hottest ? QStringLiteral("hotness") : QStringLiteral("created_at")); }
    public:
        inline virtual ~RequestApi() {}

        inline QUrl authRequestUrl() const {
            QUrl url("https://www.ok.ru/https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&st.fJS=on&st.st.screenSize=1920x1080&st.st.browserSize=621&st.st.flashVer=18.0.0&st.email=" + encodeStr(Settings::instance() -> od_key()) + "&st.password=" + encodeStr(Settings::instance() -> od_val()) + "&st.remember=on&st.iscode=false");
            qDebug() << url;
            return url;
        }

        inline QUrl authSidUrl() const { return QUrl(QStringLiteral("http://ok.ru/web-api/music/conf")); }

        inline QString grabUserId(QNetworkReply * reply) {
            Html::Document doc(reply);
            doc.output();
//            Html::Set results = doc.find("a.u-menu_a.tdn[href='/profile']");
//            return results.link().section('/', 2);
            return QString();
        }

        inline QString grabSID() const {
            QJsonObject obj = WebManager::manager() -> postJson(QNetworkRequest(authSidUrl()), QByteArray()); // registrate sid for requests
            if (obj.contains(QStringLiteral("sid")))
                return obj.value(QStringLiteral("sid")).toString();
            else {
                qDebug() << "SID ERROR" << obj;
                return QString();
            }
        }

//        /////////////////
//        /// AUTH
//        ////////////////

//        //QString authTokenUrl() const {
//        //    QUrl url("https://api.soundcloud.com/oauth2/token");
//        //    QUrlQuery query = genDefaultParams();


//        //    query.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
//        //    query.addQueryItem("scope", "non-expiring");

//        ////    query.addQueryItem("grant_type", "password");
//        ////    query.addQueryItem("username", "USERNAME");
//        ////    query.addQueryItem("password", "PASSWORD");

//        //    url.setQuery(query);
//        //    return url.toString();
//        //}
//        inline QUrl authTokenUrl() const { return QUrl(QStringLiteral("https://api.soundcloud.com/oauth2/token")); }
//        inline QString confirmAuthUrl(QString access_token) { return QStringLiteral("https://api.soundcloud.com/me.json?oauth_token=") % access_token; }

//        QByteArray authTokenUrlParams(QString code) {
//            QUrlQuery query = genDefaultParams();

//            setParam(query, QStringLiteral("client_secret"), QStringLiteral("54ca588303e1d2bf524509faf20931b4"));
//            setParam(query, QStringLiteral("grant_type"), QStringLiteral("authorization_code"));
//            setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
//            setParam(query, QStringLiteral("code"), code);

//            return query.toString(QUrl::FullyEncoded).toUtf8();
//        }
//    //    QUrlQuery userMethodParams(QString & token) {
//    //        QUrlQuery query = QUrlQuery();
//    //        query.addQueryItem("oauth_token", token);
//    //        return query;
//    //    }

//        /////////////////
//        /// API
//        ////////////////
//        QUrl audioSearchUrl(QString & predicate, QString & genre, bool hottest = false) {
//            QUrlQuery query = genDefaultParams();
//            setAudioTypesParam(query);
//            setOrder(query, hottest);

//            if (!genre.isEmpty())
//                setGenreLimitation(query, genre);

//            if (!predicate.isEmpty())
//                setSearchPredicate(query, predicate);

//            return baseUrl(QStringLiteral("tracks"), query);
//        }

//        QJsonArray search_postprocess(QString & predicate, QString & genre, const SearchLimit & limitations) { //count = 5
//            return lQuery(
//                audioSearchUrl(predicate, genre, limitations.by_popularity()),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(limitations.count, SOUNDCLOUD_OFFSET_LIMIT)),
//                wrap
//            );
//        }


//        QUrl groupAudioUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            setAudioTypesParam(query);
//            return baseUrl("groups/" % uid % "/tracks", query);
//        }
//        QJsonArray groupAudio(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//        //    group_id = "101";
//            return lQuery(
//                groupAudioUrl(group_id),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl groupPlaylistsUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("groups/" % uid % "/playlists", query);
//        }
//        QJsonArray groupPlaylists(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//        //    group_id = "101";
//            return lQuery(
//                groupPlaylistsUrl(group_id),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl audioInfoUrl(QString & audio_uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("tracks/" % audio_uid, query);
//        }
//        QJsonObject audioInfo(QString audio_uid) { return sQuery(audioInfoUrl(audio_uid)); }


//        QUrl audioUrl(QStringList & audio_uids) {
//            QUrlQuery query = genDefaultParams();
//            setIdsFilter(query, audio_uids);
//            return baseUrl("tracks", query);
//        }
//        //"id": 142370360,
//        //"permalink": "sam-smith-stay-with-me",
//        QJsonArray audioInfo(QStringList & audio_uids) { return sQuery(audioUrl(audio_uids), wrap).value(QStringLiteral("response")).toArray(); }


//        QUrl userAudioUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("users/" % uid % "/tracks", query);
//        }
//        QJsonArray userAudio(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//            return lQuery(
//                userAudioUrl(uid),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl userPlaylistsUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("users/" % uid % "/playlists", query);
//        }
//        QJsonArray userPlaylists(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//            return lQuery(
//                userPlaylistsUrl(uid),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl userFollowingsUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("users/" % uid % "/followings", query);
//        }
//        QJsonArray userFollowings(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//            return lQuery(
//                userFollowingsUrl(uid),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl userFollowersUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("users/" % uid % "/followers", query);
//        }
//        QJsonArray userFollowers(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//            return lQuery(
//                userFollowersUrl(uid),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }


//        QUrl userGroupsUrl(QString & uid) {
//            QUrlQuery query = genDefaultParams();
//            return baseUrl("users/" % uid % "/groups", query);
//        }
//        QJsonArray userGroups(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//            return lQuery(
//                userGroupsUrl(uid),
//                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT))
//            );
//        }

    //    bool SoundcloudApi::responseRoutine(QString fieldName, QNetworkReply * reply, ApiFuncContainer * func) {
    //        QJsonObject obj = Web::replyToJson(reply, true);

    //        reply -> deleteLater();

    //        bool hasError = obj.value("response").toObject().contains("errors");

    //        qDebug() << fieldName << hasError << obj;

    //        if (hasError)
    //            errorSend(obj, func -> obj);
    //        else {
    //            if (func -> result.contains(fieldName)) { // rewrite on array of json arrays
    //                QVariantList n_ar = obj.value("response").toArray().toVariantList();

    //                if (!n_ar.isEmpty()) {
    //                    QVariantList ar = func -> result.value(fieldName).toArray().toVariantList();
    //                    ar.append(n_ar);

    //                    func -> result.insert(fieldName, QJsonArray::fromVariantList(ar));
    //                }
    //            } else
    //                func -> result.insert(fieldName, obj.value("response").toArray());
    //        }

    //        return !hasError;
    //    }
    };
}

#endif // OD_REQUEST_API
