#include "logger.h"
#include <qdebug.h>

Logger * Logger::self = 0;

Logger * Logger::instance(QObject * parent) {
    if(!self)
        self = new Logger(parent);
    return self;
}


Logger::Logger(QObject * parent) : QObject(parent), out(0), file(0), m_showDate(true)  {
    connect(this, SIGNAL(write(QString,QString)), this, SLOT(writeToStream(QString,QString)));
    connect(this, SIGNAL(write(QString,QString,QString)), this, SLOT(writeToStream(QString,QString,QString)));
}

Logger::~Logger() {
    if (file != 0) {
        out -> flush();
        file -> close();
    }

    delete out;
    delete file;
}

void Logger::initiate(QString fileName, QTextEdit * editor) {
    if ((m_editor = editor)) {
        editor -> setReadOnly(true);
        editor -> setAcceptRichText(true);
    }

    if (!fileName.isEmpty()) {
        file = new QFile;
        file -> setFileName(fileName);
        file -> open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

        out = new QTextStream(file);
        out -> setCodec("UTF-8");
    }
}

void Logger::writeToStream(QString initiator, QString value) {
    if (m_editor != 0) {
        QString text;

        if (initiator != lastInitiator) {
            lastInitiator = initiator;
            text = "<hr><center><b>"+initiator+"</b></center>";
        }

        text = QString("%1<br>%2&nbsp;&nbsp;%3").arg(
            text,
            (m_showDate ? "<b>" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + "</b>" : ""),
            value
        );

        m_editor -> insertHtml(text);
    }

    if (file) {
        (*out) << initiator << " : " << value << "\n";
        out -> flush();
    }
}

void Logger::writeToStream(QString initiator, QString value, QString attr) {
    writeToStream(
        initiator,
        QString("<span style='color: blue'>%1</span>(<span style='color: green'>%2</span>)").arg(value, attr)
    );
}
