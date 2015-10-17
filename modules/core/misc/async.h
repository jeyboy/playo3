#ifndef ASYNC_H
#define ASYNC_H

#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qhash.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

#include "func.h"

//Q_INVOKABLE void test(QString value);

class Async : public QObject { // refactor ? //TODO: use qjsonvalue for unification
    Q_OBJECT
public:
    inline Async() {}
    virtual ~Async() {}

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
        QJsonObject res = initiator -> result();
        QMetaObject::invokeMethod(func.obj, func.slot, Qt::AutoConnection, Q_ARG(QJsonObject&, res));
        delete initiator;
    }
    void asyncFinished2() {
        QFutureWatcher<QJsonArray> * initiator = (QFutureWatcher<QJsonArray> *)sender();
        Func func = requests.take(initiator);
        QJsonArray res = initiator -> result();
        QMetaObject::invokeMethod(func.obj, func.slot, Qt::AutoConnection, Q_ARG(QJsonArray&, res));
        delete initiator;
    }

protected:
    QHash<QFutureWatcherBase *, Func> requests;
};

#endif // ASYNC_H
