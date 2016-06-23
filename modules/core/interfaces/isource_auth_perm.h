#ifndef ISOURCE_AUTH_PERM
#define ISOURCE_AUTH_PERM

#include <qstring.h>
#include <qtoolbutton.h>

#include "modules/core/web/utils/web_manager.h"

#define SOURCE_API_AUTH_JSON QStringLiteral("api_auth")
#define SOURCE_SITE_AUTH_JSON QStringLiteral("site_auth")

#define SOURCE_API_EXPIRED_AT_JSON QStringLiteral("_aa")
#define SOURCE_API_TOKEN_JSON QStringLiteral("_at")
#define SOURCE_API_USER_ID_JSON QStringLiteral("_au")

#define SOURCE_SITE_USER_ID_JSON QStringLiteral("_su")
#define SOURCE_SITE_HASH_JSON QStringLiteral("_sh")
#define SOURCE_SITE_TOKEN_JSON QStringLiteral("_st")
#define SOURCE_SITE_EXPIRED_AT_JSON QStringLiteral("_sa")


#define HAS_FLAG(flags, flag) (flags & flag) == flag
#define APPEND_FLAG(flags, flag) flags = (SourceFlags)(flags | flag)
#define REJECT_FLAG(flags, flag) flags &= (~(flag))

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
        sf_feeds_has = 16,

        sf_site_search_auth_only = 32,
        sf_site_media_content_auth_only = 64,
        sf_site_user_content_auth_only = 128,
        sf_site_feeds_auth_only = 256,

        sf_prefer_site_search = 512,
        sf_prefer_site_media_content = 1024,
        sf_prefer_site_user_content = 2048,
        sf_prefer_site_feeds = 4096,

        sf_api_search_auth_only = 8192,
        sf_api_media_content_auth_only = 16384,
        sf_api_user_content_auth_only = 32768,
        sf_api_feeds_auth_only = 65536,

//        sf_prefer_api_search = 131072,
//        sf_prefer_api_media_content = 262144,
//        sf_prefer_api_user_content = 524288,
//        sf_prefer_api_feeds = 1048576,

        sf_site_auth_mandatory = sf_site_search_auth_only | sf_site_media_content_auth_only | sf_site_user_content_auth_only | sf_site_feeds_auth_only,
        sf_api_auth_mandatory = sf_api_search_auth_only | sf_api_media_content_auth_only | sf_api_user_content_auth_only | sf_api_feeds_auth_only,

        sf_auth_mandatory = sf_site_auth_mandatory | sf_api_auth_mandatory
    };

    enum Permissions : int {
        perm_none = 0,
        perm_site = 1,
        perm_api = 2
    };

    enum PermitRequest {
        pr_none = 0,
        pr_search = 1,
        pr_media_content = 2,
        pr_user_content = 4,
        pr_feed = 8
    };

    class ISourceAuthPerm : public QObject {
        Q_OBJECT
    public:
        ISourceAuthPerm() : button(0) {}

        virtual inline SourceFlags defaultFlags() { return sf_none; }

        inline void setApiExpiration(const QString & expiration) { attrs[SOURCE_API_EXPIRED_AT_JSON] = expiration; }
        inline QString apiExpiration() const { return attrs[SOURCE_API_EXPIRED_AT_JSON].toString(); }

        inline void setApiUserID(const QString & user_id) { attrs[SOURCE_API_USER_ID_JSON] = user_id; }
        inline QString apiUserID() const { return attrs[SOURCE_API_USER_ID_JSON].toString(); }

        inline void setSiteUserID(const QString & user_id) { attrs[SOURCE_SITE_USER_ID_JSON] = user_id; }
        inline QString siteUserID() const { return attrs[SOURCE_SITE_USER_ID_JSON].toString(); }

        QString userID(const PermitRequest & req_perm = pr_user_content) {
            Permissions perm = permissions(req_perm);
            return perm == perm_site ? siteUserID() : apiUserID();
        }

        inline void setApiToken(const QString & token) { attrs[SOURCE_API_TOKEN_JSON] = token; }
        inline QString apiToken() const { return attrs[SOURCE_API_TOKEN_JSON].toString(); }
        inline void setSiteToken(const QString & token) { attrs[SOURCE_SITE_TOKEN_JSON] = token; }
        inline QString siteToken() const { return attrs[SOURCE_SITE_TOKEN_JSON].toString(); }
        inline void setSiteHash(const QString & token) { attrs[SOURCE_SITE_HASH_JSON] = token; }
        inline QString siteHash() const { return attrs[SOURCE_SITE_HASH_JSON].toString(); }

        Permissions permissions(const PermitRequest & req_perm = pr_search);
        inline bool hasApiConnection()      { return HAS_FLAG(defaultFlags(), sf_auth_api_has); }
        inline bool hasSiteConnection()     { return HAS_FLAG(defaultFlags(), sf_auth_site_has); }
//        inline bool preferApi()             { return HAS_FLAG(defaultFlags(), sf_prefer_api); }

        inline bool isConnected()           { return apiConnected() || siteConnected(); }
        inline bool apiConnected()          { return attrs.value(SOURCE_API_AUTH_JSON, false).toBool(); }
        inline bool siteConnected()         { return attrs.value(SOURCE_SITE_AUTH_JSON, false).toBool(); }

        virtual bool restoreUserConnection() { return false; }
        virtual bool connectUserApi() { return false; }
        virtual bool connectUserSite() { return false; }

        virtual void clearAdditionals() {}
        virtual void saveAdditionals(QJsonObject & /*obj*/) {}
        virtual void loadAdditionals(QJsonObject & /*obj*/) {}

        virtual QToolButton * initButton(QWidget * /*parent*/ = 0) { return button; }
    public slots:
        bool connectUser(const ConnectionType & conType = connection_new);
        void disconnectUser();

    protected:
        QToolButton * button;

        QVariantHash attrs;
        QString error;
    };
}


#endif // ISOURCE_AUTH_PERM
