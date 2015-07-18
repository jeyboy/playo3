#ifndef VK_REQUEST_API
#define VK_REQUEST_API

//https://vk.com/dev/execute

#include "../iapi.h"

namespace Vk {
    class RequestApi : public IApi {
    protected:
        inline QString boolToStr(bool val) { return val ? QStringLiteral("1") : QStringLiteral("0"); }
        inline QString apiVersion() { return QStringLiteral("5.21"); }
        inline QString getApiLimit() { return QStringLiteral("20"); }

        QString authUrl() {
            QUrl url(QStringLiteral("https://oauth.vk.com/authorize"));

            QUrlQuery query = QUrlQuery();
            setParam(query, QStringLiteral("v"), apiVersion());
            setParam(query, QStringLiteral("display"), QStringLiteral("page"));
            setParam(query, QStringLiteral("client_id"), QStringLiteral("4332211"));
            setParam(query, QStringLiteral("response_type"), QStringLiteral("token"));
            setParam(query, QStringLiteral("scope"), QStringLiteral("audio,video,friends,groups,offline"));
            setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("https://oauth.vk.com/blank.html"));

            url.setQuery(query);
            return url.toString();
        }

    public:
    //    QUrl wallUrl(QString & uid) {
    //        QUrlQuery query = genDefaultParams();

    //        setParam(query, "code", QString(
    //            "var limit = 100; var offset = _1_; var finished = false; "
    //            "var response = []; var look_window = limit * " + getApiLimit() + " %2b offset; var post_items = [];"

    //            "while (offset < look_window && !finished) {"
    //            "   var items = API.wall.get({ count: limit, offset: offset, owner_id: " + uid + "}).items;"
    //            "   finished = items.length < limit;"
    //            "   post_items.push(items);"
    //            "   offset = offset %2b limit;"
    //            "}"

    //            "while(post_items.length > 0) {"
    //            "   var curr = post_items.pop();"
    //            "   var audios = curr.attachments@.audio %2b"
    //            "       curr.copy_history[0].attachments@.audio %2b"
    //            "       curr.copy_history[1].attachments@.audio %2b"
    //            "       curr.copy_history[2].attachments@.audio %2b"
    //            "       curr.copy_history[3].attachments@.audio;"
    //            "   if (audios.length > 0) {"
    //            "       response.unshift({ title: curr.text, date: curr.date, audios: audios });"
    //            "   }"
    //            "}"

    //            "return { offset: offset, posts: response, finished: finished };"
    //        ));

    //        return baseUrl("execute", query);
    //    }
    //    QJsonArray wallAudio(QString & uid) {
    //        return lQuery(wallUrl(uid), DEFAULT_LIMIT_AMOUNT, "posts");
    //    }

        QUrl audioAlbumsUrl(QString & uid) {
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"),
                QString(
                   "var curr; var count = 5;"
                   "var folders_result = API.audio.getAlbums({"
                   "                count: count, "
                   "                offset: _1_, "
                   "                owner_id: " % uid % ""
                   "    });"
                   "var folders_result = folders_result.items;"
                   "var proceed_folders = {};"
                   "while(folders_result.length > 0) {"
                   "    curr = folders_result.pop();"
                   "    proceed_folders.push({"
                   "        folder_id: curr.id,"
                   "        title: curr.title,"
                   "        items: API.audio.get({"
                   "            owner_id: " % uid % ","
                   "            album_id: curr.id"
                   "        }).items "
                   "    });"
                   "};"
                   "return { "
                   "    albums: proceed_folders, "
                   "    finished: (proceed_folders.length < count), "
                   "    offset: _1_ %2b count"
                   "};"
               )
            );

