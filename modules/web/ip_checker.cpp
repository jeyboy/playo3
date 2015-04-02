//#include "ip_checker.h"

//IpChecker *IpChecker::self = 0;

//IpChecker *IpChecker::instance() {
//    if(!self)
//        self = new IpChecker();
//    return self;
//}

//IpChecker * IpChecker::instance(QString currIp) {
//    if(!self)
//        self = new IpChecker(currIp);
//    else
//        qDebug() << "Alert !!!!!!!!!!!!   ip did not setted";
//    return self;
//}

//bool IpChecker::isLocalOnly() const {
//    return noInternet;
//}

//bool IpChecker::isInitialized() const {
//    return initialized;
//}

//bool IpChecker::isProccessed() const {
//    return inProgress;
//}

//void IpChecker::initIp() {
//    inProgress = true;
//    QNetworkReply * reply = netManager -> get(QNetworkRequest(QUrl("http://whatismyip.org/")));
//    connect(reply, SIGNAL(finished()), SLOT(ipResponse()));
//}

//bool IpChecker::hasIp() {
//    QString guessedGatewayAddress;

//    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
//        if (address.protocol() == QAbstractSocket::IPv4Protocol || address.protocol() == QAbstractSocket::IPv6Protocol) {
//            guessedGatewayAddress = address.toString();

//            if (guessedGatewayAddress == ip) {
//                return true;
//            }
//        }
//    }

//    return false;
//}

//QString IpChecker::currentIp() const {
//    return ip;
//}

//void IpChecker::ipResponse() {
//    QNetworkReply * reply = (QNetworkReply*)QObject::sender();

//    if(reply -> error() == QNetworkReply::NoError) {
//        QString newIp = reply -> readAll();

//        QRegExp rx("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
//        rx.indexIn(newIp);
//        QStringList list = rx.capturedTexts();

//        if (!list.isEmpty()) {
//            newIp = list.first();

//            if (!newIp.isEmpty()) {
//                qDebug() << "received IP " << newIp << " " << ip;
//                if (ip != newIp && initialized)
//                    emit ipChanged();

//                ip = newIp;
//                initialized = true;
//            }
//        }
//    } else {
//        noInternet = true;
//    }
//    reply -> deleteLater();
//    inProgress = false;
//    qDebug() << "IP: " << ip;
//}

//void IpChecker::ipChecking() {
//    qDebug() << "CHECK IP";
//    if (!initialized || (initialized && !hasIp()))
//        initIp();
//}
