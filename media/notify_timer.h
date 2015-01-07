#ifndef NOTIFY_TIMER_H
#define NOTIFY_TIMER_H

#include <QTimer>

class NotifyTimer : public QTimer {
    Q_OBJECT

public:
    NotifyTimer(QObject * parent);

public slots:
    void start(int msec);
    void start();
    void stop();

signals:
    void started();
    void stoped();
};


#endif // NOTIFY_TIMER_H
