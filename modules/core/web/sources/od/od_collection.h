#ifndef OD_COLLECTION
#define OD_COLLECTION

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Collection : public virtual Base {
            protected:
                // collections did not have search or something else - just pseudo(?) random list
                QJsonValue randomCollections() { // TODO: not tested
                    return saRequest(
                        audioUrlStr(path_audio_collections),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_collections
                    );
                }
            };
        }
    }
}

#endif // OD_COLLECTION
