#include "playo.h"
#include "single_application.h"
#include "misc/logger.h"

#include <qapplication.h>

int main(int argc, char * argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");
    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");

    qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

//    QCoreApplication::setOrganizationName("BigBug");
//    QCoreApplication::setOrganizationDomain("bigbug.sos");
//    QCoreApplication::setApplicationName("Playo3");

    SingleApplication a(argc, argv, QStringLiteral("bigbugplayo"));
    a.setApplicationVersion(APP_VERSION);
    a.setStyleSheet(Stylesheets::appStyles());

    QThread::currentThread() -> setPriority(QThread::HighPriority);

    QString message;
    QStringList list = QCoreApplication::arguments();
    if (list.length() > 1) {
        list.removeFirst();
        message = list.join('|');
    }

    if (a.isRunning()) {
        if (!message.isEmpty())
            a.sendMessage(message);
//        return 0; // uncomment later
    }

    Logger::instance(QCoreApplication::instance()) -> initiate(QStringLiteral("log.txt"), new QPlainTextEdit());

    Playo w;

    QObject::connect(&a, SIGNAL(messageAvailable(QString)), &w, SLOT(receiveMessage(QString)));
    if (!message.isEmpty())
        w.receiveMessage(message);

    w.show();

//    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
