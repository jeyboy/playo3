#ifndef ASYNC_H
#define ASYNC_H

#include <qjsonobject.h>
#include <qhash.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, char * respSlot) {
        obj = receiver;
        slot = respSlot;
    }
    inline ~Func() {}

    QObject * obj;
    char * slot;
};

class Async : public QObject { // refactor ?
    Q_OBJECT
public:
    Async(QObject * parent = 0);
    virtual ~Async();

    void registerAsync(QFuture<QJsonObject> feature, Func func) {
        QFutureWatcher<QJsonObject> * initiator = new QFutureWatcher<QJsonObject>();
        connect(initiator, SIGNAL(finished()), this, SLOT(asyncFinished()));
        initiator -> setFuture(feature);
        requests.insert(initiator, func);
    }

    void registerAsync(QFuture<QJsonArray> feature, Func func) {
        QFutureWatcher<QJsonArray> * initiator = new QFutureWatcher<QJsonArray>();
        connect(initiator, SIGNAL(finished()), this, SLOT(asyncFinished2()));
        initiator -> setFuture(feature);
        requests.insert(initiator, func);
    }
protected slots:
    void asyncFinished() {
        QFutureWatcher<QJsonObject> * initiator = (QFutureWatcher<QJsonObject> *)sender();
        Func func = requests.take(initiator);
        QMetaObject::invokeMethod(func.obj, func.slot, Q_ARG(QJsonObject, initiator -> result()));
        delete initiator;
    }
    void asyncFinished2() {
        QFutureWatcher<QJsonArray> * initiator = (QFutureWatcher<QJsonArray> *)sender();
        Func func = requests.take(initiator);
        QMetaObject::invokeMethod(func.obj, func.slot, Q_ARG(QJsonArray, initiator -> result()));
        delete initiator;
    }

protected:
    QHash<QFutureWatcherBase *, Func> requests;
};

#endif // ASYNC_H
