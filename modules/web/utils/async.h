#ifndef ASYNC_H
#define ASYNC_H

#include <qhash.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

#include "misc/web_utils/json.h"

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot) {
        obj = receiver;
        slot = respSlot;
    }
    inline ~Func() {}

    QObject * obj;
    const char * slot;
};

//Q_INVOKABLE void test(QString value);

class Async : public QObject { // refactor ?
    Q_OBJECT
public:
    inline Async(QObject * parent = 0) : QObject(parent) {}
    virtual ~Async() {}

    void registerAsync(QFuture<Json::Obj> feature, Func func) {
        QFutureWatcher<Json::Obj> * initiator = new QFutureWatcher<Json::Obj>();
        connect(initiator, SIGNAL(finished()), this, SLOT(asyncFinished()));
        initiator -> setFuture(feature);
        requests.insert(initiator, func);
    }

    void registerAsync(QFuture<Json::Arr> feature, Func func) {
        QFutureWatcher<Json::Arr> * initiator = new QFutureWatcher<Json::Arr>();
        connect(initiator, SIGNAL(finished()), this, SLOT(asyncFinished2()));
        initiator -> setFuture(feature);
        requests.insert(initiator, func);
    }
protected slots:
    void asyncFinished() {
        QFutureWatcher<Json::Obj> * initiator = (QFutureWatcher<Json::Obj> *)sender();
        Func func = requests.take(initiator);
        Json::Obj res = initiator -> result();
        QMetaObject::invokeMethod(func.obj, func.slot, Qt::AutoConnection, Q_ARG(Json::Obj&, res));
        delete initiator;
    }
    void asyncFinished2() {
        QFutureWatcher<Json::Arr> * initiator = (QFutureWatcher<Json::Arr> *)sender();
        Func func = requests.take(initiator);
        Json::Arr res = initiator -> result();
        QMetaObject::invokeMethod(func.obj, func.slot, Qt::AutoConnection, Q_ARG(Json::Arr&, res));
        delete initiator;
    }

protected:
    QHash<QFutureWatcherBase *, Func> requests;
};

#endif // ASYNC_H
