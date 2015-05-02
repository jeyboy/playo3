#include "vk_api_private.h"

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

QUrl VkApiPrivate::audioRecomendationUrl(QString uid, bool byUser, QString token) {
    QUrl url(getApiUrl() + "execute");
    QUrlQuery query = methodParams(token);

    query.addQueryItem("code",
                       QString(
                           "var recomendations = API.audio.getRecommendations({"
                                        + QString(byUser ? "user_id: " : "target_audio: ") + uid + ", "
                           "            count: 1000, "
                           "            shuffle: 0"
                           "}).items;"
                           "return { "
                           "    audio_list: recomendations "
                           "};"
                       )
    );
    url.setQuery(query);
    return url;
}

QUrl VkApiPrivate::audioPopularUrl(bool onlyEng, QString token, int genreId) {
    //INFO: result through execute method limited by 200 items


//    QUrl url(getApiUrl() + "execute");
//    QUrlQuery query = methodParams(token);

//    query.addQueryItem("code",
//                       QString(
//                           "var recomendations = API.audio.getPopular({"
//                                "only_eng: " + boolToStr(onlyEng) + ", "
//                                "count: 1000 "
//                                (genreId != -1 ? ", genre_id: " + QString::number(genreId) : "") + ""
//                             "}).items;"
//                           "return {audio_list: recomendations};"
//                       )
//    );

    QUrl url(getApiUrl() + "audio.getPopular");
    QUrlQuery query = methodParams(token);

    query.addQueryItem("only_eng", boolToStr(onlyEng));
    query.addQueryItem("count", QString::number(1000));
    if (genreId != -1)
        query.addQueryItem("genre_id", QString::number(genreId));

    url.setQuery(query);
    return url;

    // response: [{}]
}

// sort  2 - by popularity, 1 - by duration, 0 - by creation date
QUrl VkApiPrivate::audioSearchUrl(QString searchStr, bool autoFix, bool artistOnly, bool searchByOwn, int sort, QString token) {
    QUrl url(getApiUrl() + "execute");
    QUrlQuery query = methodParams(token);

    // count max eq 300 , limit is 1000
    //TODO: add loop for 1000 items reading
    query.addQueryItem("code",
                       QString(
                           "var search = [];"
                           "search.push(API.audio.search({"
                                "q: " + searchStr + ", "
                                "count: 300, "
                                "auto_complete: " + boolToStr(autoFix) + ", "
                                "lyrics: 0, "
                                "performer_only: " + boolToStr(artistOnly) + ", "
                                "sort: " + QString::number(sort) + ", "
                                "search_own: " + boolToStr(searchByOwn) + ""
                           "}).items);"
                           "return {audio_list: recomendations};"
                       )
    );

    url.setQuery(query);
    return url;

    // response: [{}]
}

QUrl VkApiPrivate::isAppUser(QString token, QString uid) {
    QUrl url(getApiUrl() + "users.isAppUser");
    QUrlQuery query = methodParams(token);

    query.addQueryItem("user_id", uid);
    url.setQuery(query);
    return url;
}

//QUrl VkApi::getAudioListUrl() const { return QUrl(getApiUrl() + "audio.get"); }
//QUrl VkApi::getAudioCountUrl() const { return QUrl(getApiUrl() + "audio.getCount"); }
//QUrl VkApi::getAudioSearchUrl() const { return QUrl(getApiUrl() + "audio.search"); }
//QUrl VkApi::getAudioCopyUrl() const { return QUrl(getApiUrl() + "audio.add"); }
//QUrl VkApi::getAudioRemoveUrl() const { return QUrl(getApiUrl() + "audio.delete"); }

//QUrl VkApi::getAudioAlbumsListUrl() const { return QUrl(getApiUrl() + "audio.getAlbums"); }
//QUrl VkApi::getAudioAlbumAddUrl() const { return QUrl(getApiUrl() + "audio.addAlbum"); }
//QUrl VkApi::getAudioAlbumEditUrl() const { return QUrl(getApiUrl() + "audio.editAlbum"); }
//QUrl VkApi::getAudioAlbumRemoveUrl() const { return QUrl(getApiUrl() + "audio.deleteAlbum"); }
//QUrl VkApi::getAudioAlbumMoveToUrl() const { return QUrl(getApiUrl() + "audio.moveToAlbum"); }

//QUrl VkApi::getAudioSaveServerUrl() const { return QUrl(getApiUrl() + "audio.getUploadServer"); }
//QUrl VkApi::getAudioSaveUrl() const { return QUrl(getApiUrl() + "audio.save"); }

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

QUrlQuery VkApiPrivate::methodParams(QString & token) {
    QUrlQuery query = QUrlQuery();

    query.addQueryItem("v", apiVersion());
    query.addQueryItem("access_token", token);

    return query;
}
