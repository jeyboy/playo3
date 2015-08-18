#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qdebug.h>
#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "misc/logger.h"
#include "misc/web_utils/web_manager.h"
#include "media/genres/web/target_genres.h"

#include "modules/web/web_sub_types.h"

#define DEFAULT_PREDICATE_NAME QString()
#define REQUEST_DELAY 260 // ms
#define MAX_PAGE 999
#define STYLES_MAX_PAGE 50

class ISearchable {
public:
    inline virtual ~ISearchable() {}

    QString encodeStr(QString & str) { return QUrl::toPercentEncoding(str); }

    enum ByTypeArg { sets, charts, soundtracks, by_genres, by_years, other, hits, fresh };

    virtual QString name() const = 0;

    virtual Playo3::WebSubType siteType() = 0;

    virtual QJsonArray search(QString & predicate, QString & genre, bool is_popular, bool by_artist, bool by_song, int count) {
        if (!predicate.isEmpty()) {
            return search_postprocess(predicate, by_artist, by_song, genre, count);
        } else if (!genre.isEmpty())
            return byGenre(genre);
        else if (is_popular)
            return popular();
        else return QJsonArray();
    }

    inline TargetGenres genresList() {
        if (genres.isEmpty()) genres_prepocessing();
        return genres;
    }

    virtual QJsonArray byGenre(const QString & /*genre*/) { return QJsonArray(); }

    virtual QJsonArray byChar(QChar /*target_char*/) { return QJsonArray(); }

    virtual QJsonArray byType(ByTypeArg /*target_type*/) { return QJsonArray(); }

    virtual QJsonArray popular() { return QJsonArray(); }

    //    virtual QJsonArray related(QUrl /*target_page*/) { return QJsonArray(); }

    virtual QString refresh(QString refresh_page) = 0;
protected:
    virtual QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
    QUrl baseUrl(QString predicate, QUrlQuery & query) {
        QUrl url(baseUrlStr(predicate));
        url.setQuery(query);
        return url;
    }
    virtual inline QUrlQuery genDefaultParams() { return QUrlQuery(); }

    virtual QJsonArray search_postprocess(QString & /*predicate*/, bool /*by_artist*/, bool /*by_song*/, QString & /*genre*/, int /*count*/) { return QJsonArray(); }
    virtual void genres_prepocessing() {}

    TargetGenres genres;
};

#endif // ISEARCHABLE
