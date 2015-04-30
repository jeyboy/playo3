#ifndef LOGGER
#define LOGGER

#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger : public QObject {
    Q_OBJECT
public:
    static Logger * instance(QObject * parent = 0);
    inline static void close() { delete self; }

    ~Logger();

    void initiate(QString fileName = "", QTextEdit * editor = 0);
    inline void setShowDateTime(bool show) { m_showDate = show; }
    inline QTextEdit * getEditor() { return m_editor; }

    inline void startMark() { mark = QDateTime::currentMSecsSinceEpoch(); }
    inline void endMark(QString initiator, QString value) {
        write(initiator, QString("%1(%2 ms)").arg(value, QString::number(QDateTime::currentMSecsSinceEpoch() - mark)));
    }

private:
    explicit Logger(QObject * parent);

    QTextStream * out;
    QFile * file;
    QTextEdit * m_editor;
    bool m_showDate;
    QString lastInitiator;

    qint64 mark;
//    QHash<QString, qint64> marks;

    static Logger * self;

public slots:
    void write(QString initiator, QString value);
};

#endif // LOGGER

