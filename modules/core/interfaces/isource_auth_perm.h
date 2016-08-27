#ifndef ISOURCE_AUTH_PERM
#define ISOURCE_AUTH_PERM

#include <qstring.h>
#include <qtoolbutton.h>
#include <qmenu.h>

#include "isource_perm_flags.h"
#include "modules/core/web/utils/web_manager.h"

#define SOURCE_API_AUTH_JSON QStringLiteral("api_auth")
#define SOURCE_SITE_AUTH_JSON QStringLiteral("site_auth")

#define SOURCE_API_EXPIRED_AT_JSON QStringLiteral("_aa")
#define SOURCE_API_TOKEN_JSON QStringLiteral("_at")
#define SOURCE_API_USER_ID_JSON QStringLiteral("_au")

#define SOURCE_SITE_USER_ID_JSON QStringLiteral("_su")
#define SOURCE_SITE_HASH_JSON QStringLiteral("_sh")
#define SOURCE_SITE_TOKEN_JSON QStringLiteral("_st")
#define SOURCE_SITE_ADDITIONAL_TOKEN_JSON QStringLiteral("_sat")
#define SOURCE_SITE_EXPIRED_AT_JSON QStringLiteral("_sa")


#define NOT_HAS_FLAG(flags, flag) (flags & flag) != flag
#define HAS_FLAG(flags, flag) (flags & flag) == flag
#define APPEND_FLAG(flags, flag) flags = (SourceFlags)(flags | flag)
#define REJECT_FLAG(flags, flag) flags &= (~(flag))

namespace Core {
    class ISourceAuthPerm : public QObject {
        Q_OBJECT
    protected:
        QMap<SourceFlags, bool> flags;

        virtual QToolButton * initButton(QWidget * parent = 0) = 0;

        inline void setApiExpiration(const QString & expiration)                { attrs[SOURCE_API_EXPIRED_AT_JSON] = expiration; }
        inline QString apiExpiration() const                                    { return attrs[SOURCE_API_EXPIRED_AT_JSON].toString(); }
        inline void setApiUserID(const QString & user_id)                       { attrs[SOURCE_API_USER_ID_JSON] = user_id; }
        inline QString apiUserID() const                                        { return attrs[SOURCE_API_USER_ID_JSON].toString(); }

        inline void setApiToken(const QString & token)                          { attrs[SOURCE_API_TOKEN_JSON] = token; }
        inline QString apiToken() const                                         { return attrs[SOURCE_API_TOKEN_JSON].toString(); }

        inline void setSiteUserID(const QString & user_id)                      { attrs[SOURCE_SITE_USER_ID_JSON] = user_id; }
        inline QString siteUserID() const                                       { return attrs[SOURCE_SITE_USER_ID_JSON].toString(); }

        inline void setSiteToken(const QString & token)                         { attrs[SOURCE_SITE_TOKEN_JSON] = token; }
        inline QString siteToken() const                                        { return attrs[SOURCE_SITE_TOKEN_JSON].toString(); }


        inline void setSiteAdditionalToken(const QString & token)               { attrs[SOURCE_SITE_ADDITIONAL_TOKEN_JSON] = token; }
        inline QString siteAdditionalToken() const                              { return attrs[SOURCE_SITE_ADDITIONAL_TOKEN_JSON].toString(); }

        inline void setSiteHash(const QString & token)                          { attrs[SOURCE_SITE_HASH_JSON] = token; }
        inline QString siteHash() const                                         { return attrs[SOURCE_SITE_HASH_JSON].toString(); }

        virtual bool connectUserApi()                                           { return false; }
        virtual bool connectUserSite()                                          { return false; }

        virtual bool hasOfflineCredentials()                                    { return false; }
        virtual bool takeOfflineCredentials()                                   { return false; }
        virtual bool takeOnlineCredentials()                                    { return false; }

        virtual void saveAdditionals(QJsonObject & /*obj*/)                     {}
        virtual void loadAdditionals(QJsonObject & /*obj*/)                     {}
        virtual void clearAdditionals()                                         {}

        QToolButton * button;
        QMenu * menu;

