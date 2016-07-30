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
#define SOURCE_SITE_EXPIRED_AT_JSON QStringLiteral("_sa")


#define NOT_HAS_FLAG(flags, flag) (flags & flag) != flag
#define HAS_FLAG(flags, flag) (flags & flag) == flag
#define APPEND_FLAG(flags, flag) flags = (SourceFlags)(flags | flag)
#define REJECT_FLAG(flags, flag) flags &= (~(flag))

namespace Core {
    enum SourceFlags : quint64 {
        sf_none = 0,

        sf_primary_source                   = 1,

        sf_auth_api_has                     = (quint64(1)) << 1,
        sf_auth_site_has                    = (quint64(1)) << 2,
        sf_site_offline_credentials_req     = (quint64(1)) << 3,
        sf_site_online_credentials_req      = (quint64(1)) << 4,

        ///////////////////////////// info flags ///////////////////////

        sf_items_serachable                 = (quint64(1)) << 5, // search by item names
        sf_sets_serachable                  = (quint64(1)) << 6, // search by sets

        sf_users_serachable                 = (quint64(1)) << 7, // search by users
        sf_groups_serachable                = (quint64(1)) << 8, // search by groups

        sf_by_abc_serachable                = (quint64(1)) << 9,
        sf_by_lyrics_serachable             = (quint64(1)) << 10,
        sf_by_tags_serachable               = (quint64(1)) << 11,
        sf_by_genres_serachable             = (quint64(1)) << 12,
        sf_by_moods_serachable              = (quint64(1)) << 13,

        sf_sociable_users                   = (quint64(1)) << 14, // user content
        sf_sociable_groups                  = (quint64(1)) << 15, // user content
        sf_shareable                        = (quint64(1)) << 16, // restore item by id
        sf_packable                         = (quint64(1)) << 17, // has charts / tops / etc
        sf_recomendable_by_item             = (quint64(1)) << 18, // give recomendations by item
        sf_recomendable_by_user             = (quint64(1)) << 19, // give recomendations by user
        sf_newable                          = (quint64(1)) << 20, // has section with new items
        sf_populable                        = (quint64(1)) << 21, // has section with popular items
        sf_taggable                         = (quint64(1)) << 22, // has grouping by tags
        sf_genreable                        = (quint64(1)) << 23, // has grouping by genres
        sf_moodable                         = (quint64(1)) << 24, // has grouping by moods
//        sf_streamable                       = (quint64(1)) << 25,
//                                            = (quint64(1)) << 26, // reserved


        sf_content_lyrics_has               = (quint64(1)) << 27,
        sf_content_audio_has                = (quint64(1)) << 28,
        sf_content_video_has                = (quint64(1)) << 29,
        sf_content_photo_has                = (quint64(1)) << 30,
        sf_content_news_has                 = (quint64(1)) << 31,
        sf_content_radio_has                = (quint64(1)) << 32,

        ///////////////////////////// control flags ///////////////////////

        sf_site_search_media_auth_only      = (quint64(1)) << 33,
        sf_api_search_media_auth_only       = (quint64(1)) << 34,
        sf_prefer_site_search_media         = (quint64(1)) << 35,

        sf_site_search_objects_auth_only    = (quint64(1)) << 36,
        sf_api_search_objects_auth_only     = (quint64(1)) << 37,
        sf_prefer_site_search_objects       = (quint64(1)) << 38,

        sf_site_media_content_auth_only     = (quint64(1)) << 39,
        sf_prefer_site_media_content        = (quint64(1)) << 40,
        sf_api_media_content_auth_only      = (quint64(1)) << 41,

        sf_site_object_content_auth_only    = (quint64(1)) << 42, // user or group
        sf_prefer_site_object_content       = (quint64(1)) << 43,
        sf_api_object_content_auth_only     = (quint64(1)) << 44,

        sf_site_packs_auth_only             = (quint64(1)) << 45,
        sf_prefer_site_packs                = (quint64(1)) << 46,
        sf_api_packs_auth_only              = (quint64(1)) << 47,

//        sf_site_sociable_auth_only          = (quint64(1)) << 48,
//        sf_prefer_site_sociable             = (quint64(1)) << 49,
//        sf_api_sociable_auth_only           = (quint64(1)) << 50,

        sf_site_recomendations_auth_only    = (quint64(1)) << 51,
        sf_prefer_site_recomendations       = (quint64(1)) << 52,
        sf_api_recomendations_auth_only     = (quint64(1)) << 53,

//        sf_site_feeds_auth_only             = (quint64(1)) << 54,
//        sf_prefer_site_feeds                = (quint64(1)) << 55,
//        sf_api_feeds_auth_only              = (quint64(1)) << 56,

        sf_site_auth_mandatory =
            sf_site_search_media_auth_only | sf_site_media_content_auth_only | sf_site_search_objects_auth_only |
            sf_site_object_content_auth_only | sf_site_packs_auth_only | /*sf_site_feeds_auth_only |*/
            /*sf_site_sociable_auth_only |*/ sf_site_recomendations_auth_only,
        sf_api_auth_mandatory =
            sf_api_search_media_auth_only | sf_api_media_content_auth_only | sf_api_search_objects_auth_only |
            sf_api_object_content_auth_only | sf_api_packs_auth_only | /*sf_api_feeds_auth_only |*/
            /*sf_api_sociable_auth_only |*/ sf_api_recomendations_auth_only,

        sf_auth_mandatory = sf_site_auth_mandatory | sf_api_auth_mandatory
    };

    class ISourceAuthPerm : public QObject {
        Q_OBJECT
    protected:
        virtual QToolButton * initButton(QWidget * parent = 0) = 0;

