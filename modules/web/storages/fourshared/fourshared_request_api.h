#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "../../iapi.h"
#include "fourshared_api_keys.h"
#include "misc/file_utils/filename_conversions.h"
#include "media/format.h"

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

            return baseUrl(QStringLiteral("files"), query);
        }
        QJsonArray audioSearch(QString & predicate, int count = 5) {
            QJsonArray res = lQuery(
                audioSearchUrl(predicate),
                QueryRules(QStringLiteral("files"), requestLimit(), qMin(count, FOURSHARED_OFFSET_LIMIT)),
                wrap
            );

            return prepareAudios(res);
        }
    private:
        bool retreivePreviewUrl(QString & path, QString & song_path, WebManager * manager) {
//            response = manager -> getToJson(QNetworkRequest(url), post_proc & wrap);
        }

        QJsonArray prepareAudios(QJsonArray & items) {
            if (items.isEmpty()) return items;

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);

            QJsonArray ar;
            QString ext, title, path, song_path;

            for(QJsonArray::Iterator item = items.begin(); item != items.end(); item++) {
                QJsonObject obj, item_obj = (*item).toObject();
                path = item_obj.value("downloadPage").toString();

                if (retreivePreviewUrl(path, song_path, manager)) {
                    title = item_obj.value("name").toString();

                    if (FilenameConversions::extractExtension(title, ext))
                        obj.insert(Grabber::extension_key, ext);

                    obj.insert(Grabber::title_key, title);

                    obj.insert(Grabber::size_key, Format::toUnits(item_obj.value("size").toInt()));
                    obj.insert(Grabber::refresh_key, path);
                    obj.insert(Grabber::url_key, song_path);

                    ar << obj;
                }
            }

            if (isNew) manager -> deleteLater();
            return ar;
        }
    };       
}

#endif // FOURSHARED_REQUEST_API
