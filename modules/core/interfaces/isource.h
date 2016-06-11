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

    enum SourceFlags {
        sf_none = 0,

        sf_auth_api_has = 2,
        sf_auth_api_done = 4,
        sf_auth_site_has = 8,
        sf_auth_site_done = 16,

        sf_search_auth_only = 32,
        sf_media_content_auth_only = 64,
        sf_user_content_auth_only = 128,

        sf_auth_mandatory = sf_search_auth_only | sf_media_content_auth_only | sf_user_content_auth_only
    };

    enum PermitFlags {
        pf_none = 0,
        pf_search = 1,
        pf_media_content = 2,
        pf_user_content = 4
    };

    namespace Web { class Sociable; }

    class ISource {
        inline bool isAuthedFor(const SourceFlags & flag) {
            bool manda_con = flags & flag > 0;
            return !manda_con || (manda_con && (apiConnected() || siteConnected()));
        }
    public:
        ISource() : button(0), flags(sf_none) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }
        virtual inline SourceFlags defaultFlags() { return sf_none; }

        inline bool isPermitted(const PermitFlags & perm_flag = pf_search) {
            switch(perm_flag) {
                case pf_search: {
                    bool mand = (flags & sf_search_auth_only) > 0;
                    return mand == isAuthedFor(sf_search_auth_only);
                }
                case pf_media_content: {
                    bool mand = (flags & sf_media_content_auth_only) > 0;
                    return mand == isAuthedFor(sf_media_content_auth_only);
                }
                case pf_user_content: {
                    bool mand = (flags & sf_user_content_auth_only) > 0;
                    return mand == isAuthedFor(sf_user_content_auth_only);
                }
                default: return true;
            }
        }
        inline bool hasApiConnection() { return flags & sf_auth_api_has; }
        inline bool apiConnected() { return flags & sf_auth_api_done; }
        inline bool hasSiteConnection() { return flags & sf_auth_site_has; }
        inline bool siteConnected() { return flags & sf_auth_site_done; }
        inline bool searchWithAuthOnly() { return flags & sf_search_auth_only; }

        inline bool isConnected() { return apiConnected() || siteConnected(); }
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
        SourceFlags flags;

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> followedGet(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
