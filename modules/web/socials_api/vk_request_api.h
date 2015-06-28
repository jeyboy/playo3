#ifndef VK_REQUEST_API
#define VK_REQUEST_API

//https://vk.com/dev/execute

#include "../iapi.h"

class VkRequestApi : public IApi {
protected:
    inline QString boolToStr(bool val) { return val ? "1" : "0"; }
    inline QString apiVersion() { return "5.21"; }
    inline QString getApiLimit() { return "20"; }

    QString authUrl() {
        QUrl url("https://oauth.vk.com/authorize");

        QUrlQuery query = QUrlQuery();
        setParam(query, "v", apiVersion());
        setParam(query, "display", "page");
        setParam(query, "client_id", "4332211");
        setParam(query, "response_type", "token");
        setParam(query, "scope", "audio,video,friends,groups,offline");
        setParam(query, "redirect_uri", "https://oauth.vk.com/blank.html");

        url.setQuery(query);
        return url.toString();
    }

    QUrl wallUrl(QString uid) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code", QString(
            "var limit = 100; var offset = %%1; finished = false"
            "var response = []; var look_window = limit * " + getApiLimit() + " + offset; var post_items = [];"

            "while (offset < look_window && !finished) {"
            "   var items = API.wall.get({ count: limit, offset: offset, owner_id: " + uid + "}).items;"
            "   finished = items.length == 0;"
            "   post_items = post_items + items;"
            "   offset = offset + limit;"
            "}"

            "while(post_items.length > 0) {"
            "   var curr = post_items.pop();"
            "   var audios = curr.attachments@.audio +"
            "       curr.copy_history[0].attachments@.audio +"
            "       curr.copy_history[1].attachments@.audio +"
            "       curr.copy_history[2].attachments@.audio +"
            "       curr.copy_history[3].attachments@.audio;"
            "   if (audios.length > 0) {"
            "       response.unshift({ title: curr.text, date: curr.date, audios: audios });"
            "   }"
            "}"

