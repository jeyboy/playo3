#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestApi : public virtual IQueriable {
            protected:
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
                    program = 6, web = 7, mobile = 8, android = 10
                };

                inline void setCategory(QUrlQuery & query, const CategoryTypes & cType) { setParam(query, tkn_category, (int)cType); }
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

                QString searchApiUrl(const QString & predicate = QString(), const CategoryTypes & ctype = music) {
                    QUrlQuery query = genDefaultParams(qst_api_def);

                    setSearchPredicate(query, predicate);

                    setCategory(query, ctype);
                    return baseUrlStr(qst_api_def, tkn_files, query);
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
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_API
