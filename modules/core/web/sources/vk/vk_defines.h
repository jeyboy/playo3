#ifndef VK_DEFINES
#define VK_DEFINES

#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"
#include "modules/core/web/utils/js_document.h"

#include "vk_keys.h"

#define RESPONSE_TO_JSON(response) \
    Info::extractLimitedBy(response -> toText(), LSTR("<!json>"), LSTR("<!>"))

#define RESPONSE_TO_JSON_PARTS(response) \
    QStringList parts = Info::extractPartsLimitedBy(response -> toText(), LSTR("<!json>"), LSTR("<!>")); \
    QJsonArray json_parts; \
    for(QStringList::Iterator part = parts.begin(); part != parts.end(); part++) {\
        QJsonDocument doc = QJsonDocument::fromJson((*part).toUtf8()); \
        if (doc.isArray())\
            json_parts << doc.array();\
        else \
            json_parts << doc.object();\
    } \


#define RESPONSE_TO_JSON_OBJECT(response) \
    QJsonDocument::fromJson(RESPONSE_TO_JSON(response).toUtf8()).object()

#define RESPONSE_TO_JSON_ARRAY(response) \
    QJsonDocument::fromJson(RESPONSE_TO_JSON(response).toUtf8()).array()

namespace Core {
    namespace Web {
        namespace Vk {
            class Base : public virtual ISource, public virtual IQueriable {
            protected:
                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT, int pages_limit = DEFAULT_REQUESTS_LIMIT,
//                    int per_request = 100,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_limit, DEFAULT_REQUESTS_LIMIT),
                        QString(), // limit already inserted to scripts
                        0, // per_request
                        QString(), // template already inserted to scripts
                        offset
                    );
                }

                void permaToId(const QString & perma, QString & id, QString & id_type) {
                    QUrlQuery query;
                    setParam(query, tkn_screen_name, perma);

                    QJsonObject ret = sRequest(
                        baseUrlStr(qst_api, path_resole_user, query),
                        call_type_json, 0, proc_json_extract
                    );

                    id = QString::number(ret.value(tkn_object_id).toInt());
                    id_type = ret.value(tkn_type).toString();
                }

                void prepareTracks(const QJsonArray & items, QJsonArray & tracks, QHash<QString, QJsonArray> * albums = 0) {
                    for(QJsonArray::ConstIterator item = items.constBegin(); item != items.constEnd(); item++) {
                        QJsonArray track = (*item).toArray();
                        QJsonObject track_obj;

                        track_obj.insert(tkn_owner_id, JSON_CONV_STR(track[1]));
                        track_obj.insert(tkn_id, JSON_CONV_STR(track[0]));

                        QString url = JSON_CONV_STR(track[2]);
                        if (!url.isEmpty())
                            track_obj.insert(tkn_url, url);

////                                    track[9].toString().toInt() // ?lyrics_id // '0' if empty

                        track_obj.insert(tkn_artist, UnicodeDecoding::decodeHtmlEntites(track[4].toString()));
                        track_obj.insert(tkn_title, UnicodeDecoding::decodeHtmlEntites(track[3].toString()));
                        track_obj.insert(tkn_duration, track[5].toInt());
////                                    track_obj.insert(tkn_genre_id, ); // not presented

                        QString album_id = JSON_CONV_STR(track[6]);
                        if (albums && album_id.toInt() > 0)// album_id // '0' if empty
                            albums -> operator [](album_id) << track_obj;

                        tracks << track_obj;

                        // 0 - id
                        // 1 - owner_id
                        // 2 - url
                        // 3 - title
                        // 4 - artist
                        // 5 - duration
                        // 6 - album_id
                        // 7 -
                        // 8 -
                        // 9 - lyrics
                        // 10 -
                        // 11 -
                        // 12 -
                        // 13 - some token
                    }
                }

                void prepareVideo(const QJsonArray & items, QJsonArray & videos/*, QHash<QString, QJsonArray> * albums = 0*/) {
                    for(QJsonArray::ConstIterator item = items.constBegin(); item != items.constEnd(); item++) {
                        QJsonArray video = (*item).toArray();
                        QJsonObject video_obj;

                        video_obj.insert(tkn_owner_id, JSON_CONV_STR(video[0]));
                        video_obj.insert(tkn_id, JSON_CONV_STR(video[1]));

                        video_obj.insert(tkn_video_art, video[2].toString());

                        video_obj.insert(tkn_title, UnicodeDecoding::decodeHtmlEntites(video[3].toString()));
                        video_obj.insert(tkn_duration, Duration::toSecs(video[5].toString()));


//                        QString album_id = ISource::idToStr(video[6]);
//                        if (albums && album_id.toInt() > 0)// album_id // '0' if empty
//                            albums -> operator [](album_id) << video_obj;

                        videos << video_obj;

                        // 0 - owner_id
                        // 1 - id
                        // 2 - art_url
                        // 3 - title
                        // 4 - ? (73)
                        // 5 - duration ("5:43")
                        // 6 - some token
                        // 7 - ? ("")
                        // 8 - link to owner
                        // 9 - creation date
                        // 10 - views amount
                    }
                }

