#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "../iapi.h"
#include "misc/settings.h"
#include "misc/web_utils/html_parser.h"
#include "od_api_keys.h"

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
            QUrl url(base_auth_url % "https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&st.fJS=on&st.st.screenSize=1920x1080&st.st.browserSize=621&st.st.flashVer=18.0.0&st.email=" + encodeStr(Settings::instance() -> od_key()) + "&st.password=" + encodeStr(Settings::instance() -> od_val()) + "&st.remember=on&st.iscode=false");
            qDebug() << url;
            return url;
        }

        inline QUrl authSidUrl() { return QUrl(baseUrlStr(QStringLiteral("web-api/music/conf"))); }

        inline QUrl audioUrl(const QString func) { return QUrl(base_audio_url % func % QStringLiteral(";") % genDefaultParams().toString()); }



        inline QUrl addPlaylistUrl() { return audioUrl(QStringLiteral("playlistsAdd")); } // params : (name: '') (publicPlaylist: 'true')
        inline QUrl removePlaylistUrl() { return audioUrl(QStringLiteral("playlistsRemove")); } // params : (pid: '' (id of playlist))
        inline QUrl getPlaylistsUrl() { return audioUrl(QStringLiteral("playlistsGet")); } // params: maybe has uid param ? need to check
        inline QUrl addToPlaylistsUrl() { return audioUrl(QStringLiteral("like")); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
        inline QUrl removeFromPlaylistsUrl() { return audioUrl(QStringLiteral("dislike")); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')


        inline QUrl searchAudioUrl() { return audioUrl(QStringLiteral("relevant")); } // params : (q: predicate) and pagination attrs
        inline QUrl tracksSearchUrl() { return audioUrl(QStringLiteral("tracks")); } // params : (q: predicate) and pagination attrs

        inline QUrl myAudioUrl() { return audioUrl(QStringLiteral("my")); } // params: (uid: sets for friend request) and pagination attrs
        inline QUrl collectionsAudioUrl() { return audioUrl(QStringLiteral("collections")); } // params: (collectionId: not used for index of collections) and pagination attrs
        inline QUrl downloadedAudioUrl() { return audioUrl(QStringLiteral("downloaded")); } // params : pagination attrs
        inline QUrl listenedAudioUrl() { return audioUrl(QStringLiteral("history")); } // params : pagination attrs
        inline QUrl popularAudioUrl() { return audioUrl(QStringLiteral("popTracks")); }
//        inline QUrl customAudioUrl() { return audioUrl(QStringLiteral("custom")); } // param (ids: ids of (track / album / artist) splited by coma (need to check)) ? // maybe this is info request per ids for items

        inline QUrl playAudioUrl() { return audioUrl(QStringLiteral("play")); } // params: (tid: track id) (pid: ? ('null')) (type: genre_id or genre name maybe ('8')) (position: position in list (0))
        inline QUrl play30AudioUrl() { return audioUrl(QStringLiteral("play30")); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: genre_id or genre name maybe ('8')) (position: position in list (0))

        inline QUrl tunersUrl() { return audioUrl(QStringLiteral("myTuners")); } // params: (locale: 'ru')  need to check pagination
        inline QUrl radioUrl(QString tuner) { return audioUrl(QStringLiteral("myRadio")); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

        inline QString grabUserId(QNetworkReply * reply) { // this did not used anywhere at this time
            Html::Document doc(reply);
            Html::Set results = doc.find("a.u-menu_a.tdn[href^'/profile']");
            if (results.isEmpty()) {
                Html::Set results = doc.find(".ff_links_li a[href~'st.uid=']]");
                return results.link().section("st.uid=", 1).section('&', 0, 0);
            } else
                return results.link().section('/', 2);
            return QString();
        }

        inline QString grabSID() {
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
