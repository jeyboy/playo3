#ifndef ECHONEST_API_H
#define ECHONEST_API_H

#include "echonest_genre_api.h"
#include "echonest_artist_api.h"
#include "echonest_song_api.h"
#include "misc/web_utils/custom_network_access_manager.h"

class EchonestApi : public QObject, public EchonestGenreApi, public EchonestArtistApi, public EchonestSongApi {
    Q_OBJECT
public:
    inline QString name() const { return "echonest"; }
    ~EchonestApi() { }

    static EchonestApi * instance();
    inline static void close() { delete self; }
protected:
    inline QString baseUrl(QString predicate) { return "http://developer.echonest.com/api/v4/" + predicate; }
    void registerQuery(QUrl url);

    inline QString token() const { return "TSCA6XDZTJQ1OOJSV"; }
    inline int requestLimit() { return 100; }

    inline int extractAmount(QJsonObject & response) {
        return response.value("response").toObject().value("total").toInt();
    }
    inline void setLimit(QUrlQuery & query, int limit, int offset = 0) {
        if (offset > 0) EchonestGenreApi::setParam(query, "start", QString::number(offset));
        EchonestGenreApi::setParam(query, "results", QString::number(limit));
    }

    bool proceedQuery(QUrl url, QJsonObject & response) {
        CustomNetworkAccessManager * manager;
        bool isNew = CustomNetworkAccessManager::validManager(manager);

        response = manager -> getToJson(QNetworkRequest(url));

        if (isNew) delete manager;
        bool status = response.value("response").toObject().value("status").toObject().value("code").toInt() == 0;

        if (!status)
            qDebug() << response.value("response").toObject().value("status").toObject().value("message").toString();

        return status;
    }

    QHash<QUrl, CallInitiator> requests;

protected slots:
    void requestFinished() {
//        QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *)sender();
//        ApiFuncContainer * func = initiator -> result();
//        connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
//        emit routineFinished(func -> result);
//        disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
//        delete func;
    }

private:
    inline EchonestApi() : QObject() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
