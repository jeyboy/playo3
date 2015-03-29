//#ifndef IP_CHECKER_H
//#define IP_CHECKER_H

//#include <QObject>
//#include <QUrl>
//#include "media/media_player/utils/web/custom_network_access_manager.h"

////TODO: add ip check by timer
//class IpChecker : public QObject {
//    Q_OBJECT
//public:
//    static IpChecker * instance();
//    static IpChecker * instance(QString currIp);
//    static void close() {
//        delete self;
//    }

//    ~IpChecker() {

//    }

//    bool isLocalOnly() const;
//    bool isInitialized() const;
//    bool isProccessed() const;

//    QString currentIp() const;

//protected slots:
//    void ipResponse();
//    void ipChecking();

//signals:
//    void ipChanged();

//protected:
//    void initIp();
//    bool hasIp();

//    IpChecker(QString currIp = "") : QObject() {
//        ip = currIp;
//        initialized = !ip.isEmpty();
//        noInternet = false;
//        inProgress = false;
//        netManager = new CustomNetworkAccessManager();

//        ipChecking();

//        QObject::connect(&checkTimer, SIGNAL(timeout()), this, SLOT(ipChecking()));
//        checkTimer.start(60000); // every 1 minute
//    }

//    static IpChecker *self;

//    CustomNetworkAccessManager * netManager;

//    QString ip;
//    bool noInternet, inProgress, initialized;
//    QTimer checkTimer;
//};

//#endif // IP_CHECKER_H
