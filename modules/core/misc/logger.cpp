#include "logger.h"

Logger::Logger() : QObject(), out(0), file(0), m_showDate(true)  {}

Logger::~Logger() {
    if (file != 0) {
        out -> flush();
        file -> close();
    }

    delete out;
    delete file;
}

void Logger::write(const QString & initiator, const QString & value, LogLevel level) {
    QMetaObject::invokeMethod(
        this,
        "writeToStream",
        Qt::AutoConnection,
        Q_ARG(const QString &, initiator),
        Q_ARG(const QString &, value),
        Q_ARG(int, level)
    );
}
void Logger::write(const QString & initiator, const QString & value, const QString & attr, LogLevel level) {
    QMetaObject::invokeMethod(
        this,
        "writeToStream",
        Qt::AutoConnection,
        Q_ARG(const QString &, initiator),
        Q_ARG(const QString &, value),
        Q_ARG(const QString &, attr),
        Q_ARG(int, level)
    );
}
void Logger::write(const QString & initiator, const QString & value, const QStringList & attrs, LogLevel level) {
    QMetaObject::invokeMethod(
        this,
        "writeToStream",
        Qt::AutoConnection,
        Q_ARG(const QString &, initiator),
        Q_ARG(const QString &, value),
        Q_ARG(const QStringList &, attrs),
        Q_ARG(int, level)
    );
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

void Logger::toFile(const QString & initiator, const QString & value) {
    if (file) {
        (*out) << initiator << " : " << value << "\n";
        out -> flush();
    }
}
void Logger::toEditor(const QString & initiator, const QString & value) {
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

void Logger::writeToStream(const QString & initiator, const QString & value, int level) {
    writeToStream(initiator, value, QString(), level);
}

void Logger::writeToStream(const QString & initiator, const QString & value, const QStringList & attrs, int level) {
    writeToStream(initiator, value, attrs.join(" , "), level);
}

void Logger::writeToStream(const QString & initiator, const QString & value, const QString & attr, int level) {
    toFile(initiator, QString("%1   :::   %2").arg(value, attr));
    toEditor(initiator,
        QString("<span style='color: " %
            QString(level == log_error ? QStringLiteral("red") : level == log_info ? QStringLiteral("green") : QStringLiteral("blue")) %
                "'>%1</span> ::: <span style='color: darkblue'>%2</span>").arg(value, attr)
    );
}
