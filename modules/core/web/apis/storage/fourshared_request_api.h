#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_api_keys.h"
#include "modules/core/web/grabber_keys.h"

#include "modules/core/web/interfaces/auth/teu_auth.h"
#include "modules/core/web/interfaces/iapi.h"
//#include "modules/core/misc/file_utils/extensions.h"
//#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_PAGES_LIMIT 25
#define FOURSHARED_ITEMS_LIMIT 1000
#define FOURSHARED_PER_REQUEST_LIMIT 100

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestApiAuth : public TeuAuth, public IApi {
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4,
                    book = 5,  program = 6, web = 7, mobile = 8,
                    android = 10
                };

                inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, tkn_category, (int)cType); }
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_query, predicate); }

                PolyQueryRules rules(
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

                QString searchUrl(const QString & predicate = QString(), const CategoryTypes & ctype = music) {
                    QUrlQuery query = genDefaultParams();

                    setSearchPredicate(query, predicate);

                    setCategory(query, ctype);
                    return baseUrlStr(tkn_files, query);
                }

                QString audioSearchUrl(const QString & predicate = QString()) {
                    return searchUrl(predicate, music);
                }

                QString videoSearchUrl(const QString & predicate = QString()) {
                    return searchUrl(predicate, video);
                }
            protected:
                QJsonArray popularAuth(const SearchLimit & limits) {
                    QJsonArray arr;

                    if (limits.include_audio())
                        return pRequest(
                            audioSearchUrl(),
                            call_type_json,
                            rules(limits.start_offset, limits.items_limit),
                            proc_none,
                            &arr,
                            QStringList() << tkn_files
                        );

                    if (limits.include_video())
                        return pRequest(
                            videoSearchUrl(),
                            call_type_json,
                            rules(limits.start_offset, limits.items_limit),
                            proc_none,
                            &arr,
                            QStringList() << tkn_files
                        );


                    return arr;

//                    QJsonArray res = lQuery(
//                        IApi::baseUrl(files_token_key, query),
//                        QueryRules(files_token_key, requestLimit(), FOURSHARED_OFFSET_LIMIT / 5),
//                        none
//                    );
                }

                QJsonArray searchProcAuth(const SearchLimit & limits) {
                    QJsonArray arr;

                    if (limits.include_audio())
                        return pRequest(
                            audioSearchUrl(limits.predicate),
                            call_type_json,
                            rules(limits.start_offset, limits.items_limit),
                            proc_none,
                            &arr,
                            QStringList() << tkn_files
                        );

                    if (limits.include_video())
                        return pRequest(
                            videoSearchUrl(limits.predicate),
                            call_type_json,
                            rules(limits.start_offset, limits.items_limit),
                            proc_none,
                            &arr,
                            QStringList() << tkn_files
                        );


                    return arr;

//                    QJsonArray res = lQuery(
//                        audioSearchUrl(predicate),
//                        QueryRules(files_token_key, requestLimit(), qMin(limitations.total_limit, FOURSHARED_OFFSET_LIMIT)),
//                        none
//                    );
                }
            };

            class RequestApiNoAuth : virtual public Api {
                PolyQueryRules rules(
                    int offset = 0, int items_limit = FOURSHARED_ITEMS_LIMIT, int pages_limit = FOURSHARED_PAGES_LIMIT,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_limit, FOURSHARED_PAGES_LIMIT),
                    );
                }

                QJsonArray popularNoAuth(const SearchLimit & limits) {
                    QJsonArray arr;

                    if (limits.include_audio())
                        return pRequest(
                            QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/music").arg(OFFSET_TEMPLATE),
                            call_type_html,
                            rules(limits.start_offset, limits.items_limit),
                            proc_tracks1,
                            &arr
                        );

                    if (limits.include_video())
                        return pRequest(
                            QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/video").arg(OFFSET_TEMPLATE),
                            call_type_html,
                            rules(limits.start_offset, limits.items_limit),
                            proc_video1,
                            &arr
                        );

                    return arr;
                }

                virtual QJsonArray searchProcNoAuth(const SearchLimit & limits) {
                    QJsonArray arr;

                    if (limits.include_audio())
                        return pRequest(
                            QStringLiteral("http://search.4shared.com/q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                            call_type_html,
                            rules(limits.start_offset, limits.items_limit),
                            proc_tracks1,
                            &arr
                        );

                    if (limits.include_video())
                        return pRequest(
                            QStringLiteral("http://search.4shared.com/q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                            call_type_html,
                            rules(limits.start_offset, limits.items_limit),
                            proc_video1,
                            &arr
                        );

                    return arr;
                }
            };

            class RequestApi : public RequestApiAuth, public RequestApiNoAuth {
                virtual bool isConnected() = 0;

                QJsonArray popular(const SearchLimit & limits) {
                    return isConnected() ? popularAuth(limits) : popularNoAuth(limits);
                }

                QJsonArray searchProc(const SearchLimit & limits) {
                    return isConnected() ? searchProcAuth(limits) : searchProcNoAuth(limits);
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_API
