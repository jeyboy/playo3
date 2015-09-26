#ifndef IGRABBER_API
#define IGRABBER_API

//#include "misc/web_utils/html_parser.h"
#include "grabber_keys.h"
#include "isearchable.h"

namespace Core {
    class IGrabberApi : public ISearchable {
    public:
        enum toJsonType { songs1, songs2, artists1, artists2, genres1 };

        QString refresh(QString refresh_page) {
            if (refresh_page.isEmpty()) return QString();

            Web::Response * response = Web::Manager::prepare() -> followedGet(QUrl(refresh_page));
            QString res = refresh_postprocess(response);
            delete response;
            return res;
        }

    protected:
        virtual QString refresh_postprocess(Web::Response * /*response*/) { return QString(); }

        virtual bool toJson(toJsonType, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) = 0;

        inline QJsonArray sQuery(QUrl url, toJsonType jtype) {
            QJsonArray items; sQuery(url, items, jtype);  return items;
        }

        bool sQuery(QUrl url, QJsonArray & items, toJsonType jtype) {
            Logger::instance() -> startMark();
            Web::Response * response = Web::Manager::prepare() -> followedGet(url);
            bool res = toJson(jtype, response, items, true);
            Logger::instance() -> endMark(QStringLiteral("Grabber"), url.toString());
            return res;
        }

        inline QJsonArray lQuery(QString url, toJsonType jtype, int count, int start = 1) {
            QJsonArray items; return lQuery(url, items, jtype, count, start);
        }

        QJsonArray & lQuery(QString url, QJsonArray & result, toJsonType jtype, int count, int start = 1) {
            while (sQuery(QUrl(url.arg(QString::number(start))), result, jtype)) {
                if (start++ >= count) break;
                QThread::msleep(REQUEST_DELAY);
            }

            return result;
        }
    };
}

#endif // IGRABBER_API