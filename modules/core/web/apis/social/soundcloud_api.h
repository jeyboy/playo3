#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/apis/service/recaptcha.h"

#include "soundcloud/requests.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Api : public Requests, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { setSearchLimitations(true, true, true, true); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_sc; }


                QJsonObject objectInfo(const QString & uid) {
                    QJsonObject res;

                    if (uid[0] == '-')
                        getGroupInfo(uid.mid(1), res);
                    else
                        getUserInfo(uid, res);

                    return res;
                }
                inline void objectInfoAsync(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::objectInfo, uid, func);
                }

                QString refresh(const QString & audio_uid, const DataMediaType & /*itemMediaType*/) {
                    return idToPathSite(audio_uid);
                }

            protected:
                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return playlistByPredicate(predicate, limits.items_limit, limits.start_offset);
                }

                QJsonValue searchProc(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(limitations.predicate, limitations.genre, limitations.by_popularity()),
                        call_type_json,
                        rules(limitations.start_offset, limitations.items_limit),
                        0,
                        proc_json_patch
                    );
                }             
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
