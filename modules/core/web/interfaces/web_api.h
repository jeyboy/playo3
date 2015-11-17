#ifndef WEB_API_H
#define WEB_API_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qurl.h>

#include "dialogs/user_action_dialog.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/core/misc/thread_utils.h"
#include "settings.h"

namespace Core {
    class WebApi : public QObject {
        Q_OBJECT
    public:
        WebApi();
        virtual ~WebApi() {}

        inline QString getError() { return error; }

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

        void fromJson(const QJsonObject & hash);
        void toJson(QJsonObject & hash);

    signals:
        void errorReceived(int, QString);
        void authorized();

    public slots:
        virtual inline void disconnect() { clearData(); }

        bool showingCaptcha(const QUrl & pict_url, QString & result);
        bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
        bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());

    protected:
        UserActionDialog * actionDialog;

        QHash<QString, QString> friends;
        QHash<QString, QString> groups;

        QString error;
    };
}

#endif // WEB_API_H
