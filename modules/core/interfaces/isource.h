#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/data_sub_types.h"

#define UID_HEAD QStringLiteral("@")

namespace Core {
    enum ConnectionType : int {
        connection_restore = 1,
        connection_oauth = 2,
        connection_manual = 4
    };

    class ISource {
    public:
        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline virtual bool isRestorable() { return true; }
        inline virtual bool isSearchable() { return false; }
        inline virtual bool isSociable() { return false; }
        inline virtual bool isShareable() { return false; }

        inline virtual bool isConnected() { return true; }
        virtual inline bool connect_user(ConnectionType /*conType*/ = connection_restore) { return false; }
        virtual inline void disconnect_user() { }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        virtual void toJson(QJsonObject & /*hash*/) { qDebug() << name() << "TO JSON"; } // stub
        virtual void fromJson(const QJsonObject & /*hash*/) { qDebug() << name() << "FROM JSON"; } // stub

        virtual inline QString refresh(const QString & refresh_page) {
            if (refresh_page.isEmpty() || !isRestorable()) return QString();
            return refresh_process(take_refresh_page(refresh_page));
        }
    protected:
        virtual Web::Response * take_refresh_page(const QString & refresh_page) { return Web::Manager::prepare() -> followedGet(QUrl(refresh_page)); }
        virtual QString refresh_process(Web::Response * response) { delete response; return QString(); }
    };
}

#endif // ISOURCE
