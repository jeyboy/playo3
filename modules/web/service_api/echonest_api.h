#ifndef ECHONEST_API_H
#define ECHONEST_API_H

#include "echonest_genre_api.h"
#include "misc/web_utils/custom_network_access_manager.h"

class EchonestApi : public QObject, public EchonestGenreApi {
    Q_OBJECT
public:
    inline QString name() const { return "echonest"; }
    inline QString token() const { return "TSCA6XDZTJQ1OOJSV"; }
    inline QString baseUrl(QString predicate) { return "http://developer.echonest.com/api/v4/" + predicate; }
    void registerQuery(QUrl url);

    ~EchonestApi() { }

    static EchonestApi * instance();
    static EchonestApi * instance(QJsonObject obj);
    inline static void close() { delete self; }
protected:
    inline int extractAmount(QJsonObject & response) {
        return response.value("response").toObject().value("total").toInt();
    }

    bool proceedQuery(QUrl url, QJsonObject & response) {
        CustomNetworkAccessManager * manager;
        bool isNew = CustomNetworkAccessManager::validManager(manager);

        QNetworkReply * m_http = manager -> getSync(QNetworkRequest(url));
        response = manager -> replyToJson(m_http);

        m_http -> close();
        delete m_http;

        if (isNew) delete manager;
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
    inline EchonestApi() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
