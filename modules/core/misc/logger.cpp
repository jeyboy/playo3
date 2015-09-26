#include "logger.h"

Logger * Logger::self = 0;

Logger * Logger::instance(QObject * parent) {
    if(!self)
        self = new Logger(parent);
    return self;
}

Logger::Logger(QObject * parent) : QObject(parent), out(0), file(0), m_showDate(true)  {
    connect(this, SIGNAL(write(const QString &,const QString &, bool)), this, SLOT(writeToStream(const QString &,const QString &, bool)));
    connect(this, SIGNAL(write(const QString &,const QString &, const QString &, bool)), this, SLOT(writeToStream(const QString &,const QString &, const QString &, bool)));
    connect(this, SIGNAL(write(const QString &,const QString &, const QStringList &, bool)), this, SLOT(writeToStream(const QString &,const QString &, const QStringList &, bool)));
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
    if ((m_editor = editor))
        editor -> setReadOnly(true);

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
            text = "<br>" % initiator % "<br>";
        }

        text = QString("%1%2 ::: %3").arg(
            text,
            (m_showDate ? QStringLiteral("<b>") % QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy hh:mm:ss ")) % QStringLiteral("</b>") : QStringLiteral("")),
            value
        );

        bool atEnd = m_editor -> verticalScrollBar() -> maximum() - m_editor -> verticalScrollBar() -> value() < 10;
        m_editor -> appendHtml(text);

        if (atEnd)
            m_editor -> ensureCursorVisible();
    }
}

void Logger::writeToStream(const QString & initiator, const QString & value, bool error) {
    writeToStream(initiator, value, QString(), error);
}

void Logger::writeToStream(const QString & initiator, const QString & value, const QStringList & attrs, bool error) {
    writeToStream(initiator, value, attrs.join(" , "), error);
}

void Logger::writeToStream(const QString & initiator, const QString & value, const QString & attr, bool error) {
    toFile(initiator, QString("%1   :::   %2").arg(value, attr));
    toEditor(initiator,
        QString("<span style='color: " % QString(error ? QStringLiteral("red") : QStringLiteral("green")) % "'>%1</span> ::: <span style='color: darkblue'>%2</span>").arg(value, attr)
    );
}