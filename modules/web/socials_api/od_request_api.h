#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include <qcryptographichash.h>

#include "../iapi.h"
#include "../web_api.h"
#include "misc/web_utils/html_parser.h"
#include "od_api_keys.h"

#define OD_OFFSET_LIMIT 100

namespace Od {
    class RequestApi : public WebApi, public IApi {
        int magic [33] = { 4, 3, 5, 6, 1, 2, 8, 7, 2, 9, 3, 5, 7, 1, 4, 8, 8, 3, 4, 3, 1, 7, 3, 5, 9, 8, 1, 4, 3, 7, 2, 8 };
    protected:
        QString hash_key;
        QString authE, authP;

        RequestApi(QObject * obj = 0) : WebApi(obj) {}

        bool sessionIsValid() {
            QJsonObject obj = WebManager::manager() -> getJson(initAudioUrl());
            return !obj.contains(QStringLiteral("error"));
        }

        inline void nullifyCredentials() {
            authE.clear();
            authP.clear();
        }

        inline void checkCredentials() {
            if (authE.isEmpty() || authP.isEmpty())
                showingLogin(authE, authP);
        }

        inline QUrl authRequestUrl() const {
            QUrl url(base_auth_url % "https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=" + encodeStr(authE) + "&st.password=" + encodeStr(authP) + "&st.remember=on&st.iscode=false");
            qDebug() << url;
            return url;
        }

        inline QUrl initUrl() const {
            QUrl url(base_url % "dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=" % hash_key);
            return url;
        }

        inline QHash<QString, QString> initHeaders() {
            QHash<QString, QString> headers;
//            headers.insert(QStringLiteral("User-Agent"), QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0"));
            headers.insert(QStringLiteral("User-Agent"), QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0"));
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

        inline QUrl myAudioUrl(const QString & uid) { return audioUrl(QStringLiteral("my"), QUrlQuery(QStringLiteral("uid=") % uid)); } // params: (uid: sets for friend request) and pagination attrs
        inline QUrl collectionsAudioUrl() { return audioUrl(QStringLiteral("collections")); } // params: (collectionId: not used for index of collections) and pagination attrs
        inline QUrl downloadedAudioUrl() { return audioUrl(QStringLiteral("downloaded")); } // params : pagination attrs
        inline QUrl isDownloadedAudioUrl() { return audioUrl(QStringLiteral("isDownloaded")); } // params : (tid: track id)
        inline QUrl listenedAudioUrl() { return audioUrl(QStringLiteral("history")); } // params : pagination attrs
        inline QUrl popularAudioUrl() { return audioUrl(QStringLiteral("popTracks")); }
        inline QUrl popAudioUrl() { return audioUrl(QStringLiteral("pop")); } // (locale: 'ru') and pagination attrs
        inline QUrl customAudioUrl() { return audioUrl(QStringLiteral("custom")); } // param (ids: ids of (track / album / artist) splited by coma) ? // info request per ids for items
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
        QString calcMagicNumber(const QString & md5) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData((md5 % QStringLiteral("secret")).toLatin1());
            QByteArray src = md5.toLatin1();//hash.result().toHex().toLower();

            QList<int> newSrc; newSrc.reserve(src.size() + 1);
            for(QByteArray::Iterator chr = src.begin(); chr != src.end(); chr++)
                newSrc << ((*chr) - ((*chr) < 58 ? 48 : 87));

            int sum = 0;
            for(QList<int>::Iterator num = newSrc.begin(); num != newSrc.end(); num++)
                sum += (*num);

            newSrc << newSrc.last();

            QString res;
            for (int x = 0; x < 32; x++)
                res.append(QString::number(qAbs(sum - newSrc[x + 1] * newSrc[x] * magic[x])));

            return res;
        }

        inline virtual ~RequestApi() {}

        QJsonObject userInfo(QString & uid) {
            if (uid.isEmpty())
                return WebManager::manager() -> getJson(initAudioUrl());
            else
                return WebManager::manager() -> getJson(myAudioUrl(uid));
        }

        inline QString refresh(QString refresh_page) { // here refresh_page must by eq to track id
            QJsonObject obj = WebManager::manager() -> getJson(playAudioUrl(refresh_page));
            QUrl url(obj.value(QStringLiteral("play")).toString());
            QUrlQuery query = QUrlQuery(url.query());
            query.addQueryItem(QStringLiteral("clientHash"), calcMagicNumber(query.queryItemValue(QStringLiteral("md5"))));
            url.setQuery(query);

            qDebug() << "OD" << url;

            return url.toString();
        }

        inline QString grabUserId(Html::Document & doc) { // this did not used anywhere at this time
            Html::Set results = doc.find("a.u-menu_a.tdn[href^'/profile']");
            if (results.isEmpty()) {
                Html::Set results = doc.find(".ff_links_li a[href~'st.uid=']]");
                return results.link().section("st.uid=", 1).section('&', 0, 0);
            }
            else return results.link().section('/', 2).section('?', 0, 0);

            return QString();
        }

        inline QString grabSID() {
            QJsonObject obj = WebManager::manager() -> postJson(authSidUrl(), initHeaders()); // calculate sid for requests
            if (obj.contains(QStringLiteral("sid")))
                return obj.value(QStringLiteral("sid")).toString();
            else {
                qDebug() << "SID ERROR" << obj;
                return QString();
            }
        }

        inline void checkSecurity(Html::Document & doc) {
            Html::Set forms = doc.find("[id^'hook_Form'] form");
//            doc.output();

            if (!forms.isEmpty()) {
                QList<FormInput> inputs;
                inputs << FormInput(QStringLiteral("code"), QStringLiteral("Code from sms"));
                inputs << FormInput(QStringLiteral("Resend sms"), forms.find("#accRcvrSent").link(), WebManager::manager(), "sendGet");
                actionDialog -> buildForm(inputs);

                if (actionDialog -> exec() == QDialog::Accepted) {
                    QHash<QString, QString> attrs;
                    attrs.insert("st.mobileCaptcha", actionDialog -> getValue(QStringLiteral("code")));
                    QUrl url = QUrl(base_url).resolved(forms.first() -> toFormSubmit(attrs));
                    QNetworkReply * reply = WebManager::manager() -> followedForm(url, initHeaders());
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
