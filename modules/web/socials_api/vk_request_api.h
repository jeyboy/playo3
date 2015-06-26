#ifndef VK_REQUEST_API
#define VK_REQUEST_API

#include "../iapi.h"

class VkRequestApi : public IApi {
protected:
    inline static QString boolToStr(bool val) { return val ? "1" : "0"; }
    inline static QString getObjLimit() { return "20"; }

    static QString authUrl();
    static QUrl wallUrl(QString uid, QString token, int offset, int count);
    static QUrl audioRefreshUrl(QStringList uids, QString token);
    static QUrl audioAlbumsUrl(QString uid, QString token, int offset);
    static QUrl audioInfoUrl(QString uid, QString currUid, QString token);
    static QUrl audioRecomendationUrl(QString uid, bool byUser, bool randomize, QString token);
    static QUrl audioPopularUrl(bool onlyEng, QString token, int genreId = -1);
    static QUrl audioSearchUrl(QString searchStr, bool autoFix, bool artistOnly, bool searchByOwn, int sort, QString token);
    static QUrl audioSearchLimitedUrl(QString searchStr, int limit, QString token);
    static QUrl audioLyricsUrl(QString token, QString lyrics_id);

    static QUrl isAppUser(QString token, QString uid);







    ///////////////////////////////////////////////////////////
    /// WALL
    ///////////////////////////////////////////////////////////

    ApiFunc * VkApi::wallMediaRoutine(ApiFunc * func, int offset, int count) {
        QJsonObject doc;
        QVariantList res;

        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
        QNetworkReply * m_http;

        while(true) {
            m_http = netManager -> getSync(QNetworkRequest(
                VkApiPrivate::wallUrl(func -> uid, token(), offset, count)
            ));

            if (!responseRoutine(m_http, func, doc))
                break;

            doc = doc.value("response").toObject();

            res.append(doc.value("posts").toArray().toVariantList());

            offset = doc.value("offset").toInt();
            count = doc.value("count").toInt();
            if (offset >= count)
                break;
        }

        func -> result.insert("posts", QJsonArray::fromVariantList(res));
        return func;
    }

