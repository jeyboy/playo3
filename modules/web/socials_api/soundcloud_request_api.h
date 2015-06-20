#ifndef SOUNDCLOUD_REQUEST_API
#define SOUNDCLOUD_REQUEST_API

#include "../iapi.h"

class SoundcloudRequestApi : public IApi {
private:
    inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, "types", "original,remix,live,podcast"); }
    inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, "q", predicate); }
    inline void setIdsFilter(QUrlQuery & query, QStringList & uids) { setParam(query, "ids", uids.join(",")); }
    inline void setGenreLimitation(QUrlQuery & query, QString & genre) { setParam(query, "genres", genre); }
    inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, "order", hottest ? "hotness" : "created_at"); }
public:
    /////////////////
    /// AUTH
    ////////////////
    inline QString authUrl() const {
        QUrl url("https://soundcloud.com/connect");

        QUrlQuery query = buildDefaultParams();
        setParam(query, "response_type", "code");
        setParam(query, "scope", "non-expiring");
        setParam(query, "redirect_uri", "http://sos.com");
        setParam(query, "display", "popup");

        url.setQuery(query);
        return url.toString();
    }

    //QString authTokenUrl() const {
    //    QUrl url("https://api.soundcloud.com/oauth2/token");
    //    QUrlQuery query = buildDefaultParams();


    //    query.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    //    query.addQueryItem("scope", "non-expiring");

    ////    query.addQueryItem("grant_type", "password");
    ////    query.addQueryItem("username", "USERNAME");
    ////    query.addQueryItem("password", "PASSWORD");

    //    url.setQuery(query);
    //    return url.toString();
    //}
    inline QUrl authTokenUrl() const { return QUrl("https://api.soundcloud.com/oauth2/token"); }
    inline QString confirmAuthUrl(QString access_token) {
        return "https://api.soundcloud.com/me.json?oauth_token=" + access_token;
    }

    QByteArray authTokenUrlParams(QString code) {
        QUrlQuery query = buildDefaultParams();

        setParam(query, "client_secret", "54ca588303e1d2bf524509faf20931b4");
        setParam(query, "grant_type", "authorization_code");
        setParam(query, "redirect_uri", "http://sos.com");
        setParam(query, "code", code);

        return query.toString(QUrl::FullyEncoded).toUtf8();
    }
//    QUrlQuery userMethodParams(QString & token) {
//        QUrlQuery query = QUrlQuery();
//        query.addQueryItem("oauth_token", token);
//        return query;
//    }

    /////////////////
    /// API
    ////////////////

    QUrl audiosSearchUrl(QString & predicate, QString & genre, bool hottest, int offset) {
        QUrlQuery query = buildDefaultParams();
        setAudioTypesParam(query);
        setLimit(query, 99999, offset);
        setOrder(query, hottest);

        if (!genre.isEmpty())
            setGenreLimitation(query, genre);

        if (!predicate.isEmpty())
            setSearchPredicate(query, predicate);

        QUrl url(getApiUrl() + "tracks.json");
        url.setQuery(query);
        return url;
    }


    QUrl SoundcloudApiPrivate::groupAudiosUrl(QString & uid, int offset) {
        QUrlQuery query = buildDefaultParams();
        setAudioTypesParam(query);
        setLimit(query, 99999, offset);

        QUrl url(getApiUrl() + "groups/" + uid + "/tracks.json");
        url.setQuery(query);
        return url;
    }

    QUrl SoundcloudApiPrivate::groupPlaylistsUrl(QString & uid, int offset) {
        QUrl url(getApiUrl() + "groups/" + uid + "/playlists.json");
        QUrlQuery query = buildDefaultParams();
        setLimit(query, 99999, offset);
        url.setQuery(query);

        return url;
    }

    QUrl SoundcloudApiPrivate::audioUrl(QString & audio_uid) {
        QUrlQuery query = buildDefaultParams();
        QUrl url(getApiUrl() + "/tracks/" + audio_uid + ".json");
        url.setQuery(query);
        return url;
    }

    QUrl SoundcloudApiPrivate::audiosUrl(QStringList & audio_uids) {
        QUrlQuery query = buildDefaultParams();
        setIdsFilter(query, audio_uids);

        QUrl url(getApiUrl() + "/tracks.json");
        url.setQuery(query);
        return url;
    }

    QUrl SoundcloudApiPrivate::userAudiosUrl(QString & uid, int offset) {
        QUrlQuery query = buildDefaultParams();
        setAudioTypesParam(query);
        setLimit(query, 99999, offset);

        QUrl url(getApiUrl() + "users/" + uid + "/tracks.json");
        url.setQuery(query);
        return url;
    }

    QUrl SoundcloudApiPrivate::userPlaylistsUrl(QString & uid, int offset) {
        QUrl url(getApiUrl() + "users/" + uid + "/playlists.json");
        QUrlQuery query = buildDefaultParams();
        setLimit(query, 99999, offset);
        url.setQuery(query);

        return url;
    }

    QUrl SoundcloudApiPrivate::userFolowingsUrl(QString & uid, int offset) {
        QUrl url(getApiUrl() + "users/" + uid + "/followings.json");
        QUrlQuery query = buildDefaultParams();
        setLimit(query, 99999, offset);
        url.setQuery(query);

        return url;
    }

    QUrl SoundcloudApiPrivate::userFolowersUrl(QString & uid, int offset) {
        QUrl url(getApiUrl() + "users/" + uid + "/followers.json");
        QUrlQuery query = buildDefaultParams();
        setLimit(query, 99999, offset);
        url.setQuery(query);

        return url;
    }

    QUrl SoundcloudApiPrivate::userGroupsUrl(QString & uid, int offset) {
        QUrl url(getApiUrl() + "users/" + uid + "/groups.json");
        QUrlQuery query = buildDefaultParams();
        setLimit(query, 99999, offset);
        url.setQuery(query);

        return url;
    }



























