#ifndef IGRABBER_API
#define IGRABBER_API

#include "../grabber_keys.h"
#include "modules/core/interfaces/isearchable.h"

//#define MAX_TOTAL 1000

//namespace Core {
//    class IGrabberApi : public ISearchable {
//    public:
//        enum toJsonType { songs1, songs2, artists1, artists2, genres1 };
//    protected:
////        virtual bool toJson(toJsonType, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) = 0;

////        inline QJsonArray sGrabQuery(QUrl url, toJsonType jtype) {
////            QJsonArray items;
////            sQuery(url, items, jtype);
////            return items;
////        }

////        bool sGrabQuery(QUrl url, QJsonArray & items, toJsonType jtype) {
////            Logger::obj().startMark();
////            Web::Response * response = Web::Manager::prepare() -> followedGet(url);
////            bool res = toJson(jtype, response, items, true);
////            Logger::obj().endMark(QStringLiteral("Grabber-") % QString::number(items.size()) % (res ? "true" : "false"), url.toString());
////            return res;
////        }

////        inline QJsonArray lGrabQuery(QString url, toJsonType jtype, int count, int start = 1) {
////            QJsonArray items;
////            return lQuery(url, items, jtype, count, start);
////        }

////        QJsonArray & lGrabQuery(QString url, QJsonArray & result, toJsonType jtype, int count, int start = 1, int total = MAX_TOTAL) {
////            int page_limit = count + (start - 1);
////            while (sQuery(QUrl(url.arg(QString::number(start))), result, jtype)) {
////                qDebug() << "LLL" << start << page_limit << result.size() << total;
////                if (result.isEmpty() || start++ >= page_limit || result.size() >= total) break;
////                QThread::msleep(REQUEST_DELAY);
////            }

////            return result;
////        }
//    };
//}

#endif // IGRABBER_API
