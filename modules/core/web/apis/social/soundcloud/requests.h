#ifndef SOUNDCLOUD_REQUESTS
#define SOUNDCLOUD_REQUESTS

#include "api/requests.h"
#include "site/requests.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Requests : public ISource, public Sociable, public Api::Requests, public Site::Requests {
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | sf_auth_site_has | sf_site_offline_credentials_req |
                        sf_content_audio_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable |
                        sf_groups_serachable | sf_tags_serachable | sf_genres_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_charteable |
                        sf_recomendable_by_item | sf_newable | sf_taggable | sf_genreable |
                        sf_api_auth_mandatory |
                        sf_site_recomendations_auth_only | sf_site_user_content_auth_only
                    );
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api_def: return url_api_base % predicate % val_default_format;
                        case qst_site_def: return url_api_base % predicate;
                        case qst_site_alt1: return url_api_base2 % predicate;
                    }
                }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    return response.value(tkn_response).toArray().size() < arg -> per_request_limit;
                }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & json, int & code, QString & message) {
                    QJsonObject stat_obj = json.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
                    message = stat_obj.value(tkn_error_message).toString();
                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                }

                void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr);
                void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr);

                bool connectUserApi() {
                    QString newToken, userID
                    bool res = Api::Requests::connectUser(newToken, userID);

                    if (res) {
                        setApiToken(newToken);
                        setApiUserID(userID);
                    }

                    return res;
                }

                bool connectUserSite() { return Site::Requests::connectUser(); } // not realized yet

                bool hasOfflineCredentials()     { return !siteToken().isEmpty(); }
                bool takeOfflineCredentials();

                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    takeOfflineCredentials();
                }
                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

            public:
                inline virtual ~Requests() {}

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api_def) {
                    switch(stype) {
                        case qst_json_def:
                            return QUrlQuery(tkn_client_id % val_id_tkn);
                        case qst_html_def:
                        case qst_html_alt1:
                            return QUrlQuery(tkn_client_id % siteToken() % QStringLiteral("app_version=") % siteHash());
                        default: return QUrlQuery();
                    }
                }

//                inline QHash<QString, QString> customsList() { return QHash<QString, QString>(); }
//                inline QJsonValue procCustom(const QString & /*custom_params*/) { return QJsonArray(); }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUESTS
