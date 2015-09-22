#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "od_api_misc.h"

#define OD_OFFSET_LIMIT 2000
#define OD_SEARCH_LIMIT 200

namespace Od {
    class RequestApi : public Misc {
    protected:
        QString hash_key;

        RequestApi(QObject * obj = 0) : Misc(obj) {}

        bool sessionIsValid() { return !hasError(WebManager::manager() -> getJson(initAudioUrl())); }

        inline QUrl authRequestUrl() const {
            QUrl url(base_auth_url % "https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=" + encodeStr(authE) + "&st.password=" + encodeStr(authP) + "&st.remember=on&st.iscode=false");
            qDebug() << url;
            return url;
        }

        inline QUrl initUrl() const {
            QUrl url(base_url % "dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=" % hash_key);
            return url;
        }

        inline QUrl audioUrl(const QString func, const QUrlQuery & query = QUrlQuery()) {
            QUrl url(base_audio_url % func % QStringLiteral(";") % genDefaultParams().toString());
            url.setQuery(query);
            return url;
        }

        inline QUrl addPlaylistUrl() { return audioUrl(QStringLiteral("playlistsAdd")); } // params : (name: '') (publicPlaylist: 'true')
        inline QUrl removePlaylistUrl() { return audioUrl(QStringLiteral("playlistsRemove")); } // params : (pid: '' (id of playlist))
        inline QUrl getPlaylistsUrl() { return audioUrl(QStringLiteral("playlistsGet")); } // params: maybe has uid param ? need to check
        inline QUrl addToPlaylistsUrl() { return audioUrl(QStringLiteral("like")); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
        inline QUrl removeFromPlaylistsUrl() { return audioUrl(QStringLiteral("dislike")); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

        inline QUrl searchUrl(const QString & predicate) { return audioUrl(QStringLiteral("relevant"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
        inline QUrl searchTracksUrl(const QString & predicate) { return audioUrl(QStringLiteral("tracks"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
        inline QUrl searchAlbumsUrl(const QString & predicate) { return audioUrl(QStringLiteral("albums"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
        inline QUrl searchArtistsUrl(const QString & predicate) { return audioUrl(QStringLiteral("artists"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs

        inline QUrl albumAudiosUrl() { return audioUrl(QStringLiteral("album")); } // params : (albumId: album id)
        inline QUrl artistAudiosUrl() { return audioUrl(QStringLiteral("artist")); } // params : (artistId: artist id)


        inline QUrl collectionsAudioUrl() { return audioUrl(QStringLiteral("collections")); } // params: (collectionId: not used for index of collections) and pagination attrs
        inline QUrl downloadedAudioUrl() { return audioUrl(QStringLiteral("downloaded")); } // params : pagination attrs
        inline QUrl isDownloadedAudioUrl() { return audioUrl(QStringLiteral("isDownloaded")); } // params : (tid: track id)
        inline QUrl listenedAudioUrl() { return audioUrl(QStringLiteral("history")); } // params : pagination attrs
        inline QUrl popularAudioUrl() { return audioUrl(QStringLiteral("popTracks")); }

        // artists // tracks // tuners // collections // albums
        inline QUrl popAudioUrl() { return audioUrl(QStringLiteral("pop")); } // (locale: 'ru') and pagination attrs
        inline QUrl customAudioUrl() { return audioUrl(QStringLiteral("custom")); } // param (ids: ids of (track / album / artist) splited by coma) ? // info request per ids for items
//        inline QUrl formaddplUrl() { return audioUrl(QStringLiteral("formaddpl")); } // params: (tid: track id) // used for adding new item to playlist

        // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
        inline QUrl playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrl(QStringLiteral("play"), QUrlQuery(QStringLiteral("tid=%2").arg(tid))); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
        inline QUrl play30AudioUrl() { return audioUrl(QStringLiteral("play30")); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))

        inline QUrl tunersUrl() { return audioUrl(QStringLiteral("myTuners")); } // params: (locale: 'ru')  need to check pagination
        inline QUrl radioUrl(QString /*tuner*/) { return audioUrl(QStringLiteral("myRadio")); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

        QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
            if (predicate.isEmpty() || limitations.by_popularity())
                return lQuery(popAudioUrl(), QueryRules(QStringLiteral("tracks"), requestLimit()));
            else {
                if (limitations.by_artists())
                    return lQuery(searchArtistsUrl(predicate), QueryRules(QStringLiteral("artists"), requestLimit(), OD_SEARCH_LIMIT));
                else if (limitations.by_songs())
                    return lQuery(searchTracksUrl(predicate), QueryRules(QStringLiteral("tracks"), requestLimit(), OD_SEARCH_LIMIT));
                else
                    return lQuery(searchUrl(predicate), QueryRules(QStringLiteral("tracks"), requestLimit(), OD_SEARCH_LIMIT));
            }
        }
    public:
        inline virtual ~RequestApi() {}

        inline QUrl initAudioUrl() { return audioUrl(QStringLiteral("init")); }
        inline QUrl myAudioUrl(const QString & uid) { return audioUrl(QStringLiteral("my"), QUrlQuery(QStringLiteral("uid=") % uid)); } // params: (uid: sets for friend request) and pagination attrs
        QJsonObject userInfo(const QString & uid) {
            if (uid.isEmpty()) {
                qDebug() << initAudioUrl();
                return WebManager::manager() -> getJson(initAudioUrl());
            } else
                return WebManager::manager() -> getJson(myAudioUrl(uid));
        }

        inline QUrl playlistAudioUrl(const QString & pid) { return audioUrl(QStringLiteral("my"), QUrlQuery(QStringLiteral("pid=") % pid)); } // params: (pid: playlist id) and pagination attrs
        QJsonArray playlistInfo(QString & pid, int count) {
            return lQuery(
                playlistAudioUrl(pid),
                QueryRules(QStringLiteral("tracks"), requestLimit(), qMin(count, OD_OFFSET_LIMIT))
            );
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
