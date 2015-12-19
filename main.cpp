#include "playo.h"
#include "single_application.h"
#include "modules/core/misc/logger.h"

#include <qapplication.h>

// QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg, QtInfoMsg, QtSystemMsg = QtCriticalMsg
void myMessageOutput(QtMsgType msgType, const QMessageLogContext & context, const QString & message) {
    OutputDebugString(reinterpret_cast<const wchar_t *>(QString(message % QStringLiteral("\n")).utf16()) );

    Logger::obj().write(
        QStringLiteral("%2 - %3 - %4").arg(QString(context.file), QString(context.function), QString::number(context.line)),
        message,
        context.category,
        msgType == QtWarningMsg || msgType == QtCriticalMsg || msgType == QtFatalMsg
    );
}


int main(int argc, char * argv[]) {
    try {
        qRegisterMetaType<QVector <int> >("QVector<int>");
        qRegisterMetaType<QTextBlock>("QTextBlock");
        qRegisterMetaType<QTextCursor>("QTextCursor");

        qputenv("QT_LOGGING_RULES", "qt.network.ssl.warning=false");

    //    QCoreApplication::setOrganizationName("BigBug");
    //    QCoreApplication::setOrganizationDomain("bigbug.sos");
    //    QCoreApplication::setApplicationName("Playo3");

        SingleApplication a(argc, argv, QStringLiteral("bigbugplayo"));
        a.setApplicationVersion(APP_VERSION);

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

        Logger::obj().initiate(QStringLiteral("log.txt"), new QPlainTextEdit());
        qInstallMessageHandler(myMessageOutput);

        Playo w;
        QObject::connect(&a, SIGNAL(messageAvailable(QString)), &w, SLOT(receiveMessage(QString)));
        if (!message.isEmpty())
            w.receiveMessage(message);

        w.show();

    //    a.setQuitOnLastWindowClosed(false);
        return a.exec();
    }

    catch(const std::runtime_error & re) {
        qDebug() << "Runtime error: " << re.what();
        return 0;
    }
    catch(const std::exception & ex) {
        // speciffic handling for all exceptions extending std::exception, except
        // std::runtime_error which is handled explicitly
        qDebug() << "Error occurred: " << ex.what();
        return 0;
    }
    catch(...) {
        // catch any other errors (that we have no information about)
        qDebug() << "Unknown failure occured. Possible memory corruption";
        return 0;
    }
}
