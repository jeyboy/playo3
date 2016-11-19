#ifndef LOGGER
#define LOGGER

#include <qplaintextedit.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qelapsedtimer.h>
#include <qdatetime.h>
#include <qscrollbar.h>
#include <qstringbuilder.h>
#include <qapplication.h>
#include <qfontmetrics.h>

#include "modules/core/misc/format.h"
#include "modules/core/interfaces/singleton.h"

#define TIME_MARK QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy hh:mm:ss "))

class Logger : public QObject, public Core::Singleton<Logger> {
    Q_OBJECT
public:
    enum LogLevel : int {
        log_info = 0,
        log_error = 1,
        log_url
    };

    ~Logger();

    static void dump(const QByteArray & content) {
        QString p = QCoreApplication::applicationDirPath() % '/' % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz") % QStringLiteral(".html");
        QFile f(p);
        if (f.open(QFile::WriteOnly)) {
            f.write(content);
            f.close();
        }
    }

    void write(const QString & initiator, const QString & value, LogLevel level = log_info);
    void write(const QString & initiator, const QString & value, const QString & attr, LogLevel level = log_info);
    void write(const QString & initiator, const QString & value, const QStringList & attrs, LogLevel level = log_info);

    void initiate(QString fileName = "", QPlainTextEdit * editor = 0);
    inline void setShowDateTime(bool show) { m_showDate = show; }
    inline QPlainTextEdit * getEditor() { return m_editor; }
    inline void unregisterEditor() { m_editor = 0; }

    inline void startMark() { timer.start(); }
    inline void endMark(QString initiator, QString value) {
        write(initiator, value, QString::number(timer.elapsed()) % QStringLiteral(" ms (") % Info::paddedNumber(timer.nsecsElapsed()) % QStringLiteral(" ns)"));
    }

private:
    void toFile(const QString & initiator, const QString & value);
    void toEditor(const QString & initiator, const QString & value);

    Logger(); friend class Core::Singleton<Logger>;

    QTextStream * out;
    QFile * file;
    QPlainTextEdit * m_editor;
    bool m_showDate;
    QString lastInitiator;
    QFontMetrics * fm;

    QElapsedTimer timer;

    static Logger * self;
private slots:
    void writeToStream(const QString & initiator, const QString & value, int level = log_info);
    void writeToStream(const QString & initiator, const QString & value, const QStringList & attrs, int level = log_info);
    void writeToStream(const QString & initiator, const QString & value, const QString & attr, int level = log_info);
};

#endif // LOGGER

