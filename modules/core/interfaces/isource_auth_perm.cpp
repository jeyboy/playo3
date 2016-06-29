#include "isource_auth_perm.h"

using namespace Core;

Permissions ISourceAuthPerm::permissions(const PermitRequest & req_perm) {
    SourceFlags api_flag, site_flag, site_prefer;

    switch(req_perm) {
        case pr_search: {
            api_flag = sf_api_search_auth_only;
            site_flag = sf_site_search_auth_only;
            site_prefer = sf_prefer_site_search;
        break;}
        case pr_media_content: {
            api_flag = sf_api_media_content_auth_only;
            site_flag = sf_site_media_content_auth_only;
            site_prefer = sf_prefer_site_media_content;
        break;}
        case pr_user_content: {
            api_flag = sf_api_user_content_auth_only;
            site_flag = sf_site_user_content_auth_only;
            site_prefer = sf_prefer_site_user_content;
        break;}
        case pr_feed: {
            api_flag = sf_api_feeds_auth_only;
            site_flag = sf_site_feeds_auth_only;
            site_prefer = sf_prefer_site_feeds;
        break;}
        case pr_recommendations: {
            api_flag = sf_api_recomendations_auth_only;
            site_flag = sf_site_recomendations_auth_only;
            site_prefer = sf_prefer_site_recomendations;
        break;}
        default: return perm_none;
    }

    SourceFlags flags = defaultFlags();
    bool api_flag_permit = HAS_FLAG(flags, api_flag);
    bool site_flag_permit = HAS_FLAG(flags, site_flag);

    Permissions res = perm_none;

    if (!api_flag_permit || api_flag_permit == apiConnected())
        res = perm_api;

    if (!site_flag_permit || site_flag_permit == siteConnected()) {
        if (!res || (res > 0 && HAS_FLAG(flags, site_prefer)))
            res = perm_site;
    }

    return res;
}

bool ISourceAuthPerm::connectUser(const ConnectionType & conType) {
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
void ISourceAuthPerm::disconnectUser() {
    attrs.remove(SOURCE_API_AUTH_JSON); attrs.remove(SOURCE_SITE_AUTH_JSON);
    attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
    attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
    attrs.remove(SOURCE_API_USER_ID_JSON); attrs.remove(SOURCE_SITE_TOKEN_JSON);
    attrs.remove(SOURCE_SITE_EXPIRED_AT_JSON);

    clearAdditionals();
    initButton();
}
