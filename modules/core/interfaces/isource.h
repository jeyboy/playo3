#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>
#include <qtoolbutton.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/data_sub_types.h"

#define UID_HEAD QStringLiteral("@")

namespace Core {
    enum ConnectionType : int {
        connection_restore = 1,
        connection_oauth = 2,
        connection_manual = 4
    };

    namespace Web { class Sociable; }

    class ISource {
    public:
        ISource() : button(0) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }

        virtual inline bool connectionRequired() { return false; }
        virtual inline bool isConnected() { return true; }
        virtual inline bool connectUser(const ConnectionType & /*conType*/ = connection_restore) { return false; }
        virtual inline void disconnectUser() { }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        virtual void toJson(QJsonObject & /*hash*/) { qDebug() << name() << "STUB TO JSON"; } // stub
        virtual void fromJson(const QJsonObject & /*hash*/) { qDebug() << name() << "STUB FROM JSON"; } // stub

        virtual inline bool isRefreshable() { return true; }
        virtual inline QString refresh(const QString & refresh_page) {
            if (!isRefreshable() || refresh_page.isEmpty()) return QString();
            return refreshProc(takeRefreshPage(refresh_page));
        }

        virtual QToolButton * initButton(QWidget * /*parent*/ = 0) { return button; }

        void openTab(const QString & userID);
        void openRecomendations(const QString & userID);
        void openRelationTab(Web::Sociable * currApi);
    protected:
        QToolButton * button;

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> followedGet(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
