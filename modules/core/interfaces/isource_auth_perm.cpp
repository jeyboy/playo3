#include "isource_auth_perm.h"

using namespace Core;

Permissions ISourceAuthPerm::permissions(const PermitRequest & req_perm) {
    SourceFlags api_flag, site_flag, site_prefer;

//    pr_search_media         = 1,
//    pr_search_objects       = 2, // user or group

//    pr_media_content        = 4,
//    pr_object_content       = 8, // user or group


    switch(req_perm) {
        case pr_search_media: {
            api_flag = sf_api_search_media_auth_only;
            site_flag = sf_site_search_media_auth_only;
            site_prefer = sf_prefer_site_search_media;
        break;}
        case pr_media_content: {
            api_flag = sf_api_media_content_auth_only;
            site_flag = sf_site_media_content_auth_only;
            site_prefer = sf_prefer_site_media_content;
        break;}

        case pr_search_objects: {
            api_flag = sf_api_search_objects_auth_only;
            site_flag = sf_site_search_objects_auth_only;
            site_prefer = sf_prefer_site_search_objects;
        break;}
        case pr_object_content: {
            api_flag = sf_api_object_content_auth_only;
            site_flag = sf_site_object_content_auth_only;
            site_prefer = sf_prefer_site_object_content;
        break;}
//        case pr_feed: {
//            api_flag = sf_api_feeds_auth_only;
//            site_flag = sf_site_feeds_auth_only;
//            site_prefer = sf_prefer_site_feeds;
//        break;}
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

    // if respondable to api and did not has limitation by auth or has limitation and we are connected
    if (HAS_FLAG(flags, sf_auth_api_has) && (!api_flag_permit || api_flag_permit == apiConnected()))
        res = perm_api;

    if (HAS_FLAG(flags, sf_auth_site_has) && (!site_flag_permit || site_flag_permit == siteConnected())) {
        if (!res || (res > 0 && HAS_FLAG(flags, site_prefer)))
            res = perm_site;
    }

    return res;
}

bool ISourceAuthPerm::connectUser() {
    bool res = true;

    button -> setIcon(QIcon(QStringLiteral(":/items/process")));
    button -> setEnabled(false);

    if (!apiConnected() && hasApiConnection()) {
        res &= connectUserApi();
        if (res) attrs[SOURCE_API_AUTH_JSON] = true;
    }

    if (res && !siteConnected() && hasSiteConnection()) {
        res &= requireOfflineCredentials() == takeOfflineCredentials();

        res &= connectUserSite();
        if (res)
            attrs[SOURCE_SITE_AUTH_JSON] = true;
    }

    if (siteConnected()) // take / refresh online attrs
        res &= requireOnlineCredentials() == takeOnlineCredentials();

    initButton();

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
