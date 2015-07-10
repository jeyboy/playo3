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

void Logger::initiate(QString fileName, QPlainTextEdit * editor) {
    if ((m_editor = editor)) {
        editor -> setReadOnly(true);
    }

    if (!fileName.isEmpty()) {
        file = new QFile;
        file -> setFileName(fileName);
        file -> open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

        out = new QTextStream(file);
        out -> setCodec("UTF-8");
    }
}

void Logger::toFile(QString initiator, QString value) {
    if (file) {
        (*out) << initiator << " : " << value << "\n";
        out -> flush();
    }
}
void Logger::toEditor(QString initiator, QString value) {
    if (m_editor != 0) {
        QString text;

        if (initiator != lastInitiator) {
            lastInitiator = initiator;
            text = "<br>" + initiator + "<br>";
        }

        text = QString("%1%2 ::: %3").arg(
            text,
            (m_showDate ? "<b>" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + "</b>" : ""),
            value
        );

        bool atEnd = m_editor -> verticalScrollBar() -> maximum() - m_editor -> verticalScrollBar() -> value() < 10;
        m_editor -> appendHtml(text);

        if (atEnd) {
            m_editor -> ensureCursorVisible();
        }
    }
}

void Logger::writeToStream(QString initiator, QString value) {
    toEditor(initiator, value);
    toFile(initiator, value);
}

void Logger::writeToStream(QString initiator, QString value, QStringList attrs, bool error) {
    writeToStream(initiator, value, attrs.join(" , "), error);
}

void Logger::writeToStream(QString initiator, QString value, QString attr, bool error) {
    toFile(initiator, QString("%1   :::   %2").arg(value, attr));
    toEditor(initiator,
        QString("<span style='color: " + QString(error ? "red" : "green") + "'>%1</span> ::: <span style='color: darkblue'>%2</span>").arg(value, attr)
    );
}
