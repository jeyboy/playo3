#ifndef WEB_CONNECTION
#define WEB_CONNECTION

#include <QtConcurrent/QtConcurrentRun>
#include <qfuturewatcher.h>

#include "modules/core/interfaces/singleton.h"

namespace Core {
    namespace Web {
        class WebConnection : public QObject, public Singleton<WebConnection> {
            Q_OBJECT

            QFutureWatcher<void> * connection_checker;

            void checkConnection(QFutureWatcher<void> * checker);

            friend class Singleton<WebConnection>;
            WebConnection() : connection_checker(0) {}
            ~WebConnection() {
                if (connection_checker)
                    connection_checker -> cancel();
            }
        public:
            bool status() { return connection_checker == 0; }

            void check() {
                if (!connection_checker) {
                    emit connectionStatus(false);
                    connection_checker = new QFutureWatcher<void>();
                    connection_checker -> setFuture(QtConcurrent::run(this, &WebConnection::checkConnection, connection_checker));
                }
            }

        signals:
            void connectionStatus(bool);
        };
    }
}

#endif // WEB_CONNECTION
