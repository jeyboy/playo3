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
                    QJsonArray block_content = saRequest(
                        audioUrlStr(path_audio_collections),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_collections
                    );

                    prepareCollections(block_content);
                    return prepareBlock(dmt_audio_set, block_content);
                }
            };
        }
    }
}

#endif // OD_COLLECTION
