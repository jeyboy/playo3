#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>
#include <qtoolbutton.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/data_sub_types.h"

#define UID_HEAD QStringLiteral("@")
#define SOURCE_FLAGS_JSON QStringLiteral("flags")

#define HAS_FLAG(flags, flag) (flags & flag) == flag;
#define APPEND_FLAG(flags, flag) flags = (SourceFlags)(flags | flag);
#define REJECT_FLAG(flags, flag) flags &= (~(flag));

namespace Core {
    enum ConnectionType : int {
        connection_restore = 1,
        connection_new = 2
    };

    enum SourceFlags {
        sf_none = 0,

        sf_prefer_api = 1,

        sf_auth_api_has = 2,
        sf_auth_api_done = 4,
        sf_auth_site_has = 8,
        sf_auth_site_done = 16,

        sf_site_search_auth_only = 32,
        sf_site_media_content_auth_only = 64,
        sf_site_user_content_auth_only = 128,
//        sf_site_genres_auth_only = 256,

        sf_api_search_auth_only = 512,
        sf_api_media_content_auth_only = 1024,
        sf_api_user_content_auth_only = 2048,
//        sf_api_genres_auth_only = 4096,

        sf_auth_mandatory =
            sf_site_search_auth_only | sf_site_media_content_auth_only | sf_site_user_content_auth_only |
            sf_api_search_auth_only | sf_api_media_content_auth_only | sf_api_user_content_auth_only
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
            bool manda_con = HAS_FLAG(flags, flag);
            return !manda_con || (manda_con && (apiConnected() || siteConnected()));
        }
    public:
        ISource() : button(0), flags(defaultFlags()) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }
        virtual inline SourceFlags defaultFlags() { return sf_none; }

        inline bool isPermitted(const PermitFlags & perm_flag = pf_search) {
            SourceFlags api_flag, site_flag;

            switch(perm_flag) {
                case pf_search: {
                    api_flag = sf_api_search_auth_only;
                    site_flag = sf_site_search_auth_only;
                }
                case pf_media_content: {
                    api_flag = sf_api_media_content_auth_only;
                    site_flag = sf_site_media_content_auth_only;
                }
                case pf_user_content: {
                    api_flag = sf_api_user_content_auth_only;
                    site_flag = sf_site_user_content_auth_only;
                }
                default: return true;
            }

            bool api_flag_permit = HAS_FLAG(flags, api_flag);
            bool site_flag_permit = HAS_FLAG(flags, site_flag);

            return !api_flag_permit || api_flag_permit == apiConnected() ||
                   !site_flag_permit || site_flag_permit == siteConnected();
        }
        inline bool hasApiConnection()      { return HAS_FLAG(flags, sf_auth_api_has); }
        inline bool apiConnected()          { return HAS_FLAG(flags, sf_auth_api_done); }
        inline bool hasSiteConnection()     { return HAS_FLAG(flags, sf_auth_site_has); }
        inline bool siteConnected()         { return HAS_FLAG(flags, sf_auth_site_done); }
        inline bool preferApi()             { return HAS_FLAG(flags, sf_prefer_api); }

        inline bool isConnected() { return apiConnected() || siteConnected(); }
        bool connectUser(const ConnectionType & conType = connection_restore) {
            bool res = true;

            if (conType == connection_restore) {
                res &= restoreUserConnection();
            } else {
                if (!apiConnected() && hasApiConnection()) {
                    res &= connectApi();
                    if (res) APPEND_FLAG(flags, sf_auth_api_done);
                }

                if (!siteConnected() && hasSiteConnection()) {
                    res &= connectSite();
                    if (res) APPEND_FLAG(flags, sf_auth_site_done);
                }
            }

            return res;
        }
        virtual bool restoreUserConnection() { return false; }
        virtual bool connectApi() { return false; }
        virtual bool connectSite() { return false; }
        virtual inline void disconnectUser() { }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        virtual void toJson(QJsonObject & hash) {
            if (flags != defaultFlags())
                hash.insert(SOURCE_FLAGS_JSON, flags);

            qDebug() << name() << "STUB TO JSON";
        }
        virtual void fromJson(const QJsonObject & hash) {
            if (hash.contains(SOURCE_FLAGS_JSON))
                flags = (SourceFlags)hash.value(SOURCE_FLAGS_JSON).toInt();

            qDebug() << name() << "STUB FROM JSON";
        }

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

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
