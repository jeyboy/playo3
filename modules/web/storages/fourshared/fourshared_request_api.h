#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "../../iapi.h"

#include "fourshared_api_keys.h"
#include "misc/file_utils/filename_conversions.h"
#include "media/format.h"
#include "media/duration.h"
#include "media/genres/music_genres.h"
#include "misc/web_utils/html_parser.h"

#define FOURSHARED_OFFSET_LIMIT 1000

namespace Fourshared {
    class RequestApi : public IApi {
        enum CategoryTypes {
            music = 1, video = 2, photo = 3, archive = 4,
            book = 5,  program = 6, web = 7, mobile = 8,
            android = 10
        };

        inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, "category", (int)cType); }
        inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, "query", predicate); }
    public:
        QUrl audioSearchUrl(QString & predicate, CategoryTypes cType = music) {
            QUrlQuery query = genDefaultParams();
            setCategory(query, cType);
            setSearchPredicate(query, predicate);

            return IApi::baseUrl(QStringLiteral("files"), query);
        }
        QJsonArray audioSearch(QString & predicate, int count = 5, bool initInfo = false) {
            QJsonArray res = lQuery(
                audioSearchUrl(predicate),
                QueryRules(QStringLiteral("files"), requestLimit(), qMin(count, FOURSHARED_OFFSET_LIMIT)),
                none
            );

            return prepareAudios(res, initInfo);
        }

        QString refresh(QString refresh_page) {
            if (refresh_page.isEmpty()) return QString();

            WebManager * manager = 0;
            bool is_new = WebManager::valid(manager);
            QNetworkReply * response = manager -> getSync(refresh_page);
            QString res = Html::Document(response).find(QStringLiteral("input.jsD1PreviewUrl")).value();
            delete response;
            if (is_new) manager -> deleteLater();
            return res;
        }

    private:
        QJsonArray prepareAudios(QJsonArray & items, bool initInfo) {
            if (extractCount(items) == 0)
                return items;

            Html::Selector prevSelector("input.jsD1PreviewUrl");
            Html::Selector durSelector("input.jsD1Duration");
            Html::Selector tagsSelector(".generalID3tags .id3tag");

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);

            QJsonArray ar;
            QString ext, title, path, song_path;

            for(QJsonArray::Iterator parts_it = items.begin(); parts_it != items.end(); parts_it++) {
                QJsonArray part = (*parts_it).toArray();
                for(QJsonArray::Iterator item = part.begin(); item != part.end(); item++) {

                    QJsonObject obj, item_obj = (*item).toObject();
                    path = item_obj.value("downloadPage").toString();

                    if (initInfo) {
                        QNetworkReply * reply = manager -> getSync(path);
                        Html::Document doc(reply);
                        reply -> deleteLater();

                        song_path = doc.find(&prevSelector).value();

                        obj.insert(Grabber::duration_key, Duration::fromMillis(doc.find(&durSelector).value().toInt()));

                        Html::Set tags = doc.find(&tagsSelector);
                        for(Html::Set::Iterator tag = tags.begin(); tag != tags.end(); tag++) {
                            Html::Tag * span = (*tag) -> childTag(QStringLiteral("span"));
                            if (span) {
                                QString tag_title = span -> text();

                                if (tag_title == filetype_tag || tag_title == filetype_tag2)
                                    obj.insert(Grabber::extension_key, (*tag) -> text());
                                else if (tag_title == bitrate_tag || tag_title == bitrate_tag2)
                                    obj.insert(Grabber::bitrate_key, (*tag) -> text());
                                else if (tag_title == discretion_rate_tag || tag_title == discretion_rate_tag2)
                                    obj.insert(Grabber::discretion_rate_key, (*tag) -> text());
                                else if (tag_title == genre_tag || tag_title == genre_tag2) {
                                    int genre_id = MusicGenres::instance() -> toInt((*tag) -> text().trimmed());
                                    if (MusicGenres::instance() -> defaultInt() != genre_id)
                                        obj.insert(Grabber::genre_id_key, genre_id);
                                }

                                // year ignored at this time
                            }
                        }

                        obj.insert(Grabber::url_key, song_path);
                    } else obj.insert(Grabber::skip_info_key, true);

                    if (!initInfo || !song_path.isEmpty()) {
                        title = item_obj.value("name").toString();

                        if (FilenameConversions::extractExtension(title, ext))
                            obj.insert(Grabber::extension_key, ext);

                        obj.insert(Grabber::title_key, title);

                        obj.insert(Grabber::size_key, Format::toUnits(item_obj.value("size").toInt()));
                        obj.insert(Grabber::refresh_key, path);

                        ar << obj;
                    }

                    if (initInfo) QThread::msleep(REQUEST_DELAY);
                }
            }

            if (isNew) manager -> deleteLater();
            return ar;
        }
    };       
}

#endif // FOURSHARED_REQUEST_API
