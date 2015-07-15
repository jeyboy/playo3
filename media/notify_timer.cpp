#include "notify_timer.h"

NotifyTimer::NotifyTimer(QObject * parent): QTimer(parent) {}

void NotifyTimer::start(int msec) {
    emit started();
    QTimer::start(msec);
}
void NotifyTimer::start() {
    emit started();
    QTimer::start();
}

void NotifyTimer::stop()  {
    emit stoped();
    QTimer::stop();
}
