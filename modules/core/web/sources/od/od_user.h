#ifndef OD_USER
#define OD_USER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class User : public Base {
            public:
                QJsonValue userInfo() {
                    return sRequest(
                        audioUrlStr(path_audio_init),
                        call_type_json
                    );
                }

                QJsonValue userMedia(const QString & user_id) {
                    return sRequest(
                        audioUrlStr(
                            tkn_my,
                            {{tkn_uid, user_id}}
                        ),
                        call_type_json
                    );
                }

                QJsonValue usersById(const QString & user_id) {
                    return sRequest(
                        baseUrlStr(qst_site, QStringLiteral("profile/") % user_id, {}),
                        call_type_html, 0, proc_user3
                    );
                }

                QJsonValue usersByName(const QString & /*name*/) {
//                    return sRequest(
//                        baseUrlStr(qst_site, QStringLiteral("profile/") % user_id, {}),
//                        call_type_html, 0, proc_user1
//                    );
                    return QJsonArray();
                }

//                QJsonValue popular(const SearchLimit & /*limits*/) {
//                    return pRequest(
//                        popAudioUrl(),
//                        call_type_json,
//                        rules(),
//                        0,
//                        proc_json_extract,
//                        QStringList() << tkn_tracks
//                    );

////                    return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
//                }
            };
        }
    }
}

#endif // OD_USER
