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

    enum SourceFlags : quint64 {
        sf_none = 0,

        sf_auth_api_has                     = 1,
        sf_auth_site_has                    = (quint64(1)) << 1,


        sf_items_serachable                 = (quint64(1)) << 2,
        sf_sets_serachable                  = (quint64(1)) << 3,
        sf_users_serachable                 = (quint64(1)) << 4,
        sf_groups_serachable                = (quint64(1)) << 5,

        sf_searchable =
            sf_items_serachable | sf_sets_serachable |
            sf_users_serachable | sf_groups_serachable,

        sf_sociable_users                   = (quint64(1)) << 6, // user content
        sf_sociable_groups                  = (quint64(1)) << 7, // user content
        sf_taggable                         = (quint64(1)) << 8,
        sf_shareable                        = (quint64(1)) << 9, // site media content
        sf_charteable                       = (quint64(1)) << 10,
        sf_recomendable                     = (quint64(1)) << 11,
        sf_newable                          = (quint64(1)) << 12,
        sf_populable                        = (quint64(1)) << 13,
        sf_genreable                        = (quint64(1)) << 14,
//        sf_streamable                       = (quint64(1)) << 15,

        sf_content_audio_has                = (quint64(1)) << 11,
        sf_content_video_has                = (quint64(1)) << 12,
        sf_content_photo_has                = (quint64(1)) << 13,
        sf_content_news_has                 = (quint64(1)) << 14,
        sf_content_radio_has                = (quint64(1)) << 15,

        /////////////////////////////

        sf_site_items_search_auth_only      = (quint64(1)) << 16,
        sf_prefer_site_items_search         = (quint64(1)) << 17,
        sf_api_items_search_auth_only       = (quint64(1)) << 18,

        sf_site_sets_search_auth_only       = (quint64(1)) << 19,
        sf_prefer_site_sets_search          = (quint64(1)) << 20,
        sf_api_sets_search_auth_only        = (quint64(1)) << 21,

        sf_site_users_search_auth_only      = (quint64(1)) << 22,
        sf_prefer_site_users_search         = (quint64(1)) << 23,
        sf_api_users_search_auth_only       = (quint64(1)) << 24,

        sf_site_groups_search_auth_only     = (quint64(1)) << 25,
        sf_prefer_site_groups_search        = (quint64(1)) << 26,
        sf_api_groups_search_auth_only      = (quint64(1)) << 27,


        sf_site_media_content_auth_only     = (quint64(1)) << 28,
        sf_prefer_site_media_content        = (quint64(1)) << 29,
        sf_api_media_content_auth_only      = (quint64(1)) << 30,

        sf_site_user_content_auth_only      = (quint64(1)) << 31,
        sf_prefer_site_user_content         = (quint64(1)) << 32,
        sf_api_user_content_auth_only       = (quint64(1)) << 33,

        sf_site_feeds_auth_only             = (quint64(1)) << 34,
        sf_prefer_site_feeds                = (quint64(1)) << 35,
        sf_api_feeds_auth_only              = (quint64(1)) << 36,

        sf_site_shareable_auth_only         = (quint64(1)) << 37,
        sf_prefer_site_shareable            = (quint64(1)) << 38,
        sf_api_shareable_auth_only          = (quint64(1)) << 39,

        sf_site_charteable_auth_only        = (quint64(1)) << 40,
        sf_prefer_site_charteable           = (quint64(1)) << 41,
        sf_api_charteable_auth_only         = (quint64(1)) << 42,

        sf_site_auth_mandatory =
            sf_site_items_search_auth_only | sf_site_sets_search_auth_only | sf_site_users_search_auth_only |
            sf_site_groups_search_auth_only | sf_site_media_content_auth_only | sf_site_user_content_auth_only |
            sf_site_feeds_auth_only | sf_site_shareable_auth_only,
        sf_api_auth_mandatory =
            sf_api_items_search_auth_only | sf_api_sets_search_auth_only | sf_api_users_search_auth_only |
            sf_api_groups_search_auth_only | sf_api_media_content_auth_only | sf_api_user_content_auth_only |
            sf_api_feeds_auth_only | sf_api_shareable_auth_only,

        sf_auth_mandatory = sf_site_auth_mandatory | sf_api_auth_mandatory
    };

    enum Permissions : int {
        perm_none = 0,
        perm_site = 1,
        perm_api = 2
    };

    enum PermitRequest {
        pr_none                 = 0,

        pr_search_items         = 1,
        pr_search_sets          = 2,
        pr_search_users         = 4,
        pr_search_groups        = 8,

        pr_media_content        = 16,
        pr_user_content         = 32,
        pr_feed                 = 64,
        pr_shareable            = 128,
        pr_chart                = 256,
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

        inline bool isSociable()            { return HAS_FLAG(defaultFlags(), sf_sociable_users) || HAS_FLAG(defaultFlags(), sf_sociable_groups); }
        inline bool isShareable()            { return HAS_FLAG(defaultFlags(), sf_shareable); }

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