        inline void setApiExpiration(const QString & expiration)
                                            { attrs[SOURCE_API_EXPIRED_AT_JSON] = expiration; }
        inline QString apiExpiration() const
                                            { return attrs[SOURCE_API_EXPIRED_AT_JSON].toString(); }
        inline void setApiUserID(const QString & user_id)
                                            { attrs[SOURCE_API_USER_ID_JSON] = user_id; }
        inline QString apiUserID() const    { return attrs[SOURCE_API_USER_ID_JSON].toString(); }

        inline void setApiToken(const QString & token)
                                            { attrs[SOURCE_API_TOKEN_JSON] = token; }
        inline QString apiToken() const     { return attrs[SOURCE_API_TOKEN_JSON].toString(); }

        inline void setSiteUserID(const QString & user_id)
                                            { attrs[SOURCE_SITE_USER_ID_JSON] = user_id; }
        inline QString siteUserID() const   { return attrs[SOURCE_SITE_USER_ID_JSON].toString(); }

        inline void setSiteToken(const QString & token)
                                            { attrs[SOURCE_SITE_TOKEN_JSON] = token; }
        inline QString siteToken() const    { return attrs[SOURCE_SITE_TOKEN_JSON].toString(); }
        inline void setSiteHash(const QString & token)
                                            { attrs[SOURCE_SITE_HASH_JSON] = token; }
        inline QString siteHash() const     { return attrs[SOURCE_SITE_HASH_JSON].toString(); }

        virtual bool connectUserApi()       { return false; }
        virtual bool connectUserSite()      { return false; }

        virtual bool hasOfflineCredentials()     { return false; }
        virtual bool takeOfflineCredentials()    { return false; }
        virtual bool takeOnlineCredentials()     { return false; }

        virtual void saveAdditionals(QJsonObject & /*obj*/) {}
        virtual void loadAdditionals(QJsonObject & /*obj*/) {}
        virtual void clearAdditionals() {}

        QToolButton * button;
        QMenu * menu;

        QVariantHash attrs;
        QString error;
    public:
        ISourceAuthPerm() : button(0), menu(0) {}
        virtual ~ISourceAuthPerm() {}

        virtual inline SourceFlags defaultFlags() { return sf_none; }

        QString userID(const PermitRequest & req_perm = pr_object_content) {
            Permissions perm = permissions(req_perm);
            return perm == perm_site ? siteUserID() : apiUserID();
        }

        bool respondableTo(const PermitRequest & req_perm = pr_search_media) { return permissions(req_perm) > 0; }
        Permissions permissions(const PermitRequest & req_perm = pr_search_media);

        inline bool isPrimary()             { return HAS_FLAG(defaultFlags(), sf_primary_source); }
        inline bool isSociable()            {
            SourceFlags flags = defaultFlags();
            return HAS_FLAG(flags, sf_sociable_users) || HAS_FLAG(flags, sf_sociable_groups);
        }
        inline bool hasOfflineSociable()    {
            SourceFlags flags = defaultFlags();
            return (HAS_FLAG(flags, sf_auth_site_has) && NOT_HAS_FLAG(flags, sf_site_object_content_auth_only)) ||
                    (HAS_FLAG(flags, sf_auth_api_has) && NOT_HAS_FLAG(flags, sf_api_object_content_auth_only));
        }
        inline bool isShareable()           { return HAS_FLAG(defaultFlags(), sf_shareable); }

        inline bool hasPacks()              { return HAS_FLAG(defaultFlags(), sf_packable); }
        inline bool hasOfflinePacks()    {
            SourceFlags flags = defaultFlags();
            return (HAS_FLAG(flags, sf_auth_site_has) && NOT_HAS_FLAG(flags, sf_site_packs_auth_only)) ||
                    (HAS_FLAG(flags, sf_auth_api_has) && NOT_HAS_FLAG(flags, sf_api_packs_auth_only));
        }

        inline bool hasItemRecomendations() { return HAS_FLAG(defaultFlags(), sf_recomendable_by_item); }
        inline bool hasUserRecomendations() { return HAS_FLAG(defaultFlags(), sf_recomendable_by_user); }
        inline bool hasNewItemsBlock()      { return HAS_FLAG(defaultFlags(), sf_newable); }
        inline bool hasPopularItemsBlock()  { return HAS_FLAG(defaultFlags(), sf_populable); }
        inline bool hasGenres()             { return HAS_FLAG(defaultFlags(), sf_genreable); }
        inline bool hasTags()               { return HAS_FLAG(defaultFlags(), sf_taggable); }
        inline bool hasMoods()              { return HAS_FLAG(defaultFlags(), sf_moodable); }


        inline bool hasApiConnection()      { return HAS_FLAG(defaultFlags(), sf_auth_api_has); }
        inline bool hasSiteConnection()     { return HAS_FLAG(defaultFlags(), sf_auth_site_has); }

        inline bool requireOfflineCredentials()
                                            { return HAS_FLAG(defaultFlags(), sf_site_offline_credentials_req); }
        inline bool requireOnlineCredentials()
                                            { return HAS_FLAG(defaultFlags(), sf_site_online_credentials_req); }


        inline bool isConnectable()         { return hasApiConnection() || hasSiteConnection(); }
        inline bool isConnected()           { return apiConnected() || siteConnected(); }
        inline bool apiConnected()          { return attrs.value(SOURCE_API_AUTH_JSON, false).toBool(); }
        inline bool siteConnected()         { return attrs.value(SOURCE_SITE_AUTH_JSON, false).toBool(); }
    public slots:
        bool connectUser();
        void disconnectUser();
    };
}


#endif // ISOURCE_AUTH_PERM
