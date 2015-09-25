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

class Logger : public QObject {
    Q_OBJECT
public:
    static Logger * instance(QObject * parent = 0);
    inline static void close() { delete self; }

    ~Logger();

    void initiate(QString fileName = "", QPlainTextEdit * editor = 0);
    inline void setShowDateTime(bool show) { m_showDate = show; }
    inline QPlainTextEdit * getEditor() { return m_editor; }
    inline void unregisterEditor() { m_editor = 0; }

    inline void startMark() { timer.start(); }
    inline void endMark(QString initiator, QString value) {
        emit write(initiator, value, QString::number(timer.elapsed()) % QStringLiteral(" ms (") % Info::paddedNumber(timer.nsecsElapsed()) % QStringLiteral(" ns)"));
    }

private:
    void toFile(QString initiator, QString value);
    void toEditor(QString initiator, QString value);

    explicit Logger(QObject * parent);

    QTextStream * out;
    QFile * file;
    QPlainTextEdit * m_editor;
    bool m_showDate;
    QString lastInitiator;

    QElapsedTimer timer;

    static Logger * self;

signals:
    void write(const QString & initiator, const QString & value, bool error = false);
    void write(const QString & initiator, const QString & value, const QString & attr, bool error = false);
    void write(const QString & initiator, const QString & value, const QStringList & attrs, bool error = false);

private slots:
    void writeToStream(const QString & initiator, const QString & value, bool error = false);
    void writeToStream(const QString & initiator, const QString & value, const QStringList & attrs, bool error = false);
    void writeToStream(const QString & initiator, const QString & value, const QString & attr, bool error = false);
};

#endif // LOGGER

