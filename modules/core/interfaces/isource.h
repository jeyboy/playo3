#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>
#include <qtoolbutton.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/data_sub_types.h"

#define UID_HEAD QStringLiteral("@")

#define SOURCE_API_AUTH_JSON QStringLiteral("api_auth")
#define SOURCE_SITE_AUTH_JSON QStringLiteral("site_auth")

#define SOURCE_API_EXPIRED_AT_JSON QStringLiteral("_aa")
#define SOURCE_API_TOKEN_JSON QStringLiteral("_at")
#define SOURCE_API_USER_ID_JSON QStringLiteral("_au")

#define SOURCE_SITE_TOKEN_JSON QStringLiteral("_st")
#define SOURCE_SITE_EXPIRED_AT_JSON QStringLiteral("_sa")


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
        sf_auth_site_has = 8,

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
    public:
        ISource() : button(0) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }
        virtual inline SourceFlags defaultFlags() { return sf_none; }
        inline QString userID() const { return attrs[SOURCE_API_USER_ID_JSON].toString(); }

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

            SourceFlags flags = defaultFlags();
            bool api_flag_permit = HAS_FLAG(flags, api_flag);
            bool site_flag_permit = HAS_FLAG(flags, site_flag);

            return !api_flag_permit || api_flag_permit == apiConnected() ||
                   !site_flag_permit || site_flag_permit == siteConnected();
        }
        inline bool hasApiConnection()      { return HAS_FLAG(defaultFlags(), sf_auth_api_has); }
        inline bool hasSiteConnection()     { return HAS_FLAG(defaultFlags(), sf_auth_site_has); }
//        inline bool preferApi()             { return HAS_FLAG(defaultFlags(), sf_prefer_api); }

        inline bool isConnected()           { return apiConnected() || siteConnected(); }
        inline bool apiConnected()          { return attrs.value(SOURCE_API_AUTH_JSON, false).toBool(); }
        inline bool siteConnected()         { return attrs.value(SOURCE_SITE_AUTH_JSON, false).toBool(); }

        bool connectUser(const ConnectionType & conType = connection_restore) {
            bool res = true;

            if (conType == connection_restore) {
                res &= restoreUserConnection();
            } else {
                if (!apiConnected() && hasApiConnection()) {
                    res &= connectUserApi();
                    if (res) attrs[SOURCE_API_AUTH_JSON] = true;
                }

                if (!siteConnected() && hasSiteConnection()) {
                    res &= connectUserSite();
                    if (res) attrs[SOURCE_SITE_AUTH_JSON] = true;
                }

                if (res) initButton();
            }

            return res;
        }
        virtual bool restoreUserConnection() { return false; }
        virtual bool connectUserApi() { return false; }
        virtual bool connectUserSite() { return false; }
        inline void disconnectUser() {
            attrs.remove(SOURCE_API_AUTH_JSON); attrs.remove(SOURCE_SITE_AUTH_JSON);
            attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
            attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
            attrs.remove(SOURCE_API_USER_ID_JSON); attrs.remove(SOURCE_SITE_TOKEN_JSON);
            attrs.remove(SOURCE_SITE_EXPIRED_AT_JSON);

//            Web::Manager::removeCookies(name());

            clearAdditionals();
            initButton();
        }
        virtual void clearAdditionals() {}

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        void toJson(QJsonObject & hash) {
            QJsonObject root;
            root.insert(QStringLiteral("attrs"), QJsonObject::fromVariantHash(attrs));
            saveAdditionals(root);

            hash.insert(name(), root);
        }
        virtual void saveAdditionals(QJsonObject & /*obj*/) {}
        void fromJson(const QJsonObject & hash) {
            QJsonObject root = hash.value(name()).toObject();

            attrs = root.value(QStringLiteral("attrs")).toObject().toVariantHash();
            loadAdditionals(root);
        }
        virtual void loadAdditionals(QJsonObject & /*obj*/) {}

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
        QVariantHash attrs;

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
