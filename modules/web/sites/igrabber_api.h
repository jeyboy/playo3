#ifndef IGRABBER_API
#define IGRABBER_API

#include "misc/web_utils/html.h"
#include "media/duration.h"
#include "grabber_keys.h"
#include "modules/isearchable.h"

namespace Grabber {
    class IGrabberApi : public ISearchable {
    public:
        enum toJsonType { songs1, songs2, artists1, artists2, genres1 };

        QString refresh(QString refresh_page) {
            if (refresh_page.isEmpty()) return QString();

            WebResponse * response = WebManager::manager() -> followedGet(QUrl(refresh_page));
            QString res = refresh_postprocess(response);
            delete response;
            return res;
        }

    protected:
        virtual QString refresh_postprocess(WebResponse * /*response*/) { return QString(); }

        virtual bool toJson(toJsonType, QNetworkReply * reply, Json::Arr & json, bool removeReply = false) = 0;

        inline Json::Arr sQuery(QUrl url, toJsonType jtype) {
            Json::Arr items; sQuery(url, items, jtype);  return items;
        }

        bool sQuery(QUrl url, Json::Arr & items, toJsonType jtype) {
            Logger::instance() -> startMark();
            WebResponse * response = WebManager::manager() -> followedGet(url);
            bool res = toJson(jtype, response, items, true);
            Logger::instance() -> endMark(QStringLiteral("Grabber"), url.toString());
            return res;
        }

        inline Json::Arr lQuery(QString url, toJsonType jtype, int count, int start = 1) {
            Json::Arr items; return lQuery(url, items, jtype, count, start);
        }

        Json::Arr & lQuery(QString url, Json::Arr & result, toJsonType jtype, int count, int start = 1) {
            while (sQuery(QUrl(url.arg(QString::number(start))), result, jtype)) {
                if (start++ >= count) break;
                QThread::msleep(REQUEST_DELAY);
            }

            return result;
        }
    };
}

#endif // IGRABBER_API