            return baseUrl(QStringLiteral("execute"), query);
        }
        QJsonArray audioAlbums(QString & uid, int offset = 0) {
            return lQuery(
                audioAlbumsUrl(uid),
                QueryRules(QStringLiteral("albums"), 5, DEFAULT_LIMIT_AMOUNT, offset),
                extract
            );
        }
        void audioAlbums(QString & uid, QJsonArray & arr, int offset = 0) {
            lQuery(
                audioAlbumsUrl(uid),
                QueryRules(QStringLiteral("albums"), 5, DEFAULT_LIMIT_AMOUNT, offset),
                arr, extract
            );
        }


        QUrl userFullInfoUrl(QString & uid) {
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"),
               QString(
                   "var curr; var proceed_groups = [];"
                   "var groups = API.groups.get({"
                   "            owner_id: " % uid % ", "
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
                   "            user_id: " % uid % ", "
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
                   "            count: " % getApiLimit() % ", "
                   "            owner_id: " % uid % ""
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
                   "        count: 6000, owner_id: " % uid % ""
                   "    }),"
                   "    albums: [proceed_folders], "
                   "    groups: proceed_groups, "
                   "    friends: proceed_friends, "
                   "    albums_offset: " % getApiLimit() % ", "
                   "    albums_finished: (folders_count < " % getApiLimit() % "), "
                   "};"
               )
           );

           return baseUrl(QStringLiteral("execute"), query);
        }
        QUrl userShortInfoUrl(QString & uid) {
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"),
               QString(
                   "var folders_result = API.audio.getAlbums({ "
                   "            count: " % getApiLimit() % ", "
                   "            owner_id: " % uid % ""
                   "        });"
                   "var folders_count = folders_result.count;"
                   "var sort_by_folders = {};"
                   "if (folders_count > 0) { "
                   "    while(folders_result.items.length > 0) { "
                   "        var curr = folders_result.items.pop(); "
                   "        sort_by_folders.push({"
                   "            folder_id: curr.id, "
                   "            title: curr.title, "
                   "            items: API.audio.get({ "
                   "                owner_id: " % uid % ", "
                   "                album_id: curr.id"
                   "            }).items"
                   "        });"
                   "    };"
                   "};"
                   "return {"
                   "    audio_list: API.audio.get({ "
                   "        count: 6000, "
                   "        owner_id: " % uid % ""
                   "    }), "
                   "    albums: [sort_by_folders], "
                   "    albums_offset: " % getApiLimit() % ", "
                   "    albums_finished: (folders_count < " % getApiLimit() % "), "
                   "};"
               )
            );

            return baseUrl(QStringLiteral("execute"), query);
        }


        QJsonObject userInfo(QString & uid, bool fullInfo = true) {
            QUrl url = fullInfo ? userFullInfoUrl(uid) : userShortInfoUrl(uid);
            QJsonObject ret = sQuery(url, extract);

            QJsonArray ar = ret.value(QStringLiteral("albums")).toArray();

            if (!ret.value(QStringLiteral("albums_finished")).toBool())
                audioAlbums(uid, ar, ret.value(QStringLiteral("albums_offset")).toInt());
            else setCount(ar, ar[0].toArray().size());

            ret.insert(QStringLiteral("albums"), ar);
            return ret;
        }

        QJsonObject userStatus(QString & uid) { // deactivated: 'deleted', id, first_name, last_name, counters
            QUrlQuery query;
            setParam(query, QStringLiteral("fields"), QStringLiteral("counters"));
            setParam(query, QStringLiteral("user_ids"), uid);

            return sQuery(baseUrl(QStringLiteral("users.get"), query)).value(QStringLiteral("response")).toArray().first().toObject();
        }

        QUrl audioRecomendationsUrl(QString & uid, bool byUser, bool randomize) {
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"),
               QString(
                   "var recomendations = API.audio.getRecommendations({"
                   + QString(byUser ? "user_id: " : "target_audio: ") % "\"" % uid % "\", "
                   "   count: 1000, "
                   "   shuffle: " % boolToStr(randomize) % ""
                   "});"
                   "return { "
                   "   audio_list: recomendations "
                   "};"
               )
            );

            return baseUrl(QStringLiteral("execute"), query);
        }

        QJsonObject audioRecomendations(QString & uid, bool byUser, bool randomize) {
            return sQuery(audioRecomendationsUrl(uid, byUser, randomize), extract);
        }


        enum SearchSort { creation_date = 0, duration = 1, popularity = 2 };

        QUrl audioSearchUrl(QString & searchStr, bool autoFix, bool artistOnly, bool searchByOwn, SearchSort sort, int limit = 1000) {
            // count max eq 300 , limit is 1000
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"), QString(
                "var limit = " % QString::number(limit) % ";"
                "var search = []; var rule = true;"
                "do {"
                "    var count = limit - search.length;"
                "    if (count > 300) count = 300;"
                "    var items = API.audio.search({"
                "        q: \"" % QUrl::toPercentEncoding(searchStr) % "\", count: count, offset: search.length, lyrics: 0,"
                "        auto_complete: " % boolToStr(autoFix) % ","
                "        performer_only: " % boolToStr(artistOnly) % ","
                "        sort: " % QString::number(sort) % ","
                "        search_own: " % boolToStr(searchByOwn) % ""
                "    }).items;"
                "    search = search %2b items;"
                "    rule = search.length < limit && items.length != 0;"
                "} while(rule);"
                "return {audio_list: search};"
            ));

            return baseUrl(QStringLiteral("execute"), query);
        }
        QJsonArray audioSearch(QString & predicate, bool onlyArtist, bool inOwn, bool mostPopular, int limit) {
            return sQuery(
                audioSearchUrl(predicate, false, onlyArtist, inOwn, mostPopular ? popularity : creation_date, qMin(1000, limit)), extract
            ).value(QStringLiteral("audio_list")).toArray();
        }

        QUrl audioSearchLimitedUrl(QString & searchStr, int limit) {
            QUrlQuery query = genDefaultParams();

            setParam(query, "code", QString(
                "    var items = API.audio.search({"
                "        q: \"" % QUrl::toPercentEncoding(searchStr) % QStringLiteral("\", count: ") % QString::number(limit) % ", lyrics: 0"
                "    }).items;"
                "return {audio_list: search};"
            ));

            return baseUrl(QStringLiteral("execute"), query);
        }

        QJsonObject audioSearchLimited(QString & predicate, int limitation) {
            return sQuery(audioSearchLimitedUrl(predicate, limitation), extract);
        }

        QUrl audioPopularUrl(bool onlyEng, int genreId) {
            QUrlQuery query = genDefaultParams();


            setParam(query, QStringLiteral("code"),
               QString(
                   "var recomendations = API.audio.getPopular({"
                        "only_eng: " + boolToStr(onlyEng) + ", "
                        "count: 1000 " + (genreId != -1 ? (", genre_id: " + QString::number(genreId)) : "") + ""
                     "});"
                   "return {audio_list: recomendations};"
               )
            );

            return baseUrl(QStringLiteral("execute"), query);
        }
        QJsonArray audioPopular(bool onlyEng, int genreId) {
            return sQuery(audioPopularUrl(onlyEng, genreId), extract).value(QStringLiteral("audio_list")).toArray();
        }

        QUrl audioRefreshUrl(QStringList & uids) {
            QUrlQuery query = genDefaultParams();

            setParam(query, QStringLiteral("code"),
               QStringLiteral("return API.audio.getById({ audios: \"") % uids.join(',') % QStringLiteral("\"});")
            );

            return baseUrl(QStringLiteral("execute"), query);
        }
        QJsonArray getAudiosInfo(QStringList & audio_uids) {
            return sQuery(audioRefreshUrl(audio_uids)).value(QStringLiteral("response")).toArray();
        }
        QJsonObject getAudioInfo(QString & audio_uid) {
            QStringList uids; uids << audio_uid;
            QJsonObject ret = getAudiosInfo(uids)[0].toObject();
            return ret;
        }

        QString refreshAudioItemUrl(QString audio_uid) {
            return getAudioInfo(audio_uid).value(QStringLiteral("url")).toString();
        }

        void nameToId(QString name, QString & id, QString & id_type) {
            QUrlQuery query;
            setParam(query, QStringLiteral("screen_name"), name);
            QJsonObject ret = sQuery(baseUrl(QStringLiteral("utils.resolveScreenName"), query)).value(QStringLiteral("response")).toObject();
            id = QString::number(ret.value(QStringLiteral("object_id")).toInt());
            id_type = ret.value(QStringLiteral("type")).toString();
        }

        QUrl audioLyricsUrl(QString & lyrics_id) {
            QUrlQuery query = genDefaultParams();
            setParam(query, QStringLiteral("lyrics_id"), lyrics_id);
            return baseUrl(QStringLiteral("audio.getLyrics"), query);
        }
    };
}

#endif // VK_REQUEST_API
