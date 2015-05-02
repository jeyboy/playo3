#ifndef WEB_API_H
#define WEB_API_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QUrl>
#include <QPixmap>

#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#include "dialogs/captchadialog.h"

#include "misc/web_utils/custom_network_access_manager.h"

#define REQUEST_DELAY 250 // ms

struct ApiFuncContainer {
    ApiFuncContainer() { }
    ApiFuncContainer(const QObject * receiver, const char * respSlot, QString user_id) {
        obj = receiver;
        slot = respSlot;
        uid = user_id;
    }
    ~ApiFuncContainer() {
    }

    QString uid;
    const QObject * obj;
    const char * slot;
    QJsonObject result;
};

class CaptchaDialog;

class WebApi : public QObject {
    Q_OBJECT
public:
    WebApi(QObject * parent = 0);
    virtual ~WebApi();

    inline QString getError() { return error; }

    virtual QString name() const = 0;
    virtual QString authUrl() const = 0;
    virtual QString proceedAuthResponse(const QUrl & url) = 0;

    virtual bool isConnected() = 0;

    inline CustomNetworkAccessManager * manager() const { return netManager; }

    QPixmap openRemoteImage(QString url);

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
public slots:
    void showingCaptcha();

protected:
    inline CustomNetworkAccessManager * createManager() { return new CustomNetworkAccessManager(); }

    CaptchaDialog * captchaDialog;
    QJsonObject responseToJson(QByteArray data);

    CustomNetworkAccessManager * netManager;

    QHash<QString, QString> friends;
    QHash<QString, QString> groups;

    QString error;
};

#endif // WEB_API_H
