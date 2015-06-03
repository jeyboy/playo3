#ifndef ECHONEST_API_H
#define ECHONEST_API_H

#include "echonest_genre_api.h"
#include <qjsonobject.h>

class EchonestApi : public QObject, public EchonestGenreApi {
    Q_OBJECT
public:
    inline QString name() const { return "echonest"; }
    inline QString token() const { return "TSCA6XDZTJQ1OOJSV"; }
    inline QString baseUrl(QString predicate) { return "http://developer.echonest.com/api/v4/" + predicate; }

    ~EchonestApi() { }

    static EchonestApi * instance();
    static EchonestApi * instance(QJsonObject obj);
    inline static void close() { delete self; }
private:
    inline EchonestApi() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
