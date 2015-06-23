#ifndef ASYNC_H
#define ASYNC_H

#include <qjsonobject.h>
#include <qhash.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

struct Func {
    inline Func() { }
    inline Func(const QObject * receiver, const char * respSlot) {
        obj = receiver;
        slot = respSlot;
    }
    inline ~Func() {}

    const QObject * obj;
    const char * slot;
};

class Async : public QObject {
    Q_OBJECT
public:
    Async(QObject * parent = 0);
    virtual ~Async();

    inline QString getError() { return error; }

    virtual QString name() const = 0;
    virtual QString authUrl() = 0;

    virtual bool isConnected() = 0;

    void clearData();

    inline void addFriend(QString uid, QString name) {
        if (!uid.isEmpty() && !name.isEmpty())
            friends.insert(uid, name);
    }
    inline void addGroup(QString uid, QString name) {
        if (!uid.isEmpty() && !name.isEmpty())
            groups.insert(uid, name);
    }

    inline QHash<QString, QString> friendsList() const { return friends; }
    inline QHash<QString, QString> groupsList() const { return groups; }

    void fromJson(QJsonObject & hash);
    void toJson(QJsonObject & hash);

signals:
    void responseReady(QString);
    void routineFinished(QJsonObject &);
    void errorReceived(int, QString);
    void authorized();

public slots:
    void showingCaptcha();
    virtual void proceedAuthResponse(const QUrl & url) = 0;

protected slots:
    void asyncFinished();

protected:
    QHash<QFutureWatcherBase *, Func> requests;
};

#endif // ASYNC_H