        QVariantHash attrs;
        QString error;
    public:
        ISourceAuthPerm() : button(0), menu(0) {}
        virtual ~ISourceAuthPerm() {}

        QString userID(const PermitRequest & req_perm = pr_object_content) {
            Permissions perm = permissions(req_perm);
            return perm == perm_site ? siteUserID() : apiUserID();
        }

        bool respondableTo(const PermitRequest & req_perm = pr_search_media) { return permissions(req_perm) > 0; }
        Permissions permissions(const PermitRequest & req_perm = pr_search_media);

        inline bool isPrimary()             { return HAS_FLAG(defaultFlags(), sf_is_primary); }
        inline bool isSociable()            {
            SourceFlags flags = defaultFlags();
            return HAS_FLAG(flags, sf_users_sociable) || HAS_FLAG(flags, sf_groups_sociable);
        }

        inline bool isUsersSociable()       { return HAS_FLAG(defaultFlags(), sf_users_sociable); }
        inline bool isGroupsSociable()       { return HAS_FLAG(defaultFlags(), sf_groups_sociable); }

        inline bool hasOfflineSociable()    {
            SourceFlags flags = defaultFlags();
            SourceFlagControls controls = defaultControls();
            return (HAS_FLAG(flags, sf_site_has) && NOT_HAS_FLAG(controls, sfc_site_objects_auth)) ||
                    (HAS_FLAG(flags, sf_api_has) && NOT_HAS_FLAG(controls, sfc_api_objects_auth));
        }
        inline bool isShareable()           { return HAS_FLAG(defaultFlags(), sf_content_shareable); }
        inline bool hasPacks()              { return HAS_FLAG(defaultFlags(), sf_content_section_packs); }
        inline bool hasOfflinePacks()    {
            SourceFlags flags = defaultFlags();
            SourceFlagControls controls = defaultControls();
            return (HAS_FLAG(flags, sf_site_has) && NOT_HAS_FLAG(controls, sfc_site_section_packs_auth)) ||
                    (HAS_FLAG(flags, sf_api_has) && NOT_HAS_FLAG(controls, sfc_api_section_packs_auth));
        }

        inline bool hasTrackRecomendations()    { return HAS_FLAG(defaultFlags(), sf_content_track_recomendable); }
        inline bool hasVideoRecomendations()    { return HAS_FLAG(defaultFlags(), sf_content_videos_recomendable); }
        inline bool hasSetRecomendations()      { return HAS_FLAG(defaultFlags(), sf_content_set_recomendable); }
        inline bool hasUserRecomendations()     { return HAS_FLAG(defaultFlags(), sf_user_recomendable); }

        inline bool hasNewItemsBlock()          { return HAS_FLAG(defaultFlags(), sf_content_section_new); }
        inline bool hasPopularItemsBlock()      { return HAS_FLAG(defaultFlags(), sf_content_section_popular); }
//        inline bool hasGenres()             { return HAS_FLAG(defaultFlags(), sf_genreable); }
//        inline bool hasTags()               { return HAS_FLAG(defaultFlags(), sf_taggable); }
//        inline bool hasMoods()              { return HAS_FLAG(defaultFlags(), sf_moodable); }


        inline bool hasApiConnection()          { return HAS_FLAG(defaultFlags(), sf_api_has); }
        inline bool hasSiteConnection()         { return HAS_FLAG(defaultFlags(), sf_site_has); }

        inline bool requireOfflineCredentials() { return HAS_FLAG(defaultFlags(), sf_site_offline_credentials_req); }
        inline bool requireOnlineCredentials()  { return HAS_FLAG(defaultFlags(), sf_site_online_credentials_req); }


        inline bool isConnectable()             { return hasApiConnection() || hasSiteConnection(); }
        inline bool isConnected()               { return apiConnected() || siteConnected(); }
        inline bool apiConnected()              { return attrs.value(SOURCE_API_AUTH_JSON, false).toBool(); }
        inline bool siteConnected()             { return attrs.value(SOURCE_SITE_AUTH_JSON, false).toBool(); }
    public slots:
        bool connectUser();
        void disconnectUser();
    };
}


#endif // ISOURCE_AUTH_PERM
