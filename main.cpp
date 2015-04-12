#include "playo.h"
#include "single_application.h"
#include <QApplication>

int main(int argc, char * argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");

//    QCoreApplication::setOrganizationName("BigBug");
//    QCoreApplication::setOrganizationDomain("bigbug.sos");
//    QCoreApplication::setApplicationName("Playo3");

    SingleApplication a(argc, argv, "bigbugplayo");
    a.setStyleSheet(Stylesheets::appStyles());

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

    Playo w;

    QObject::connect(&a, SIGNAL(messageAvailable(QString)), &w, SLOT(receiveMessage(QString)));
    if (!message.isEmpty())
        w.receiveMessage(message);

    w.show();

//    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