//        inline QUrl songSearchUrl(int mode, QString & artist, QString & title, QStringList & tags,
//                QStringList & styles, QStringList & moods, int offset = 0) {
//            QUrl url(baseUrl("song/search"));
//            QUrlQuery query = buildDefaultParams();
//            setLimit(query, requestLimit(), offset);

//            if (!artist.isEmpty()) setParam(query, "artist", artist);
//            if (!title.isEmpty()) setParam(query, "title", title);

//            if (mode == 0 || mode == 1)
//                setParam(query, "mode", QString::number(mode));


//            setParam(query, "style", styles);
//            setParam(query, "description", tags);
//            setParam(query, "mood", moods);

//            url.setQuery(query);
//            return url;
//        }

//        QJsonArray songSearch(int mode = -1, QString artist = QString(), QString title = QString(), QStringList tags = QStringList(),
//                              QStringList styles = QStringList(), QStringList moods = QStringList()) {
//            QJsonObject response;
//            QJsonArray songs;
//            int offset = 0;

//            while (proceedQuery(songSearchUrl(mode, artist, title, tags, styles, moods, offset), response)) {
//                songs.append(response.value("response").toObject().value("songs"));

//                offset += requestLimit();

//                if (offset >= extractAmount(response))
//                    break;
//            }

