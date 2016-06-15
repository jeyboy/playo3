#include "isource.h"

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core;

bool ISource::isPermitted(const PermitFlags & perm_flag) {
    SourceFlags api_flag, site_flag;

    switch(perm_flag) {
        case pf_search: {
            api_flag = sf_api_search_auth_only;
            site_flag = sf_site_search_auth_only;
        }
        case pf_media_content: {
            api_flag = sf_api_media_content_auth_only;
            site_flag = sf_site_media_content_auth_only;
        }
        case pf_user_content: {
            api_flag = sf_api_user_content_auth_only;
            site_flag = sf_site_user_content_auth_only;
        }
        default: return true;
    }

    SourceFlags flags = defaultFlags();
    bool api_flag_permit = HAS_FLAG(flags, api_flag);
    bool site_flag_permit = HAS_FLAG(flags, site_flag);

    return !api_flag_permit || api_flag_permit == apiConnected() ||
           !site_flag_permit || site_flag_permit == siteConnected();
}

bool ISource::connectUser(const ConnectionType & conType) {
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
void ISource::disconnectUser() {
    attrs.remove(SOURCE_API_AUTH_JSON); attrs.remove(SOURCE_SITE_AUTH_JSON);
    attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
    attrs.remove(SOURCE_API_EXPIRED_AT_JSON); attrs.remove(SOURCE_API_TOKEN_JSON);
    attrs.remove(SOURCE_API_USER_ID_JSON); attrs.remove(SOURCE_SITE_TOKEN_JSON);
    attrs.remove(SOURCE_SITE_EXPIRED_AT_JSON);

//            Web::Manager::removeCookies(name());

    clearAdditionals();
    initButton();
}

void ISource::openTab() {
    QString user_id = userID();

    if (connectUser(connection_new))
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(user_id)),
            Models::Params(siteType(), user_id), 0, true, true, 0, true
        );
}

void ISource::openRecomendations() {
    QString user_id = userID();

    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Rec for YOU"),
        Models::Params(siteType(), user_id, rec_user), 0, true, true
    );
}

void ISource::openRelationTab() {
    RelationsDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted)
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
            uidStr(dialog.getId())),
            Models::Params(siteType(), dialog.getId()), 0, true, true
        );
}
