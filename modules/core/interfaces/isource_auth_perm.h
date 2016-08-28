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

namespace Core {
    class ISourceAuthPerm : public QObject {
        Q_OBJECT
    protected:
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

        QMap<SourceFlags, SourcePerms> flags;
        QVariantHash attrs;
        QString error;
    public:
        ISourceAuthPerm() : button(0), menu(0) { }
        virtual ~ISourceAuthPerm() {}

        QString userID() {
            QString user_id = siteUserID();
            return user_id.isEmpty() ? user_id : apiUserID();
        }

        bool respondableTo(const SourceFlags & req_flags) { return IS_AUTH_EQ_FLAG(flags, req_flags, siteConnected(), apiConnected()); }
        SourcePerms permissions(const SourceFlags & req_flag) {
            SourcePerms perm = flags.value(req_flag);

            if (HAS_FLAG(perm, perm_prefer_site)) {
                site_perms:
                    if (HAS_FLAG(perm, perm_site)) return perm_site;
                    if (HAS_FLAG(perm, perm_site_auth_only) == siteConnected()) return perm_site;
            } else {
                if (HAS_FLAG(perm, perm_api)) return perm_api;
                if (HAS_FLAG(perm, perm_api_auth_only) == apiConnected()) return perm_api;

                goto site_perms;
            }

            return perm_none;
        }

        inline bool isPrimary()             { return flags.contains(sf_is_primary); }
        inline bool isSociable()            { return isUsersSociable() || isGroupsSociable(); }
        inline bool isUsersSociable()       { return flags.contains(sf_users_sociable); }
        inline bool isGroupsSociable()      { return flags.contains(sf_groups_sociable); }

        inline bool hasOfflineSociable()    { return IS_OFFLINE_FLAG(flags, sf_users_sociable) || IS_OFFLINE_FLAG(flags, sf_groups_sociable); }
        inline bool isShareable()           { return flags.contains(sf_is_content_shareable); }
        inline bool hasPacks()              {
            bool api_auth = apiConnected();
            bool site_auth = siteConnected();

            bool has_packs =
                    IS_AUTH_EQ_FLAG(flags, sf_new_tracks, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_popular_tracks, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_by_genre, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_by_mood, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_by_tag, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_by_artist, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_by_album, site_auth, api_auth) ||

                    IS_AUTH_EQ_FLAG(flags, sf_new_videos, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_popular_videos, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_videos_by_genre, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_videos_by_mood, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_videos_by_tag, site_auth, api_auth)
                    ;

            return has_packs;
        }
//        inline bool hasOfflinePacks()    {
//            SourceFlags flags = defaultFlags();
//            SourceFlagControls controls = defaultControls();
//            return (HAS_FLAG(flags, sf_site_has) && NOT_HAS_FLAG(controls, sfc_site_section_packs_auth)) ||
//                    (HAS_FLAG(flags, sf_api_has) && NOT_HAS_FLAG(controls, sfc_api_section_packs_auth));
//        }

        inline bool hasTrackRecomendations()    {
            bool api_auth = apiConnected();
            bool site_auth = siteConnected();

            return IS_AUTH_EQ_FLAG(flags, sf_tracks_recs_by_track_id, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_tracks_recs_by_tracks_ids, site_auth, api_auth);
        }
        inline bool hasVideoRecomendations()    {
            bool api_auth = apiConnected();
            bool site_auth = siteConnected();

            return IS_AUTH_EQ_FLAG(flags, sf_videos_recs_by_video_id, site_auth, api_auth) ||
                    IS_AUTH_EQ_FLAG(flags, sf_videos_recs_by_videos_ids, site_auth, api_auth);
        }
        inline bool hasTracksSetRecomendations(){ return respondableTo(sf_tracks_recs_by_set_id); }
        inline bool hasVideosSetRecomendations(){ return respondableTo(sf_videos_recs_by_set_id); }

        inline bool hasOwnerTracksRecomendations()     { return respondableTo(sf_owner_tracks_recs); }
        inline bool hasOnwerVideosRecomendations()     { return respondableTo(sf_owner_videos_recs); }

//        inline bool hasNewItemsBlock()          { return HAS_FLAG(defaultFlags(), sf_content_section_new); }
//        inline bool hasPopularItemsBlock()      { return HAS_FLAG(defaultFlags(), sf_content_section_popular); }
//        inline bool hasGenres()             { return HAS_FLAG(defaultFlags(), sf_genreable); }
//        inline bool hasTags()               { return HAS_FLAG(defaultFlags(), sf_taggable); }
//        inline bool hasMoods()              { return HAS_FLAG(defaultFlags(), sf_moodable); }


        inline bool hasApiConnection()          { return HAS_FLAG(flags.value(sf_endpoint), perm_api); }
        inline bool hasSiteConnection()         { return HAS_FLAG(flags.value(sf_endpoint), perm_site); }

        inline bool requireOfflineCredentials() { return HAS_FLAG(flags.value(sf_endpoint), perm_offline_credentials_req); }
        inline bool requireOnlineCredentials()  { return HAS_FLAG(flags.value(sf_endpoint), perm_online_credentials_req); }


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
