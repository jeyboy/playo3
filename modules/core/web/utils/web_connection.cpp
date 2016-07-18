#include "web_connection.h"

#include "web_manager.h"

using namespace Core::Web;

void WebConnection::checkConnection(QFutureWatcher<void> * checker) {
    Manager * manager = Manager::prepare();

    while(!checker -> isCanceled()) {
        QNetworkReply * reply = manager -> head(QNetworkRequest(QStringLiteral("http://google.com")));

        if (reply -> error() == QNetworkReply::NoError) {
            qDebug() << "~~~ CONNESTION RESTORED";
            emit connectionStatus(true);
            break;
        }

        QThread::msleep(250);
    }

    checker -> deleteLater();
    checker = 0;
}