//            return songs;
//        }













    //"id": 142370360,
    //"permalink": "sam-smith-stay-with-me",

    QJsonObject SoundcloudApi::getAudiosInfo(QStringList audio_uids) {
        return CustomNetworkAccessManager::manager() -> getToJson(QNetworkRequest(SoundcloudApiPrivate::audiosUrl(audio_uids)), true);
    }

    QJsonObject SoundcloudApi::getAudioInfo(QString audio_uid) {
        return CustomNetworkAccessManager::manager() -> getToJson(QNetworkRequest(SoundcloudApiPrivate::audioUrl(audio_uid)), true);
    //    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::audioUrl(audio_uid)));

    //    QJsonObject obj = responseToJson(m_http -> readAll());

    //    m_http -> close();
    //    delete m_http;

    //    return obj;
    }

    void SoundcloudApi::getGroupInfo(const QObject * receiver, const char * respSlot, QString uid) {
        startApiCall(QtConcurrent::run(this, &SoundcloudApi::getGroupInfoRoutine, new ApiFuncContainer(receiver, respSlot, uid)));
    }

    ApiFuncContainer * SoundcloudApi::getGroupInfoRoutine(ApiFuncContainer * func) {
    //    uid = "101";
        QNetworkReply * m_http = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupAudiosUrl(func -> uid)));

        if (responseRoutine("audio_list", m_http, func)) {
            m_http = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupPlaylistsUrl(func -> uid)));
            responseRoutine("playlists", m_http, func);
        }

        return func;
    }

    void SoundcloudApi::getUidInfo(const QObject * receiver, const char * respSlot, QString uid = "0") {
        startApiCall(QtConcurrent::run(this, &SoundcloudApi::getUidInfoRoutine, new ApiFuncContainer(receiver, respSlot, uid)));
    }

    ApiFuncContainer * SoundcloudApi::getUidInfoRoutine(ApiFuncContainer * func) {
        if (func -> uid[0] == '-') {
            func -> uid = func -> uid.mid(1);
            return getGroupInfoRoutine(func);
        }

        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

        func -> uid = /*"183";*/ func -> uid == "0" ? getUserID() : func -> uid;

        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userAudiosUrl(func -> uid)));

        if (responseRoutine("audio_list", m_http, func)) {
            m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userPlaylistsUrl(func -> uid)));

            if (responseRoutine("playlists", m_http, func)) {
                if (func -> uid == getUserID()) {
                    QThread::msleep(REQUEST_DELAY);
                    //////////////////////////////////////////////////////////////
                    m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userFolowingsUrl(func -> uid)));

                    if (responseRoutine("followings", m_http, func)) {
                        m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userFolowersUrl(func -> uid)));

                        if (responseRoutine("followers", m_http, func)) {
                            QThread::msleep(REQUEST_DELAY);

                            m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userGroupsUrl(func -> uid)));
                            responseRoutine("groups", m_http, func);
                        }
                    }
                }
            }
        }

        return func;
    }

    void SoundcloudApi::searchAudio(const QObject * receiver, const char * respSlot, QString predicate, QString genre, bool popular) {
        startApiCall(QtConcurrent::run(this, &SoundcloudApi::searchAudioRoutine, new ApiFuncContainer(receiver, respSlot, QString()), predicate, genre, popular));
    }

    QJsonObject SoundcloudApi::searchAudioSync(const QObject * receiver, QString predicate, QString genre, bool popular) {
        ApiFuncContainer * func = searchAudioRoutine(new ApiFuncContainer(receiver, 0, QString()), predicate, genre, popular);
        QJsonObject res = func -> result;
        delete func;
        return res;
    }

    ApiFuncContainer * SoundcloudApi::searchAudioRoutine(ApiFuncContainer * func, QString & predicate, QString & genre, bool popular) {
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
        QNetworkReply * m_http;

        for(int i = 0; i < 5; i++) {
            m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::audiosSearchUrl(predicate, genre, popular, i * SoundcloudApiPrivate::amountLimit())));
            responseRoutine("audio_list", m_http, func);
        }

        return func;
    }

    ///////////////////////////////////////////////////////////
    /// PROTECTED
    ///////////////////////////////////////////////////////////

    bool SoundcloudApi::responseRoutine(QString fieldName, QNetworkReply * reply, ApiFuncContainer * func) {
        QJsonObject obj = CustomNetworkAccessManager::replyToJson(reply, true);

        reply -> deleteLater();

        bool hasError = obj.value("response").toObject().contains("errors");

        qDebug() << fieldName << hasError << obj;

        if (hasError)
            errorSend(obj, func -> obj);
        else {
            if (func -> result.contains(fieldName)) { // rewrite on array of json arrays
                QVariantList n_ar = obj.value("response").toArray().toVariantList();

                if (!n_ar.isEmpty()) {
                    QVariantList ar = func -> result.value(fieldName).toArray().toVariantList();
                    ar.append(n_ar);

                    func -> result.insert(fieldName, QJsonArray::fromVariantList(ar));
                }
            } else
                func -> result.insert(fieldName, obj.value("response").toArray());
        }

        return !hasError;
    }

    void SoundcloudApi::errorSend(QJsonObject & doc, const QObject * obj) {
        QJsonObject error = doc.value("response").toObject().value("errors").toArray().first().toObject();
        int err_code = -1;//error.value("error_code").toInt();
        QString err_msg = error.value("error_message").toString();

        connect(this, SIGNAL(errorReceived(int,QString)), obj, SLOT(errorReceived(int,QString)));
        emit errorReceived(err_code, err_msg);
        disconnect(this, SIGNAL(errorReceived(int,QString)), obj, SLOT(errorReceived(int,QString)));
    }





    static QUrl audiosSearchUrl(QString & predicate, QString & genre, bool hottest = false, int offset = 0);

    static QUrl groupAudiosUrl(QString & uid, int offset = 0);
    static QUrl groupPlaylistsUrl(QString & uid, int offset = 0);

    static QUrl audioUrl(QString & audio_uid);
    static QUrl audiosUrl(QStringList & audio_uids);

    static QUrl userAudiosUrl(QString & uid, int offset = 0);
    static QUrl userPlaylistsUrl(QString & uid, int offset = 0);
    static QUrl userFolowingsUrl(QString & uid, int offset = 0);
    static QUrl userFolowersUrl(QString & uid, int offset = 0);
    static QUrl userGroupsUrl(QString & uid, int offset = 0);
};

#endif // SOUNDCLOUD_REQUEST_API
