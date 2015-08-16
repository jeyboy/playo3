#ifndef IGRABBER_API
#define IGRABBER_API

#include "misc/logger.h"

#include "misc/web_utils/web_manager.h"
#include "misc/web_utils/html_parser.h"
#include "media/genres/web/target_genres.h"
#include "media/duration.h"
#include "grabber_keys.h"

#include <qdebug.h>

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>

#define DEFAULT_PREDICATE_NAME QString()
#define GRAB_DELAY 200 // ms
#define MAX_PAGE 999
#define STYLES_MAX_PAGE 50

namespace Grabber {
    class IGrabberApi {
    public:
        enum toJsonType { songs1, songs2, artists1, artists2, genres1 };
        enum ByTypeArg { sets, charts, soundtracks, by_genres, by_years, other, hits, fresh };

        virtual QJsonArray search(QString & predicate, QString & genre, int genre_id, bool is_popular, bool by_artist, int count) {
            if (!predicate.isEmpty()) {
                return search_postprocess(predicate, by_artist, genre, genre_id, count);
            } else if (!genre.isEmpty())
                return byGenre(genre, genre_id);
            else if (is_popular)
                return popular();
            else return QJsonArray();
        }

        virtual TargetGenres genresList() const { return genres; }

        virtual QJsonArray byGenre(QString /*genre*/, int /*genre_id*/) { return QJsonArray(); }

        virtual QJsonArray byChar(QChar /*target_char*/) { return QJsonArray(); }

        virtual QJsonArray byType(ByTypeArg /*target_type*/) { return QJsonArray(); }

        virtual QJsonArray popular() { return QJsonArray(); }

        QString refresh(QString refresh_page) {
            if (refresh_page.isEmpty()) return QString();

            QNetworkReply * response = WebManager::manager() -> getSync(QUrl(refresh_page));
            QString res = refresh_postprocess(response);
            delete response;
            return res;
        }

    //    virtual QJsonArray related(QUrl /*target_page*/) { return QJsonArray(); }
    protected:
        TargetGenres genres;

        virtual QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) = 0;
        QUrl baseUrl(QString predicate, QUrlQuery & query) {
            QUrl url(baseUrlStr(predicate));
            url.setQuery(query);
            return url;
        }

        virtual QUrlQuery genDefaultParams() { return QUrlQuery(); }

        virtual QString refresh_postprocess(QNetworkReply * /*response*/) { return QString(); }
        virtual QJsonArray search_postprocess(QString & /*predicate*/, bool /*by_artist*/, QString & /*genre*/, int /*genre_id*/, int /*count*/) { return QJsonArray(); }

        virtual bool toJson(toJsonType, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) = 0;

        inline QJsonArray sQuery(QUrl url, toJsonType jtype) {
            QJsonArray items; sQuery(url, items, jtype);  return items;
        }

        bool sQuery(QUrl url, QJsonArray & items, toJsonType jtype) {
            Logger::instance() -> startMark();
            QNetworkReply * response = WebManager::manager() -> openUrl(url); // some sites may redirect
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
                QThread::msleep(GRAB_DELAY);
            }

            return result;
        }
    };
}

#endif // IGRABBER_API