    void VkApi::wallMediaList(const QObject * receiver, const char * respSlot, QString uid, int offset, int count) {
        startApiCall(QtConcurrent::run(this, &VkApi::wallMediaRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), offset, count));
    }

    ///////////////////////////////////////////////////////////
    /// FOLDERS LIST
    ///////////////////////////////////////////////////////////

    ApiFunc * VkApi::audioAlbumsRoutine(ApiFunc * func, int offset) {
        QJsonObject doc;
        QVariantList res, temp;
        res.append(func -> result.value("albums").toArray().toVariantList());

        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
        QNetworkReply * m_http;

        while(true) {
            m_http = netManager -> getSync(QNetworkRequest(
                VkApiPrivate::audioAlbumsUrl(func -> uid, token(), offset)
            ));

            if (!responseRoutine(m_http, func, doc))
                break;

            doc = doc.value("response").toObject();

            temp = doc.value("albums").toArray().toVariantList();
            if (temp.isEmpty())
                break;

            temp.append(res);
            res = temp;
    //        res.append(doc.value("albums").toArray().toVariantList());
            offset = doc.value("offset").toInt();
            if (doc.value("finished").toBool())
                break;
        }

        func -> result.insert("albums", QJsonArray::fromVariantList(res));

        return func;
    }

    void VkApi::audioAlbums(const QObject * receiver, const char * respSlot, QString uid) {
        startApiCall(QtConcurrent::run(this, &VkApi::audioAlbumsRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), 0));
    }

    ///////////////////////////////////////////////////////////
    /// AUDIO LIST
    ///////////////////////////////////////////////////////////

    ApiFunc * VkApi::audioListRoutine(ApiFunc * func) {
        QNetworkReply * m_http;
        QUrl url = VkApiPrivate::audioInfoUrl(func -> uid, userID(), token());
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

        m_http = netManager -> getSync(QNetworkRequest(url));
        if (responseRoutine(m_http, func, func -> result)) {
            func -> result = func -> result.value("response").toObject();

            if (!func -> result.value("albums_finished").toBool()) {
                int offset = func -> result.value("albums_offset").toInt();
                audioAlbumsRoutine(func, offset);
            }
        }

        return func;
    }

    void VkApi::audioList(const QObject * receiver, const char * respSlot, QString uid) {
        startApiCall(QtConcurrent::run(this, &VkApi::audioListRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid))));
    }

    ApiFunc * VkApi::audioRecomendationRoutine(ApiFunc * func, bool byUser, bool randomize) {
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

        QUrl url = VkApiPrivate::audioRecomendationUrl(
            func -> uid,
            byUser,
            randomize,
            token()
        );

        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
        if (responseRoutine(m_http, func, func -> result)) {
            func -> result = func -> result.value("response").toObject();
        }

        return func;
    }
    void VkApi::audioRecomendation(const QObject * receiver, const char * respSlot, QString uid, bool byUser, bool randomize) {
        startApiCall(QtConcurrent::run(this, &VkApi::audioRecomendationRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), byUser, randomize));
    }

    ApiFunc * VkApi::searchAudioRoutine(ApiFunc * func, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

        QUrl url = VkApiPrivate::audioSearchUrl(
            predicate,
            false,
            onlyArtist,
            inOwn,
            mostPopular ? 2 : 0,
            token()
        );

        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
        if (responseRoutine(m_http, func, func -> result))
            func -> result = func -> result.value("response").toObject();

        return func;
    }
    void VkApi::audioSearch(const QObject * receiver, const char * respSlot, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
        startApiCall(QtConcurrent::run(this, &VkApi::searchAudioRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), predicate, onlyArtist, inOwn, mostPopular));
    }

    QJsonObject VkApi::audioSearchSync(const QObject * receiver, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
        ApiFunc * func = searchAudioRoutine(new ApiFunc(receiver, 0, adapteUid(uid)), predicate, onlyArtist, inOwn, mostPopular);
        QJsonObject res = func -> result;
        delete func;
        return res;
    }

    QJsonObject VkApi::audioSearchSync(const QObject * receiver, QString predicate, int limitation) {
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
        ApiFunc * func = new ApiFunc(receiver, 0, "");

        QUrl url = VkApiPrivate::audioSearchLimitedUrl(
            predicate,
            limitation,
            token()
        );

        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
        QJsonObject res;
        if (responseRoutine(m_http, func, res))
            res = res.value("response").toObject();

        delete func;
        return res;
    }

    ApiFunc * VkApi::audioPopularRoutine(ApiFunc * func, bool onlyEng, int genreId) {
        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

        QUrl url = VkApiPrivate::audioPopularUrl(
            onlyEng,
            token(),
            genreId
        );

        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
        if (responseRoutine(m_http, func, func -> result))
            func -> result = func -> result.value("response").toObject();

        return func;
    }
    void VkApi::audioPopular(const QObject * receiver, const char * respSlot, bool onlyEng, int genreId) {
        startApiCall(QtConcurrent::run(this, &VkApi::audioPopularRoutine, new ApiFunc(receiver, respSlot, 0), onlyEng, genreId));
    }

    QJsonObject VkApi::audioPopularSync(const QObject * receiver, bool onlyEng, int genreId) {
        ApiFunc * func = audioPopularRoutine(new ApiFunc(receiver, 0, 0), onlyEng, genreId);
        QJsonObject res = func -> result;
        delete func;
        return res;
    }

    QJsonObject VkApi::getAudiosInfo(QStringList audio_uids) {
        QUrl url = VkApiPrivate::audioRefreshUrl(audio_uids, token());

        CustomNetworkAccessManager * netManager;
        bool new_manager = CustomNetworkAccessManager::validManager(netManager);

    //    QNetworkReply * reply = netManager -> getSync(QNetworkRequest(url));

    //    QJsonObject doc = responseToJson(reply -> readAll());

    //    reply -> close();
    //    delete reply;

        QJsonObject doc = netManager -> getToJson(QNetworkRequest(url));

        if (new_manager)
            delete netManager;

        return doc;
    }

    QJsonObject VkApi::getAudioInfo(QString audio_uid) {
        QStringList uids; uids << audio_uid;
        QJsonObject doc = getAudiosInfo(uids);
        return doc.value("response").toArray().first().toObject();
    }

    QString VkApi::refreshAudioItemUrl(QString audio_uid) {
        return getAudioInfo(audio_uid).value("url").toString();
    }

    ////TODO: has some troubles with ids amount in request
    //void VkApi::refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids) { // TODO: rewrite required
    //    QUrl url = VkApiPrivate::audioRefreshUrl(uids, getToken());
    //    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    ////    responses.insert(m_http, responseSlot);
    ////    collations.insert(m_http, uids);
    //    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListResponse()));
    //}



    bool VkApi::responseRoutine(QNetworkReply * reply, ApiFunc * func, QJsonObject & doc) {
        doc = CustomNetworkAccessManager::replyToJson(reply);

        QUrl url = reply -> url();
        reply -> deleteLater();

        if (doc.contains("error")) {
            doc = doc.value("error").toObject();
            return errorSend(doc, func, url);
        }

        return true;
    }

    bool VkApi::errorSend(QJsonObject & error, ApiFunc * func, QUrl url) {
        int err_code = error.value("error_code").toInt();
        QString err_msg = error.value("error_msg").toString();

        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ERROR " << error;

        if (err_code != 14) {
            connect(this, SIGNAL(errorReceived(int,QString)), func -> obj, SLOT(errorReceived(int,QString)));
            emit errorReceived(err_code, err_msg);
            disconnect(this, SIGNAL(errorReceived(int,QString)), func -> obj, SLOT(errorReceived(int,QString)));
            return false;
        }
        else return captchaProcessing(error, func, url);
    }



