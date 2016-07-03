#ifndef SOUNDCLOUD_API_SET
#define SOUNDCLOUD_API_SET

#include "defines.h"
#include "../interfaces/iset.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Set : public QueryBase, public ISet {
                protected:
                    QJsonValue popular(const SearchLimit & limitations) {
                        return pRequest(
                            baseUrlStr(
                                qst_api_def,
                                path_tracks,
                                trackSearchQuery(QString(), limitations.genre, true)
                            ),
                            call_type_json, rules(), 0, proc_json_patch
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_SET
