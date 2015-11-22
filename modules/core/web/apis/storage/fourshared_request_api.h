#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "fourshared_api_keys.h"

#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
//#include "media/format.h"
//#include "media/duration.h"
#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_OFFSET_LIMIT 1000

namespace Core {
    namespace Web {
        using namespace Grabber;

        namespace Fourshared {
            class RequestApi : public IApi {
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4,
                    book = 5,  program = 6, web = 7, mobile = 8,
                    android = 10
                };

                inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, category_token_key, (int)cType); }
                inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, query_token_key, predicate); }
            public:
                QUrl audioSearchUrl(QString & predicate, CategoryTypes cType = music) {
                    QUrlQuery query = genDefaultParams();
                    setCategory(query, cType);
                    setSearchPredicate(query, predicate);

                    return IApi::baseUrl(files_token_key, query);
                }

                QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                    bool initInfo = false;
                    QJsonArray res = lQuery(
                        audioSearchUrl(predicate),
                        QueryRules(files_token_key, requestLimit(), qMin(limitations.count, FOURSHARED_OFFSET_LIMIT)),
                        none
                    );

                    return prepareAudios(res, initInfo);
                }

                QString refresh(QString refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    QNetworkReply * response = Web::Manager::prepare() -> followedGet(refresh_page);
                    QString res = Html::Document(response).find("input.jsD1PreviewUrl").value();

                    delete response;
                    return res;
                }

                QString downloadLink(QString refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    QNetworkReply * response = Web::Manager::prepare() -> followedGet(QUrl(down_base_url % refresh_page.mid(12)));
                    QString res = Html::Document(response).find("a[href~'/download/']").link();

                    delete response;
                    return res;
                }

            private:
                QJsonArray prepareAudios(QJsonArray & items, bool initInfo) {
                    if (items.isEmpty()) return items;

                    Html::Selector prevSelector("input.jsD1PreviewUrl");
                    Html::Selector durSelector("input.jsD1Duration");
                    Html::Selector tagsSelector(".generalID3tags .id3tag");

                    Web::Manager * manager = Web::Manager::prepare();

                    QJsonArray ar;
                    QString ext, title, path, song_path;

                    for(QJsonArray::Iterator parts_it = items.begin(); parts_it != items.end(); parts_it++) {
                        QJsonArray part = (*parts_it).toArray();
                        for(QJsonArray::Iterator item = part.begin(); item != part.end(); item++) {

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
                                        else if (tag_title == genre_tag) {
                                            int genre_id = Media::MusicGenres::obj().toInt((*tag) -> text().trimmed());
                                            if (Media::MusicGenres::obj().defaultInt() != genre_id)
                                                obj.insert(genre_id_key, genre_id);
                                        }

                                        // year ignored at this time
                                    }
                                }

                                obj.insert(url_key, song_path);
                            } else obj.insert(skip_info_key, true);

                            if (!initInfo || !song_path.isEmpty()) {
                                title = item_obj.value(name_token_key).toString();

                                if (FilenameConversions::extractExtension(title, ext))
                                    obj.insert(extension_key, ext);

                                obj.insert(title_key, title);

                                obj.insert(size_key, Info::toUnits(item_obj.value(size_token_key).toInt()));
                                obj.insert(refresh_key, path);

                                ar << obj;
                            }

                            if (initInfo) QThread::msleep(REQUEST_DELAY);
                        }
                    }

                    return ar;
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_API
