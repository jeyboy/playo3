#include "playo.h"
#include "single_application.h"
#include "modules/core/misc/logger.h"

#include <qapplication.h>

// QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg, QtInfoMsg, QtSystemMsg = QtCriticalMsg
void myMessageOutput(QtMsgType msgType, const QMessageLogContext & context, const QString & message) {
    #ifdef Q_OS_WIN
        OutputDebugString(reinterpret_cast<const wchar_t *>(QString(message % QStringLiteral("\n")).utf16()) );
    #else
        const char symbols[] = { 'I', 'E', '!', 'X' };
        QString output = QString("[%1] %2").arg(symbols[msgType]).arg(message);
        std::cerr << output.toStdString() << std::endl;
//        if(type == QtFatalMsg) abort();
    #endif

    Logger::obj().write(
        QStringLiteral("%1 - %2 - %3").arg(QString(context.file), QString(context.function), QString::number(context.line)),
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

        QFont font;
        font.setFamily(font.defaultFamily());
        #ifdef HAVE_X11
            font.setFamily(QStringLiteral("Ubuntu"));
            font.setPointSize(8);
        #endif
        a.setFont(font);

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
