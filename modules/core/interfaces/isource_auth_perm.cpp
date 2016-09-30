#include "isource_auth_perm.h"

using namespace Core;


SourceFlags ISourceAuthPerm::permissions(const SourceFlags & req_perm) {
    SourceFlags perms = flags.value(req_perm, sf_none);

    if (perms > 0) {
        if (HAS_FLAG(perms, sf_site_auth_only)) {
            if (isSiteConnected()) return sf_site;
        }

        if (HAS_FLAG(perms, sf_api_auth_only)) {
            if (isApiConnected()) return sf_api;
        }

        if (HAS_FLAG(perms, sf_site))
            return sf_site;

        if (HAS_FLAG(perms, sf_api))
            return sf_api;
    }

    return perms;
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

    if (siteConnected()) { // take / refresh online attrs
        res &= requireOnlineCredentials() == takeOnlineCredentials();
        if (!res) qCritical() << "Site req on online credentials";
    }

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
