#ifndef LOGGER
#define LOGGER

#include <qplaintextedit.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>
#include <qscrollbar.h>

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

    inline void startMark() { mark = QDateTime::currentMSecsSinceEpoch(); }
    inline void endMark(QString initiator, QString value) {
        emit write(initiator, value, QString::number(QDateTime::currentMSecsSinceEpoch() - mark) + " ms");
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

    qint64 mark;

    static Logger * self;

signals:
    void write(QString initiator, QString value);
    void write(QString initiator, QString value, QString attr);

public slots:
    void writeToStream(QString initiator, QString value);
    void writeToStream(QString initiator, QString value, QString attr);
};

#endif // LOGGER

