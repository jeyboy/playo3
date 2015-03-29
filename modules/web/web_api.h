#ifndef WEB_API_H
#define WEB_API_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QPixmap>

#include "misc/web_utils/custom_network_access_manager.h"

class WebApi : public QObject {
    Q_OBJECT
public:
    WebApi();
    ~WebApi();

    inline QString getError() { return error; }

    virtual QString name() const = 0;
    virtual QString authUrl() const = 0;
    virtual QString proceedAuthResponse(const QUrl & url) = 0;

    inline virtual bool isConnected() { return false; }

    inline CustomNetworkAccessManager * manager() const { return netManager; }
    inline CustomNetworkAccessManager * createManager() { return new CustomNetworkAccessManager(); }

    QPixmap openRemoteImage(QString url);
    QNetworkReply * syncRequest(QNetworkReply * m_http);

    void clearData();

    inline void addFriend(QString uid, QString name) { friends.insert(uid, name); }
    inline void addGroup(QString uid, QString name) { groups.insert(uid, name); }

    inline QHash<QString, QString> friendsList() const { return friends; }
    inline QHash<QString, QString> groupsList() const { return groups; }

    void fromJson(QJsonObject & hash);
    QJsonObject & toJson(QJsonObject & root);

protected:
    QJsonObject responseToJson(QByteArray data);

    CustomNetworkAccessManager * netManager;

    QHash<QString, QString> friends;
    QHash<QString, QString> groups;

    QString error;
};

#endif // WEB_API_H
