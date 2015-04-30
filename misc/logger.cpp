#include "logger.h"

Logger * Logger::self = 0;

Logger * Logger::instance(QObject * parent) {
    if(!self)
        self = new Logger(parent);
    return self;
}


Logger::Logger(QObject * parent) : QObject(parent), out(0), file(0), m_showDate(true)  {}

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
        file -> open(QIODevice::Truncate | QIODevice::Text);

        out = new QTextStream(file);
        out -> setCodec("UTF-8");
    }
}

void Logger::write(QString initiator, QString value) {
    QString text;

    if (initiator != lastInitiator) {
        lastInitiator = initiator;
        text = "<hr><center><b>"+initiator+"</b></center>";
    }

    text = QString("%1<p>%2&nbsp;&nbsp;%3</p>").arg(
        text,
        (m_showDate ? "<b>" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + "</b>" : ""),
        value

    );

    if (file)
        (*out) << text;

    if (m_editor != 0)
        m_editor -> insertHtml(text);
}