                QJsonValue prepareVideoCategoryBlock(const QJsonArray & items, const QString & category_id, const QString & offset) {
//                    QString offset = JSON_STR(response, LSTR("offset_token"));
//                    QJsonArray block_content = JSON_ARR(response, block_sets);
//                    if (offset.isEmpty())
//                        return prepareBlock(dmt_video, block_content);
//                    else {
//                        QueriableResponse response(block_content, offset, 0, 1, block_content.isEmpty());
//                        return prepareBlock(dmt_video, cmd_mtd_video_by_category, response, {}, {{CMD_ID, category_id}});
//                    }

//                    cat_obj.insert(tkn_more_cmd,
//                        Cmd::build(
//                            sourceType(), cmd_mtd_video_by_category,
//                            {
//                                {CMD_ID,        JSON_CSTR(cat_obj, tkn_id)},
//                                {CMD_OFFSET,    JSON_CSTR(cat_obj, LSTR("next"))}
//                            }
//                        ).toString()
//                    );

                    bool is_mixed = false;

                    QJsonArray temp;
                    for(QJsonArray::ConstIterator video = items.constBegin(); video != items.constEnd(); video++) {
                        QJsonObject video_obj = (*video).toObject();

                        if (JSON_STR(video_obj, LSTR("type")) == LSTR("album")) {
                            is_mixed = true;
                            video_obj.insert(tkn_loadable_cmd,
                                Cmd::build(
                                    sourceType(), cmd_mtd_video_by_playlist,
                                    {
                                        {
                                            CMD_ID,
                                            QString(
                                                ID_TOKEN(
                                                    JSON_CSTR(video_obj, tkn_owner_id),
                                                    JSON_CSTR(video_obj,
                                                    tkn_id),
                                                    '_'
                                                )
                                            )
                                        }
                                    }
                                ).toString()
                            );
                            video_obj.insert(tkn_media_type, dmt_video_set);
                        }

                        temp << video_obj;
                    }

                    QueriableResponse response(temp, offset, 0, 1, temp.isEmpty());
                    return prepareBlock((is_mixed ? dmt_any_video : dmt_video), cmd_mtd_video_by_category, response, {}, {{CMD_ID, category_id}, {CMD_OFFSET, offset}});
                }

                QString decodeUrl(const QString & url) {
                    QString script(
                        "var a = \"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN0PQRSTUVWXYZO123456789+/=\";"
                        "var s = {"
                        "        v: function(t) {"
                        "            return t.split("").reverse().join("")"
                        "        },"
                        "        r: function(t, i) {"
                        "            t = t.split("");"
                        "            for (var e, o = a + a, s = t.length; s--;) e = o.indexOf(t[s]), ~e && (t[s] = o.substr(e - i, 1));"
                        "            return t.join(\"\")"
                        "        },"
                        "        x: function(t, i) {"
                        "            var e = [];"
                        "            return i = i.charCodeAt(0), each(t.split(""), function(t, o) {"
                        "                e.push(String.fromCharCode(o.charCodeAt(0) ^ i))"
                        "            }), e.join(\"\")"
                        "        }"
                        "    }"
                        "function e(t) {"
                        "    if (~t.indexOf(\"audio_api_unavailable\")) {"
                        "        var i = t.split(\"?extra=\")[1].split(\"#\"),e = o(i[1]);"
                        "        if (i = o(i[0]), !e || !i) return t;"
                        "        e = e.split(String.fromCharCode(9));"
                        "        for (var a, r, l = e.length; l--;) {"
                        "            if (r = e[l].split(String.fromCharCode(11)), a = r.splice(0, 1, i)[0], !s[a]) return t;"
                        "            i = s[a].apply(null, r)"
                        "        }"
                        "        if (i && \"http\" === i.substr(0, 4)) return i"
                        "    }"
                        "    return t"
                        "}"
                        "function o(t) {"
                        "    if (!t || t.length % 4 == 1) return !1;"
                        "    for (var i, e, o = 0, s = 0, r = ""; e = t.charAt(s++);)"
                        "      e = a.indexOf(e), ~e && (i = o % 4 ? 64 * i + e : e, o ++ % 4) && (r += String.fromCharCode(255 & i >> (-2 * o & 6)));"
                        "    return r"
                        "}"
                    );

                    QString str = Js::Document::proceedJsCall(script, QStringLiteral("e"), url);
                    return str;
                }
            };
        }
    }
}

#endif // VK_DEFINES
