#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_api_keys.h"
#include "modules/core/web/grabber_keys.h"

#include "modules/core/web/interfaces/auth/teu_auth.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/misc/file_utils/extensions.h"
//#include "media/format.h"
//#include "media/duration.h"
#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_PAGES_LIMIT 25
#define FOURSHARED_ITEMS_LIMIT 1000
#define FOURSHARED_PER_REQUEST_LIMIT 100

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestApi : public TeuAuth, public IApi {
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4,
                    book = 5,  program = 6, web = 7, mobile = 8,
                    android = 10
                };

                inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, tkn_category, (int)cType); }
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_query, predicate); }

                inline void setPagination(QUrlQuery & query, int per_request = FOURSHARED_PER_REQUEST_LIMIT) {
                    setParam(query, tkn_offset, OFFSET_TEMPLATE);
                    setParam(query, tkn_limit, per_request);
                }

                PolyQueryRules rules(int items_limit = FOURSHARED_ITEMS_LIMIT, int pages_count = FOURSHARED_PAGES_LIMIT, int offset = 0, ApiCallIterType call_type = call_iter_type_page) {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_count, FOURSHARED_PAGES_LIMIT),
                        offset
                    );
                }
            public:
                QString audioSearchUrl(const QString & predicate = QString(), CategoryTypes cType = music) {
                    QUrlQuery query = genDefaultParams();

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);

                    setCategory(query, cType);
                    setPagination(query);
                    return baseUrlStr(tkn_files, query);
                }


                QJsonArray popular(const SearchLimit & /*limitations*/) {
                    // http://search.4shared.com/q/lastmonth/CAQD/1/music

                    QJsonArray res = pRequest(
                        audioSearchUrl(),
                        call_type_json,
                        rules(),
                        proc_none,
                        0,
                        tkn_files
                    );



//                    QJsonArray res = lQuery(
//                        IApi::baseUrl(files_token_key, query),
//                        QueryRules(files_token_key, requestLimit(), FOURSHARED_OFFSET_LIMIT / 5),
//                        none
//                    );

                    return prepareAudios(res);
                }

                QJsonArray search_proc(const SearchLimit & limitations) {
                    QJsonArray res = pRequest(
                        audioSearchUrl(limitations.predicate),
                        call_type_json,
                        rules(limitations.items_limit),
                        proc_none,
                        0,
                        tkn_files
                    );


//                    QJsonArray res = lQuery(
//                        audioSearchUrl(predicate),
//                        QueryRules(files_token_key, requestLimit(), qMin(limitations.total_limit, FOURSHARED_OFFSET_LIMIT)),
//                        none
//                    );

                    return prepareAudios(res);
                }

            private:
                QJsonArray prepareAudios(QJsonArray & items, bool initInfo = false) { // initInfo is very slow
                    if (items.isEmpty()) return items;

                    Html::Selector prevSelector("input.jsD1PreviewUrl");
                    Html::Selector durSelector("input.jsD1Duration");
                    Html::Selector tagsSelector(".generalID3tags .id3tag");

                    Web::Manager * manager = Web::Manager::prepare();

                    QJsonArray ar;
                    QString ext, title, path, song_path;

                    for(QJsonArray::Iterator item = items.begin(); item != items.end(); item++) {
                        QJsonObject obj, item_obj = (*item).toObject();
                        path = item_obj.value(tkn_download_page).toString();

                        if (initInfo) {
                            Html::Document doc = manager -> followedGet(QUrl(path)) -> toHtml();

                            song_path = doc.find(&prevSelector).value();

                            obj.insert(tkn_grab_duration, Duration::fromMillis(doc.find(&durSelector).value().toInt()));

                            Html::Set tags = doc.find(&tagsSelector);
                            for(Html::Set::Iterator tag = tags.begin(); tag != tags.end(); tag++) {
                                Html::Tag * span = (*tag) -> childTag(tag_span);
                                if (span) {
                                    QString tag_title = span -> text();

                                    if (tag_title == tag_filetype)
                                        obj.insert(tkn_grab_extension, (*tag) -> text());
                                    else if (tag_title == tag_bitrate)
                                        obj.insert(tkn_grab_bitrate, (*tag) -> text());
                                    else if (tag_title == tag_discretion_rate)
                                        obj.insert(tkn_grab_discretion_rate, (*tag) -> text());
                                    else if (tag_title == tag_year)
                                        obj.insert(tkn_grab_year, (*tag) -> text());
                                    else if (tag_title == tag_genre) {
                                        int genre_id = Media::MusicGenres::obj().toInt((*tag) -> text().trimmed());
                                        if (Media::MusicGenres::obj().defaultInt() != genre_id)
                                            obj.insert(tkn_grab_genre_id, genre_id);
                                    }
                                }
                            }

                            obj.insert(tkn_grab_url, song_path);
                        } else obj.insert(tkn_skip_info, true);

                        if (!initInfo || !song_path.isEmpty()) {
                            title = item_obj.value(tkn_name).toString();

                            if (Extensions::obj().extractExtension(title, ext))
                                obj.insert(tkn_grab_extension, ext);

                            obj.insert(tkn_grab_title, title);

                            obj.insert(tkn_grab_size, Info::toUnits(item_obj.value(tkn_size).toInt()));
                            obj.insert(tkn_grab_refresh, path);

                            ar << obj;
                        }

                        if (initInfo) QThread::msleep(REQUEST_DELAY);
                    }

                    return ar;
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_API