//    ApiFunc * wallMediaRoutine(ApiFunc * func, int offset, int count);
//    void wallMediaList(const QObject * receiver, const char * respSlot, QString uid = "0", int offset = 0, int count = 0);

//    ApiFunc * audioAlbumsRoutine(ApiFunc * func, int offset = 0);
//    void audioAlbums(const QObject * receiver, const char * respSlot, QString uid);

//    ApiFunc * audioListRoutine(ApiFunc * func);
//    void audioList(const QObject * receiver, const char * respSlot, QString uid);

//    ApiFunc * audioRecomendationRoutine(ApiFunc * func, bool byUser, bool randomize);
//    void audioRecomendation(const QObject * receiver, const char * respSlot, QString uid, bool byUser, bool randomize);

//    ApiFunc * searchAudioRoutine(ApiFunc * func, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
//    void audioSearch(const QObject * receiver, const char * respSlot, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
//    QJsonObject audioSearchSync(const QObject * receiver, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
//    QJsonObject audioSearchSync(const QObject * receiver, QString predicate, int limitation = 1);

//    ApiFunc * audioPopularRoutine(ApiFunc * func, bool onlyEng, int genreId);
//    void audioPopular(const QObject * receiver, const char * respSlot, bool onlyEng, int genreId = -1);
//    QJsonObject audioPopularSync(const QObject * receiver, bool onlyEng, int genreId = -1);

//    QString refreshAudioItemUrl(QString audio_uid);
//    QJsonObject getAudioInfo(QString audio_uid);
//    QJsonObject getAudiosInfo(QStringList audio_uid);

