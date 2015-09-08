#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "../iapi.h"
#include "../web_api.h"
#include "misc/settings.h"
#include "misc/web_utils/html_parser.h"
#include "od_api_keys.h"

#define OD_OFFSET_LIMIT 100

namespace Od {
    class RequestApi : public WebApi, public IApi {
    protected:
        QString hash_key;
        QString authE, uathP; //TODO: remove settings and use this vars

        inline QUrl authRequestUrl() const {
            QUrl url(base_auth_url % "https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=" + encodeStr(Settings::instance() -> od_key()) + "&st.password=" + encodeStr(Settings::instance() -> od_val()) + "&st.remember=on&st.iscode=false");
            qDebug() << url;
            return url;
        }

        inline QUrl initUrl() const {
            QUrl url(base_url % "dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=" % hash_key);
            return url;
        }

        inline QHash<QString, QString> initHeaders() {
            QHash<QString, QString> headers;
            headers.insert(QStringLiteral("User-Agent"), QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0"));
            return headers;
        }

        inline QUrl authSidUrl() { return QUrl(baseUrlStr(QStringLiteral("web-api/music/conf"))); }

        inline QUrl audioUrl(const QString func, const QUrlQuery & query = QUrlQuery()) {
            QUrl url(base_audio_url % func % QStringLiteral(";") % genDefaultParams().toString());
            url.setQuery(query);
            return url;
        }

        inline QUrl initAudioUrl() { return audioUrl(QStringLiteral("init")); }

        inline QUrl addPlaylistUrl() { return audioUrl(QStringLiteral("playlistsAdd")); } // params : (name: '') (publicPlaylist: 'true')
        inline QUrl removePlaylistUrl() { return audioUrl(QStringLiteral("playlistsRemove")); } // params : (pid: '' (id of playlist))
        inline QUrl getPlaylistsUrl() { return audioUrl(QStringLiteral("playlistsGet")); } // params: maybe has uid param ? need to check
        inline QUrl addToPlaylistsUrl() { return audioUrl(QStringLiteral("like")); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
        inline QUrl removeFromPlaylistsUrl() { return audioUrl(QStringLiteral("dislike")); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

        inline QUrl searchUrl() { return audioUrl(QStringLiteral("relevant")); } // params : (q: predicate) and pagination attrs
        inline QUrl searchTracksUrl() { return audioUrl(QStringLiteral("tracks")); } // params : (q: predicate) and pagination attrs
        inline QUrl searchAlbumsUrl() { return audioUrl(QStringLiteral("albums")); } // params : (q: predicate) and pagination attrs

        inline QUrl albumAudiosUrl() { return audioUrl(QStringLiteral("album")); } // params : (albumId: album id)
        inline QUrl artistAudiosUrl() { return audioUrl(QStringLiteral("artist")); } // params : (artistId: artist id)

        inline QUrl myAudioUrl() { return audioUrl(QStringLiteral("my")); } // params: (uid: sets for friend request) and pagination attrs
        inline QUrl collectionsAudioUrl() { return audioUrl(QStringLiteral("collections")); } // params: (collectionId: not used for index of collections) and pagination attrs
        inline QUrl downloadedAudioUrl() { return audioUrl(QStringLiteral("downloaded")); } // params : pagination attrs
        inline QUrl isDownloadedAudioUrl() { return audioUrl(QStringLiteral("isDownloaded")); } // params : (tid: track id)
        inline QUrl listenedAudioUrl() { return audioUrl(QStringLiteral("history")); } // params : pagination attrs
        inline QUrl popularAudioUrl() { return audioUrl(QStringLiteral("popTracks")); }
        inline QUrl popAudioUrl() { return audioUrl(QStringLiteral("pop")); } // (locale: 'ru') and pagination attrs
//        inline QUrl customAudioUrl() { return audioUrl(QStringLiteral("custom")); } // param (ids: ids of (track / album / artist) splited by coma (need to check)) ? // maybe this is info request per ids for items
//        inline QUrl formaddplUrl() { return audioUrl(QStringLiteral("formaddpl")); } // params: (tid: track id) // used for adding new item to playlist

        // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
        inline QUrl playAudioUrl(QString tid, int item_type = 5) { return audioUrl(QStringLiteral("play"), QUrlQuery(QStringLiteral("type=%1&tid=%2").arg(QString::number(item_type), tid))); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
        inline QUrl play30AudioUrl() { return audioUrl(QStringLiteral("play30")); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))

        inline QUrl tunersUrl() { return audioUrl(QStringLiteral("myTuners")); } // params: (locale: 'ru')  need to check pagination
        inline QUrl radioUrl(QString /*tuner*/) { return audioUrl(QStringLiteral("myRadio")); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

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

        inline QString refresh(QString refresh_page) { // here refresh_page must by eq to track id
            QJsonObject obj = WebManager::manager() -> getJson(playAudioUrl(refresh_page));
            qDebug() << "OD PLAY" << obj;
            return obj.value(QStringLiteral("play")).toString();
        }

        inline QString grabUserId(Html::Document & doc) { // this did not used anywhere at this time
            Html::Set results = doc.find("a.u-menu_a.tdn[href^'/profile']");
            if (results.isEmpty()) {
                Html::Set results = doc.find(".ff_links_li a[href~'st.uid=']]");
                return results.link().section("st.uid=", 1).section('&', 0, 0);
            } else
                return results.link().section('/', 2).section('?', 0);
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

        inline void checkSecurity(Html::Document & doc) {
            Html::Set forms = doc.find("[id^'hook_Form'] form");
            doc.output();

            if (!forms.isEmpty()) {
                QList<FormInput> inputs;
                inputs << FormInput(QStringLiteral("code"), QStringLiteral("Code from sms"));
                actionDialog -> buildForm(inputs);

                QString resendLink = forms.find("#accRcvrSent").link();
                QList<FormInput> extra_inputs;
                extra_inputs << FormInput(QStringLiteral("Resend sms"), resendLink, WebManager::manager(), "sendGet");
                actionDialog -> extendForm(extra_inputs);

                if (actionDialog -> exec() == QDialog::Accepted) {
                    QHash<QString, QString> attrs;
                    attrs.insert("st.mobileCaptcha", actionDialog -> getValue(QStringLiteral("code")));
                    QUrl url = QUrl(base_url).resolved(forms.first() -> toFormSubmit(attrs));
                    QNetworkReply * reply = WebManager::manager() -> postForm(url, true, initHeaders());
                    //TODO: check session
                    reply -> deleteLater();
                }
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
