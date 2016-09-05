#ifndef VK_DEFINES
#define VK_DEFINES

#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "vk_keys.h"

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

                        track_obj.insert(tkn_owner_id, track[1].toString().toInt());
                        track_obj.insert(tkn_id, track[0].toString().toInt());

                        QString url = track[2].toString();
                        if (!url.isEmpty())
                            track_obj.insert(tkn_url, url);

////                                    track[9].toString().toInt() // ?lyrics_id // '0' if empty

                        track_obj.insert(tkn_artist, UnicodeDecoding::decodeHtmlEntites(track[4].toString()));
                        track_obj.insert(tkn_title, UnicodeDecoding::decodeHtmlEntites(track[3].toString()));
                        track_obj.insert(tkn_duration, track[5].toInt());
////                                    track_obj.insert(tkn_genre_id, ); // not presented

                        QString album_id = ISource::idToStr(track[6]);
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
            };
        }
    }
}

#endif // VK_DEFINES
