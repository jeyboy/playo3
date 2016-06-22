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

#define SOURCE_SITE_HASH_JSON QStringLiteral("_sh")
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

        sf_site_auth_mandatory = sf_site_search_auth_only | sf_site_media_content_auth_only | sf_site_user_content_auth_only,
        sf_api_auth_mandatory = sf_api_search_auth_only | sf_api_media_content_auth_only | sf_api_user_content_auth_only,

        sf_auth_mandatory = sf_site_auth_mandatory | sf_api_auth_mandatory
    };

    enum PermitFlags {
        pf_none = 0,
        pf_search = 1,
        pf_media_content = 2,
        pf_user_content = 4
    };

    class ISource : public QObject {
        Q_OBJECT
    public:
        ISource() : button(0) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }

        virtual inline SourceFlags defaultFlags() { return sf_none; }

        inline void setApiExpiration(const QString & expiration) { attrs[SOURCE_API_EXPIRED_AT_JSON] = expiration; }
        inline QString apiExpiration() const { return attrs[SOURCE_API_EXPIRED_AT_JSON].toString(); }

        inline void setUserID(const QString & user_id) { attrs[SOURCE_API_USER_ID_JSON] = user_id; }
        inline QString userID() const { return attrs[SOURCE_API_USER_ID_JSON].toString(); }
        inline void setApiToken(const QString & token) { attrs[SOURCE_API_TOKEN_JSON] = token; }
        inline QString apiToken() const { return attrs[SOURCE_API_TOKEN_JSON].toString(); }
        inline void setSiteToken(const QString & token) { attrs[SOURCE_SITE_TOKEN_JSON] = token; }
        inline QString siteToken() const { return attrs[SOURCE_SITE_TOKEN_JSON].toString(); }
        inline void setSiteHash(const QString & token) { attrs[SOURCE_SITE_HASH_JSON] = token; }
        inline QString siteHash() const { return attrs[SOURCE_SITE_HASH_JSON].toString(); }

        bool isPermitted(const PermitFlags & perm_flag = pf_search);
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
        virtual inline QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
            if (!isRefreshable() || refresh_page.isEmpty()) return QString();
            return refreshProc(takeRefreshPage(refresh_page), itemMediaType);
        }
        virtual inline QJsonArray loadSet(const QVariantMap & /*attrs*/) { return QJsonArray(); }

        virtual QToolButton * initButton(QWidget * /*parent*/ = 0) { return button; }

    public slots:
        bool connectUser(const ConnectionType & conType = connection_new);
        void disconnectUser();

        void openTab();
        void openRecomendations();
        void openRelationTab();
    protected:
        QToolButton * button;
        QVariantHash attrs;

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response, const DataMediaType & /*itemMediaType*/) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
