#ifndef WEB_API_H
#define WEB_API_H

#include <qapplication.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <qurl.h>
#include <qpixmap.h>

#include "dialogs/captchadialog.h"

#include "misc/web_utils/custom_network_access_manager.h"

#include "utils/async.h"

struct ApiFunc {
    inline ApiFunc() { }
    inline ApiFunc(const QObject * receiver, const char * respSlot, QString user_id) {
        obj = receiver;
        slot = respSlot;
        uid = user_id;
    }
    inline ~ApiFunc() {}

    QString uid;
    const QObject * obj;
    const char * slot;
    QJsonObject result;
};

class CaptchaDialog;

class WebApi : public Async {
    Q_OBJECT
public:
    WebApi(QObject * parent = 0);
    virtual ~WebApi();

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
    void apiCallFinished();  // remove later

protected:
    void startApiCall(QFuture<ApiFuncContainer *> feature); // remove later

    CaptchaDialog * captchaDialog;

    QHash<QString, QString> friends;
    QHash<QString, QString> groups;

    QString error;
};

#endif // WEB_API_H
