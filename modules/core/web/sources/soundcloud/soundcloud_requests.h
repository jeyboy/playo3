#ifndef SOUNDCLOUD_REQUESTS
#define SOUNDCLOUD_REQUESTS

#include "soundcloud_auth.h"
#include "soundcloud_group.h"
#include "soundcloud_playlist.h"
#include "soundcloud_set.h"
#include "soundcloud_track.h"
#include "soundcloud_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Requests : public Sociable, public Auth, public Group,
                     public Playlist, public Set, public Track, public User
            {
            protected:
                Requests() { setSociableLimitations(true, true, true, true); }
                inline virtual ~Requests() {}

                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | /*sf_auth_site_has |*/ sf_site_offline_credentials_req |
                        sf_content_audio_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable |
                        sf_groups_serachable | sf_by_tags_serachable | sf_by_genres_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_packable |
                        sf_recomendable_by_item | sf_newable | sf_taggable | sf_genreable |
                        sf_api_auth_mandatory |
                        sf_site_recomendations_auth_only | sf_site_object_content_auth_only
                    );
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api_def: return url_api_base % predicate % val_default_format;
                        case qst_site_def: return url_api_base % predicate;
                        case qst_site_alt1: return url_api_base2 % predicate;
                        default: return QString();
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
                    QString new_token, user_id;
                    bool res = connectApi(new_token, user_id, error);

                    if (res) {
                        setApiToken(new_token);
                        setApiUserID(user_id);
                    }

                    return res;
                }

                bool connectUserSite() { return connectSite(); } // not realized yet

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
                    takeOfflineCredentials();
                }

            public:
                // standart step for all search requests is 10

                // mixed search with autocomplete - users / groups/ tracks
                //'https://api-v2.soundcloud.com/search/autocomplete?q=s&queries_limit=0&results_limit=10&limit=10&offset=0&linked_partitioning=1'

                // mixed search - users / groups/ tracks
                // curl 'https://api-v2.soundcloud.com/search?q=dubstep&sc_a_id=184de48a-5c8d-4ef7-908a-b4602d2a667d&facet=model&user_id=301086-912545-767108-258358&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1466770454' -H 'Host: api-v2.soundcloud.com' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache'

                // users streams // wtf?
                // curl 'https://api-v2.soundcloud.com/stream/users/53128020?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=20&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'


                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api_def) {
                    switch(stype) {
                        case qst_api_def:
                            return QUrlQuery(tkn_client_id % val_id_tkn);
                        case qst_site_def:
                        case qst_site_alt1:
                            return QUrlQuery(tkn_client_id % siteToken() % QStringLiteral("&app_version=") % siteHash());
                        default: return QUrlQuery();
                    }
                }

                inline QMap<QString, QString> setsList() {
                    QMap<QString, QString> res;
                    QMap<QString, QString> opts = siteOptions();

                    QString new_hot_title = setTypeToStr(set_new_hot);
                    QString top_50_title = setTypeToStr(set_top_50);
                    QString popular_title = setTypeToStr(set_popular);

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
                        res.insert(new_hot_title % opt.key(), PPACK(QString::number(set_new_hot), opt.value()));
                        res.insert(top_50_title % opt.key(), PPACK(QString::number(set_top_50), opt.value()));

                        QStringList parts = opt.key().split('&', QString::SkipEmptyParts);

                        if (parts.size() == 1)
                            res.insert(popular_title % opt.key(), PPACK(QString::number(set_popular), opt.value()));
                        else {
                            res.insert(popular_title % parts.first(), PPACK(QString::number(set_popular), parts.first()));
                            res.insert(popular_title % parts.last(), PPACK(QString::number(set_popular), parts.last()));
                        }
                    }

                    res.insert(popular_title % PACKAGE_REPLACE_FRAGMENT, PPACK(QString::number(set_popular), '%'));

                    return res;
                }
                inline QJsonValue takeSet(const QString & set_params) {
                    QStringList params = set_params.split('|', QString::SkipEmptyParts);
                    SetType set_type = (SetType)params.first().toInt();
                    QString genre = params.last();

                    return setByType(set_type, genre);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUESTS
