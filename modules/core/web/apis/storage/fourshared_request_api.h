#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_api_keys.h"
#include "modules/core/web/grabber_keys.h"

#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/misc/file_utils/extensions.h"
//#include "media/format.h"
//#include "media/duration.h"
//#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_PAGES_LIMIT 25
#define FOURSHARED_ITEMS_LIMIT 1000
#define FOURSHARED_PER_REQUEST_LIMIT 100

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestApi : public IApi {
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4,
                    book = 5,  program = 6, web = 7, mobile = 8,
                    android = 10
                };

                inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, category_token_key, (int)cType); }
                inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, query_token_key, predicate); }

                inline void setPagination(QUrlQuery & query, int per_request = FOURSHARED_PER_REQUEST_LIMIT) {
                    setParam(query, tkn_offset, OFFSET_TEMPLATE);
                    setParam(query, tkn_limit, per_request);
                }

                PolyQueryRules rules(int items_limit = FOURSHARED_ITEMS_LIMIT, int pages_count = FOURSHARED_PAGES_LIMIT, int offset = 0, ApiCallIterType call_type = call_iter_page) {
                    return PolyQueryRules(
                        call_type,
                        call_iter_offset,
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
                    return baseUrl(files_token_key, query).toString();
                }


                QJsonArray popular(const SearchLimit & /*limitations*/) {
                    // http://search.4shared.com/q/lastmonth/CAQD/1/music

                    QJsonArray res = pRequest(
                        audioSearchUrl(),
                        call_type_json,
                        rules(),
                        proc_none,
                        files_token_key
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
                        rules(limitations.total_limit),
                        proc_none,
                        files_token_key
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
                        path = item_obj.value(download_page_key).toString();

                        if (initInfo) {
                            Html::Document doc = manager -> followedGet(QUrl(path)) -> toHtml();

                            song_path = doc.find(&prevSelector).value();

                            obj.insert(duration_key, Duration::fromMillis(doc.find(&durSelector).value().toInt()));

                            Html::Set tags = doc.find(&tagsSelector);
                            for(Html::Set::Iterator tag = tags.begin(); tag != tags.end(); tag++) {
                                Html::Tag * span = (*tag) -> childTag(span_tag);
                                if (span) {
                                    QString tag_title = span -> text();

                                    if (tag_title == filetype_tag)
                                        obj.insert(extension_key, (*tag) -> text());
                                    else if (tag_title == bitrate_tag)
                                        obj.insert(bitrate_key, (*tag) -> text());
                                    else if (tag_title == discretion_rate_tag)
                                        obj.insert(discretion_rate_key, (*tag) -> text());
                                    else if (tag_title == year_tag)
                                        obj.insert(year_key, (*tag) -> text());
                                    else if (tag_title == genre_tag) {
                                        int genre_id = Media::MusicGenres::obj().toInt((*tag) -> text().trimmed());
                                        if (Media::MusicGenres::obj().defaultInt() != genre_id)
                                            obj.insert(genre_id_key, genre_id);
                                    }
                                }
                            }

                            obj.insert(url_key, song_path);
                        } else obj.insert(skip_info_key, true);

                        if (!initInfo || !song_path.isEmpty()) {
                            title = item_obj.value(name_token_key).toString();

                            if (Extensions::obj().extractExtension(title, ext))
                                obj.insert(extension_key, ext);

                            obj.insert(title_key, title);

                            obj.insert(size_key, Info::toUnits(item_obj.value(size_token_key).toInt()));
                            obj.insert(refresh_key, path);

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