            "return { offset: offset, posts: response, finished: finished };"
        ));

        return baseUrl("execute", query);
    }
    QJsonArray wallMedia(QString uid) {
        return proceedQuery(wallUrl(uid), DEFAULT_LIMIT_AMOUNT, "posts");
//        QJsonObject doc;
//        QVariantList res;

//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
//        QNetworkReply * m_http;

//        while(true) {
//            m_http = netManager -> getSync(QNetworkRequest(
//                VkApiPrivate::wallUrl(func -> uid, token(), offset)
//            ));

//            if (!responseRoutine(m_http, func, doc))
//                break;

//            doc = doc.value("response").toObject();

//            res.append(doc.value("posts").toArray().toVariantList());

//            offset = doc.value("offset").toInt();
//            count = doc.value("count").toInt();
//            if (offset >= count)
//                break;
//        }

//        func -> result.insert("posts", QJsonArray::fromVariantList(res));
//        return func;
    }

    QUrl audioAlbumsUrl(QString uid) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
            QString(
               "var curr; var count = 5;"
               "var folders_result = API.audio.getAlbums({"
               "                count: count, "
               "                offset: %%1, "
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
               "    finished: (folders_count < " + getApiLimit() + "), "
               "    offset: %%1 + count"
               "};"
           )
        );

        return baseUrl("execute", query);
    }
    QJsonArray audioAlbums(QString uid, int offset = 0) {
        return proceedQuery(wallUrl(uid), DEFAULT_LIMIT_AMOUNT, "albums", false, offset);

//        QJsonObject doc;
//        QVariantList res, temp;
//        res.append(func -> result.value("albums").toArray().toVariantList());

//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
//        QNetworkReply * m_http;

//        while(true) {
//            m_http = netManager -> getSync(QNetworkRequest(
//                VkApiPrivate::audioAlbumsUrl(func -> uid, token(), offset)
//            ));

//            if (!responseRoutine(m_http, func, doc))
//                break;

//            doc = doc.value("response").toObject();

//            temp = doc.value("albums").toArray().toVariantList();
//            if (temp.isEmpty())
//                break;

//            temp.append(res);
//            res = temp;
//    //        res.append(doc.value("albums").toArray().toVariantList());
//            offset = doc.value("offset").toInt();
//            if (doc.value("finished").toBool())
//                break;
//        }

//        func -> result.insert("albums", QJsonArray::fromVariantList(res));

//        return func;
    }


    QUrl userFullInfoUrl(QString uid) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
           QString(
               "var curr; var proceed_groups = [];"
               "var groups = API.groups.get({"
               "            owner_id: " + uid + ", "
               "            count: 1000, "
               "            extended: 1"
               "    }).items;"
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
               "            count: " + getApiLimit() + ", "
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
               "    albums_offset: " + getApiLimit() + ", "
               "    albums_finished: (folders_count < " + getApiLimit() + "), "
               "};"
           )
       );

       return baseUrl("execute", query);
    }
    QUrl userShortInfoUrl(QString uid) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
           QString(
               "var folders_result = API.audio.getAlbums({ "
               "            count: " + getApiLimit() + ", "
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
               "    albums_offset: " + getApiLimit() + ", "
               "    albums_finished: (folders_count < " + getApiLimit() + "), "
               "};"
           )
        );

        return baseUrl("execute", query);
    }


    QJsonObject userInfo(QString uid, bool fullInfo = true) {
        QUrl url = fullInfo ? userFullInfoUrl(uid) : userShortInfoUrl(uid);
        QJsonObject ret = proceedQuery(url);
        ret = extractBody(ret);

        if (!ret.value("albums_finished").toBool()) {
            QJsonArray ar = audioAlbums(uid, ret.value("albums_offset").toInt());
            ret.value("albums").toArray().append(ar);
        }

        return ret;
//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

//        m_http = netManager -> getSync(QNetworkRequest(url));
//        if (responseRoutine(m_http, func, func -> result)) {
//            func -> result = func -> result.value("response").toObject();

//            if (!func -> result.value("albums_finished").toBool()) {
//                int offset = func -> result.value("albums_offset").toInt();
//                audioAlbumsRoutine(func, offset);
//            }
//        }

//        return func;
    }

    QUrl audioRecomendationUrl(QString uid, bool byUser, bool randomize) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
           QString(
               "var recomendations = API.audio.getRecommendations({"
               + QString(byUser ? "user_id: " : "target_audio: ") + "\"" + uid + "\", "
               "   count: 1000, "
               "   shuffle: " + boolToStr(randomize) + ""
               "});"
               "return { "
               "   audio_list: recomendations "
               "};"
           )
        );

        return baseUrl("execute", query);
    }

    QJsonObject audioRecomendation(QString uid, bool byUser, bool randomize) {
        return proceedQuery(audioRecomendationUrl(uid, byUser, randomize));

//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

//        QUrl url = VkApiPrivate::audioRecomendationUrl(
//            uid,
//            byUser,
//            randomize,
//        );

//        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
//        if (responseRoutine(m_http, func, func -> result)) {
//            func -> result = func -> result.value("response").toObject();
//        }

//        return func;
    }


    enum SearchSort : int {
        creation_date = 0,
        duration = 1,
        popularity = 2
    };

    QUrl audioSearchUrl(QString searchStr, bool autoFix, bool artistOnly, bool searchByOwn, SearchSort sort) {
        // count max eq 300 , limit is 1000
        QUrlQuery query = genDefaultParams();

        setParam(query, "code", QString(
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
        ));

        return baseUrl("execute", query);
    }
    QJsonObject searchAudio(QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
        return proceedQuery(audioSearchUrl(predicate, false, onlyArtist, inOwn, mostPopular ? popularity : creation_date));


//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

//        QUrl url = VkApiPrivate::audioSearchUrl(
//            predicate,
//            false,
//            onlyArtist,
//            inOwn,
//            mostPopular ? 2 : 0,
//            token()
//        );

//        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
//        if (responseRoutine(m_http, func, func -> result))
//            func -> result = func -> result.value("response").toObject();

//        return func;
    }

    QUrl audioSearchLimitedUrl(QString searchStr, int limit) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code", QString(
            "    var items = API.audio.search({"
            "        q: \"" + QUrl::toPercentEncoding(searchStr) + "\", count: " + QString::number(limit) + ", lyrics: 0"
            "    }).items;"
            "return {audio_list: search};"
        ));

        return baseUrl("execute", query);
    }

    QJsonObject searchAudioLimited(QString predicate, int limitation) {
        return proceedQuery(audioSearchLimitedUrl(predicate, limitation));


//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
//        ApiFunc * func = new ApiFunc(receiver, 0, "");

//        QUrl url = VkApiPrivate::audioSearchLimitedUrl(
//            predicate,
//            limitation,
//            token()
//        );

//        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
//        QJsonObject res;
//        if (responseRoutine(m_http, func, res))
//            res = res.value("response").toObject();

//        delete func;
//        return res;
    }

    QUrl audioPopularUrl(bool onlyEng, int genreId) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
           QString(
               "var recomendations = API.audio.getPopular({"
                    "only_eng: " + boolToStr(onlyEng) + ", "
                    "count: 1000 " + (genreId != -1 ? ", genre_id: " + QString::number(genreId) : "") + ""
                 "});"
               "return {audio_list: recomendations};"
           )
        );

        return baseUrl("execute", query);
    }
    QJsonObject audioPopular(bool onlyEng, int genreId) {
        return proceedQuery(audioPopularUrl(onlyEng, genreId));

//        CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

//        QUrl url = VkApiPrivate::audioPopularUrl(
//            onlyEng,
//            token(),
//            genreId
//        );

//        QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
//        if (responseRoutine(m_http, func, func -> result))
//            func -> result = func -> result.value("response").toObject();

//        return func;
    }

    QUrl audioRefreshUrl(QStringList uids) {
        QUrlQuery query = genDefaultParams();

        setParam(query, "code",
           QString(
               "return API.audio.getById({"
               "    audios: \"" + uids.join(',') + "\""
               "});"
           )
        );

        return baseUrl("execute", query);
    }
    QJsonObject getAudiosInfo(QStringList audio_uids) {
        return proceedQuery(audioRefreshUrl(audio_uids));


//        QUrl url = VkApiPrivate::audioRefreshUrl(audio_uids, token());

//        CustomNetworkAccessManager * netManager;
//        bool new_manager = CustomNetworkAccessManager::validManager(netManager);

//    //    QNetworkReply * reply = netManager -> getSync(QNetworkRequest(url));

//    //    QJsonObject doc = responseToJson(reply -> readAll());

//    //    reply -> close();
//    //    delete reply;

//        QJsonObject doc = netManager -> getToJson(QNetworkRequest(url));

//        if (new_manager)
//            delete netManager;

//        return doc;
    }
    QJsonObject getAudioInfo(QString audio_uid) {
        QStringList uids; uids << audio_uid;
        return proceedQuery(audioRefreshUrl(uids));
    }

    QString refreshAudioItemUrl(QString audio_uid) {
        return getAudioInfo(audio_uid).value("url").toString();
    }

    QUrl audioLyricsUrl(QString lyrics_id) {
        QUrlQuery query = genDefaultParams();
        setParam(query, "lyrics_id", lyrics_id);
        return baseUrl("audio.getLyrics", query);
    }
};

#endif // VK_REQUEST_API
