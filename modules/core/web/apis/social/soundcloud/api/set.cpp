#include "set.h"

#include "../site/requests.h"

using namespace Core::Web::Soundcloud::Api;

QJsonValue Set::setByType(const SetType & setType, const QString & genre = QString()) {
    switch(setType) {
        case popular: {
            return pRequest(
                baseUrlStr(
                    qst_api_def, path_tracks,
                    trackSearchQuery(QString(), genre, true)
                ),
                call_type_json, rules(), 0, proc_json_patch
            );
        }

        default: return Site::Requests::obj().setByType(setType, genre); // need genre conversion
    }
}