////    void refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids);
///

    //https://vk.com/dev/execute

    QString VkApiPrivate::authUrl() {
        QUrl url("https://oauth.vk.com/authorize");
        QUrlQuery queryParams = QUrlQuery();
        queryParams.addQueryItem("v", apiVersion());
        queryParams.addQueryItem("display", "page");
        queryParams.addQueryItem("client_id", "4332211");
        queryParams.addQueryItem("response_type", "token");
        queryParams.addQueryItem("scope", "audio,video,friends,groups,offline");
        queryParams.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");

        url.setQuery(queryParams);
        return url.toString();
    }

    QUrl VkApiPrivate::wallUrl(QString uid, QString token, int offset, int count) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        QString head;

        QString body = QString(
            "var posts = API.wall.get({ "
            "        count: limit, "
            "        owner_id: " + uid + ""
            "   });"
        );

        if (offset > 0) {
            head = QString(
                "var limit = 100;"
                "var offset = " + QString::number(offset) + "; var response = []; var look_window = limit * " + getObjLimit() + " + offset;"
                + body +
                "var count = " + QString::number(count) + ", post_items = []; var last_date = nil;"
            );
        } else {
            head = QString(
                "var limit = 100;"
                "var offset = limit; var response = []; var look_window = limit * " + getObjLimit() + ";"
                + body +
                "var count = posts.count, post_items = posts.items; var last_date = posts.items[0].date;"
            );
        }

        query.addQueryItem("code",
                               QString(
                                   head +
                                   "while (offset < count || offset < look_window) {"
                                   "    post_items.push("
                                   "        API.wall.get({"
                                   "            count: limit,"
                                   "            offset: offset,"
                                   "            owner_id: " + uid + ""
                                   "        }).items);"
                                   "        offset = offset %2b limit;"
                                   "}"

                                   "while(post_items.length > 0) {"
                                   "    var curr = post_items.pop();"
                                   "    var audios = curr.attachments@.audio %2b curr.copy_history[0].attachments@.audio %2b curr.copy_history[1].attachments@.audio;"
                                   "    if (audios.length > 0) {"
                                   "        response.unshift({"
                                   "            title: curr.text,"
                                   "            date: curr.date,"
                                   "            audios: audios"
                                   "        });"
                                   "    }"
                                   "}"

                                   "return {"
                                   "    date: last_date, "
                                   "    count: count, "
                                   "    offset: offset, "
                                   "    posts: response"
                                   "};"
                               )
                           );

        url.setQuery(query);
        return url;
    }
    QUrl VkApiPrivate::audioRefreshUrl(QStringList uids, QString token) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        query.addQueryItem("code",
                               QString(
                                   "return API.audio.getById({"
                                   "    audios: \"" + uids.join(',') + "\""
                                   "});"
                               )
                           );
        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioAlbumsUrl(QString uid, QString token, int offset) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);
        QString limit = "5";

        query.addQueryItem("code",
                           QString(
                               "var curr;"
                               "var folders_result = API.audio.getAlbums({"
                               "                count: " + limit + ", "
                               "                offset: " + QString::number(offset) + ", "
                               "                owner_id: " + uid + ""
                               "    });"
                               "var folders_count = folders_result.count;"
                               "var folders_result = folders_result.items;"
                               "var proceed_folders = {};"
                               "while(folders_result.length > 0) {"
                               "    curr = folders_result.pop();"
                               "    proceed_folders.push({"
                               "        folder_id: curr.id,"
                               "        title: curr.title,"
                               "        items: API.audio.get({"
                               "            owner_id: " + uid + ","
                               "            album_id: curr.id"
                               "        }).items "
                               "    });"
                               "};"
                               "return { "
                               "    albums: proceed_folders, "
                               "    finished: (folders_count < " + getObjLimit() + "), "
                               "    offset: " + QString::number(limit.toInt() + offset) + ""
                               "};"
                           )
                           );
        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioInfoUrl(QString uid, QString currUid, QString token) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        if (uid == currUid) {
            query.addQueryItem("code",
                               QString(
                                   "var curr;"
                                   "var groups = API.groups.get({"
                                   "            owner_id: " + uid + ", "
                                   "            count: 1000, "
                                   "            extended: 1"
                                   "    }).items;"
                                   "var proceed_groups = [];"
                                   "while(groups.length > 0) {"
                                   "    curr = groups.pop();"
                                   "    proceed_groups.push({"
                                   "        id: curr.id, "
                                   "        title: curr.name"
                                   "    });"
                                   "};"

                                   "var friends = API.friends.get({"
                                   "            user_id: " + uid + ", "
                                   "            order: \"name\", "
                                   "            fields: \"nickname\""
                                   "    });"
                                   "var proceed_friends = [];"
                                   "if (friends.count > 0) { "
                                   "    while(friends.items.length > 0) { "
                                   "        curr = friends.items.pop();"
                                   "        proceed_friends.push({ "
                                   "            id: curr.id, "
                                   "            title: curr.first_name %2b \" \" %2b curr.last_name"
                                   "        }); "
                                   "    }; "
                                   "};"

                                   "var folders_result = API.audio.getAlbums({"
                                   "            count: " + getObjLimit() + ", "
                                   "            owner_id: " + uid + ""
                                   "    });"
                                   "var folders_count = folders_result.count;"
                                   "var proceed_folders = {};"
                                   "if (folders_count > 0) { "
                                   "    while(folders_result.items.length > 0) { "
                                   "        curr = folders_result.items.pop();"
                                   "        proceed_folders.push({"
                                   "            folder_id: curr.id, "
                                   "            title: curr.title, "
                                   "            items: API.audio.get({ "
                                   "                album_id: curr.id "
                                   "            }).items "
                                   "        });"
                                   "    };"
                                   "};"
                                   "return {"
                                   "    audio_list: API.audio.get({ "
                                   "        count: 6000, owner_id: " + uid + ""
                                   "    }),"
                                   "    albums: proceed_folders, "
                                   "    groups: proceed_groups, "
                                   "    friends: proceed_friends, "
                                   "    albums_offset: " + getObjLimit() + ", "
                                   "    albums_finished: (folders_count < " + getObjLimit() + "), "
                                   "};"
                               )
           );
        } else {
            query.addQueryItem("code",
                               QString(
                                   "var folders_result = API.audio.getAlbums({ "
                                   "            count: " + getObjLimit() + ", "
                                   "            owner_id: " + uid + ""
                                   "        });"
                                   "var folders_count = folders_result.count;"
                                   "var sort_by_folders = {};"
                                   "if (folders_count > 0) { "
                                   "    while(folders_result.items.length > 0) { "
                                   "        var curr = folders_result.items.pop(); "
                                   "        sort_by_folders.push({" +
                                   "            folder_id: curr.id, "
                                   "            title: curr.title, "
                                   "            items: API.audio.get({ "
                                   "                owner_id: " + uid + ", "
                                   "                album_id: curr.id"
                                   "            }).items"
                                   "        });"
                                   "    };"
                                   "};"
                                   "return {"
                                   "    audio_list: API.audio.get({ "
                                   "        count: 6000, "
                                   "        owner_id: " + uid + ""
                                   "    }), "
                                   "    albums: sort_by_folders, "
                                   "    albums_offset: " + getObjLimit() + ", "
                                   "    albums_finished: (folders_count < " + getObjLimit() + "), "
                                   "};"
                               )
            );
        }
        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioRecomendationUrl(QString uid, bool byUser, bool randomize, QString token) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        query.addQueryItem("code",
                           QString(
                               "var recomendations = API.audio.getRecommendations({"
                                            + QString(byUser ? "user_id: " : "target_audio: ") + "\"" + uid + "\", "
                               "            count: 1000, "
                               "            shuffle: " + boolToStr(randomize) + ""
                               "});"
                               "return { "
                               "    audio_list: recomendations "
                               "};"
                           )
        );
        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioPopularUrl(bool onlyEng, QString token, int genreId) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        //offset
        query.addQueryItem("code",
           QString(
               "var recomendations = API.audio.getPopular({"
                    "only_eng: " + boolToStr(onlyEng) + ", "
                    "count: 1000 " + (genreId != -1 ? ", genre_id: " + QString::number(genreId) : "") + ""
                 "});"
               "return {audio_list: recomendations};"
           )
        );

    //    QUrl url(getApiUrl() + "audio.getPopular");
    //    QUrlQuery query = methodParams(token);

    //    query.addQueryItem("only_eng", boolToStr(onlyEng));
    //    query.addQueryItem("count", QString::number(1000));
    //    if (genreId != -1)
    //        query.addQueryItem("genre_id", QString::number(genreId));

        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioSearchLimitedUrl(QString searchStr, int limit, QString token) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        query.addQueryItem("code",
                           QString(
                               "    var items = API.audio.search({"
                               "        q: \"" + QUrl::toPercentEncoding(searchStr) + "\", count: " + QString::number(limit) + ", lyrics: 0"
                               "    }).items;"
                               "return {audio_list: search};"
                           )
        );

        url.setQuery(query);
        return url;
    }

    // sort  2 - by popularity, 1 - by duration, 0 - by creation date
    QUrl VkApiPrivate::audioSearchUrl(QString searchStr, bool autoFix, bool artistOnly, bool searchByOwn, int sort, QString token) {
        QUrl url(getApiUrl() + "execute");
        QUrlQuery query = methodParams(token);

        // count max eq 300 , limit is 1000
        query.addQueryItem("code",
                           QString(
                               "var it = 0;"
                               "var search = [];"
                               "var rule = true;"
                               "do {"
                               "    var items = API.audio.search({"
                               "        q: \"" + QUrl::toPercentEncoding(searchStr) + "\", count: 300, offset: it, lyrics: 0,"
                               "        auto_complete: " + boolToStr(autoFix) + ","
                               "        performer_only: " + boolToStr(artistOnly) + ","
                               "        sort: " + QString::number(sort) + ","
                               "        search_own: " + boolToStr(searchByOwn) + ""
                               "    }).items;"
                               "    search = search %2b items;"
                               "    it = it %2b items.length;"
                               "    rule = it < 1000 && items.length != 0;"
                               "} while(rule);"
                               "return {audio_list: search};"
                           )
        );

        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::audioLyricsUrl(QString token, QString lyrics_id) {
        QUrl url(getApiUrl() + "audio.getLyrics ");
        QUrlQuery query = methodParams(token);

        query.addQueryItem("lyrics_id", lyrics_id);
        url.setQuery(query);
        return url;
    }

    QUrl VkApiPrivate::isAppUser(QString token, QString uid) {
        QUrl url(getApiUrl() + "users.isAppUser");
        QUrlQuery query = methodParams(token);

        query.addQueryItem("user_id", uid);
        url.setQuery(query);
        return url;
    }
};

#endif // VK_REQUEST_API
