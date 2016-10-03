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
#define SOURCE_SITE_LOCALE QStringLiteral("_sl")

namespace Core {
    class ISourceAuthPerm : public QObject {
        Q_OBJECT
    protected:
        virtual QToolButton * initButton(QWidget * parent = 0) = 0;

        inline void setApiExpiration(const QString & expiration)        { attrs[SOURCE_API_EXPIRED_AT_JSON] = expiration; }
        inline QString apiExpiration() const                            { return attrs[SOURCE_API_EXPIRED_AT_JSON].toString(); }

        inline void setApiUserID(const QString & user_id)               { attrs[SOURCE_API_USER_ID_JSON] = user_id; }
        inline QString apiUserID() const                                { return attrs[SOURCE_API_USER_ID_JSON].toString(); }

        inline void setApiToken(const QString & token)                  { attrs[SOURCE_API_TOKEN_JSON] = token; }
        inline QString apiToken() const                                 { return attrs[SOURCE_API_TOKEN_JSON].toString(); }

        inline void setSiteUserID(const QString & user_id)              { attrs[SOURCE_SITE_USER_ID_JSON] = user_id; }
        inline QString siteUserID() const                               { return attrs[SOURCE_SITE_USER_ID_JSON].toString(); }

        inline void setSiteToken(const QString & token)                 { attrs[SOURCE_SITE_TOKEN_JSON] = token; }
        inline QString siteToken() const                                { return attrs[SOURCE_SITE_TOKEN_JSON].toString(); }


        inline void setSiteAdditionalToken(const QString & token)       { attrs[SOURCE_SITE_ADDITIONAL_TOKEN_JSON] = token; }
        inline QString siteAdditionalToken() const                      { return attrs[SOURCE_SITE_ADDITIONAL_TOKEN_JSON].toString(); }

        inline void setSiteHash(const QString & token)                  { attrs[SOURCE_SITE_HASH_JSON] = token; }
        inline QString siteHash() const                                 { return attrs[SOURCE_SITE_HASH_JSON].toString(); }

        inline void setSiteLocale(const QString & new_locale)           { attrs[SOURCE_SITE_LOCALE] = new_locale; }
        inline QString siteLocale(const QString & default_locale = QString()) const {
            return attrs.value(SOURCE_SITE_LOCALE, default_locale).toString();
        }

        virtual bool connectUserApi()                                   { return false; }
        virtual bool connectUserSite()                                  { return false; }

        virtual bool hasOfflineCredentials()                            { return false; }
        virtual bool takeOfflineCredentials()                           { return false; }
        virtual bool takeOnlineCredentials()                            { return false; }

        virtual void saveAdditionals(QJsonObject & /*obj*/)             {}
        virtual void loadAdditionals(QJsonObject & /*obj*/)             {}
        virtual void clearAdditionals()                                 {}

        QToolButton * button;
        QMenu * menu;

        QVariantHash attrs;
        QString error;

        QHash<SourceFlags, SourceFlags> flags;
    public:
        ISourceAuthPerm() : button(0), menu(0) {}
        virtual ~ISourceAuthPerm() {}

        QString userID(const SourceFlags & req_perm = sf_site) {
            return req_perm == sf_site && isSiteConnected() ? siteUserID() : apiUserID();
        }

        bool respondableTo(const SourceFlags & req_perm)                { return permissions(req_perm) > 0; }
        SourceFlags permissions(const SourceFlags & req_perm);

        inline bool isApiConnected()                                    { return attrs.value(SOURCE_API_AUTH_JSON, false).toBool(); }
        inline bool isSiteConnected()                                   { return attrs.value(SOURCE_SITE_AUTH_JSON, false).toBool(); }

        inline bool isConnectable()                                     { return hasApiConnection() || hasSiteConnection(); }
        inline bool isConnected()                                       { return isApiConnected() || isSiteConnected(); }

        inline bool isShareable()                                       { return HAS_FLAG(flags[sf_endpoint], sf_is_content_shareable); }
        inline bool isPrimary()                                         { return HAS_FLAG(flags[sf_endpoint], sf_is_primary); }
        inline bool isSociable()                                        { return HAS_FLAG(flags[sf_endpoint], sf_sociable); }

        inline bool hasApiConnection()                                  { return HAS_FLAG(flags[sf_endpoint], sf_api); }
        inline bool hasSiteConnection()                                 { return HAS_FLAG(flags[sf_endpoint], sf_site); }

        inline bool hasUsers()                                          { return HAS_FLAG(flags[sf_endpoint], sf_user); }
        inline bool hasGroups()                                         { return HAS_FLAG(flags[sf_endpoint], sf_group); }
        inline bool hasCompilations()                                   { return HAS_FLAG(flags[sf_endpoint], sf_compilation); }

        inline bool hasSimillarTracksByTrack()                          { return respondableTo(sf_audio_recs_by_audio); }
        inline bool hasSimillarTracksByUser()                           { return respondableTo(sf_audio_recs_by_user); }
        inline bool hasSimillarTracksByArtist()                         { return respondableTo(sf_audio_recs_by_artist); }
        inline bool hasSimillarTracksByTag()                            { return respondableTo(sf_audio_by_tag); }
        inline bool hasTracksFromSameLabel()                            { return respondableTo(sf_audio_by_label); }
        inline bool hasTracksFromSameArtist()                           { return respondableTo(sf_audio_by_artist); }
        inline bool hasAlbumsFromSameArtist()                           { return respondableTo(sf_album_by_artist); }


//        inline bool hasNewItemsBlock()      { return HAS_FLAG(defaultFlags(), sf_newable); }
//        inline bool hasPopularItemsBlock()  { return HAS_FLAG(defaultFlags(), sf_populable); }
//        inline bool hasGenres()             { return HAS_FLAG(defaultFlags(), sf_genreable); }
//        inline bool hasTags()               { return HAS_FLAG(defaultFlags(), sf_taggable); }
//        inline bool hasMoods()              { return HAS_FLAG(defaultFlags(), sf_moodable); }

        inline bool requireOfflineCredentials()                         { return HAS_FLAG(flags[sf_endpoint], sf_offline_credentials_req); }
        inline bool requireOnlineCredentials()                          { return HAS_FLAG(flags[sf_endpoint], sf_online_credentials_req); }


        inline bool respondableToSocial()                               { return respondableTo(sf_user_sociable) || respondableTo(sf_group_sociable); }
        inline bool respondableToCompilations()                         { return respondableTo(sf_compilation); }

    public slots:
        bool connectUser();
        void disconnectUser();
    };
}


#endif // ISOURCE_AUTH_PERM
