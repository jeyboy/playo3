#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_api_keys.h"
#include "modules/core/web/grabber_keys.h"

#include "modules/core/web/interfaces/iapi.h"
//#include "modules/core/misc/file_utils/extensions.h"
//#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_PAGES_LIMIT 10
#define FOURSHARED_ITEMS_LIMIT 1000
#define FOURSHARED_PER_REQUEST_LIMIT 100

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestApi : public IApi {
            protected:
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
                    program = 6, web = 7, mobile = 8, android = 10
                };

                inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, tkn_category, (int)cType); }
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_query, predicate); }
                inline void setGenreFilter(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genre, genre); }
                inline void setArtistFilter(QUrlQuery & query, const QString & artist) { setParam(query, tkn_artist, artist); }

                PolyQueryRules rulesApi(
                    int offset = 0, int items_limit = FOURSHARED_ITEMS_LIMIT, int pages_limit = FOURSHARED_PAGES_LIMIT,
                    int per_request = FOURSHARED_PER_REQUEST_LIMIT,
                    ApiCallIterType call_type = call_iter_type_page)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_limit, FOURSHARED_PAGES_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), FOURSHARED_PER_REQUEST_LIMIT),
                        tkn_offset,
                        offset
                    );
                }

                PolyQueryRules rulesSite(
                    int offset = 0, int items_limit = FOURSHARED_ITEMS_LIMIT,
                    int pages_limit = FOURSHARED_PAGES_LIMIT, ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_limit, FOURSHARED_PAGES_LIMIT)
                    );
                }

                QString searchApiUrl(const QString & predicate = QString(), const CategoryTypes & ctype = music) {
                    QUrlQuery query = genDefaultParams();

                    setSearchPredicate(query, predicate);

                    setCategory(query, ctype);
                    return baseUrlStr(tkn_files, query);
                }

                QString audioSearchApiUrl(const QString & predicate = QString()) {
                    return searchApiUrl(predicate, music);
                }

                QString videoSearchApiUrl(const QString & predicate = QString()) {
                    return searchApiUrl(predicate, video);
                }

                QJsonValue popularApi(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                audioSearchApiUrl(),
                                call_type_json,
                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_json_extract,
                                QStringList() << tkn_files
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                videoSearchApiUrl(),
                                call_type_json,
                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_json_extract,
                                QStringList() << tkn_files
                            )
                        );


                    return res;
                }

                QJsonValue popularSite(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/music").arg(OFFSET_TEMPLATE),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_tracks1
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/video").arg(OFFSET_TEMPLATE),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_video1
                            )
                        );

                    return res;
                }

                QJsonValue searchProcApi(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                audioSearchApiUrl(limits.predicate),
                                call_type_json,
                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_json_extract,
                                QStringList() << tkn_files
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                videoSearchApiUrl(limits.predicate),
                                call_type_json,
                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_json_extract,
                                QStringList() << tkn_files
                            )
                        );


                    return res;
                }

                QJsonValue searchProcSite(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_tracks1
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_video1
                            )
                        );

                    return res;
                }

//                QJsonValue userInfo(/*QString & uid, InfoType fullInfo = info_all*/) {
//                    QString url = fullInfo == info_all ? userFullInfoUrl(uid) : userAudiosUrl(uid);

//                    QJsonObject ret = sRequest(
//                        url,
//                        call_type_json,
//                        0,
//                        proc_json_extract
//                    );


////                    QJsonObject ret = sQuery(url, extract);

//                    QJsonArray ar = ret.value(tkn_albums).toArray();

//                    if (!ret.value(tkn_albums_finished).toBool())
//                        audioAlbums(uid, ar, ret.value(tkn_albums_offset).toInt());

//                    ret.insert(tkn_albums, ar);
//                    return ret;
//                }
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_API
