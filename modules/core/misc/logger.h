#ifndef LOGGER
#define LOGGER

#include <qplaintextedit.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qelapsedtimer.h>
#include <qdatetime.h>
#include <qscrollbar.h>
#include <qstringbuilder.h>

#include "modules/core/misc/format.h"
#include "modules/core/interfaces/singleton.h"

class Logger : public QObject, public Core::Singleton<Logger> {
    Q_OBJECT
public:
    ~Logger();

    void write(const QString & initiator, const QString & value, bool error = false);
    void write(const QString & initiator, const QString & value, const QString & attr, bool error = false);
    void write(const QString & initiator, const QString & value, const QStringList & attrs, bool error = false);

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

    QElapsedTimer timer;

    static Logger * self;
private slots:
    void writeToStream(const QString & initiator, const QString & value, bool error = false);
    void writeToStream(const QString & initiator, const QString & value, const QStringList & attrs, bool error = false);
    void writeToStream(const QString & initiator, const QString & value, const QString & attr, bool error = false);
};

#endif // LOGGER

