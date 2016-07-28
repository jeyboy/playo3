#ifndef OD_COLLECTION
#define OD_COLLECTION

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Collection : public Base {
            protected:
                inline QString collectionsAudioUrl() { return audioUrlStr(path_audio_collections); } // params: (collectionId: not used for index of collections) and pagination attrs
            };
        }
    }
}

#endif // OD_COLLECTION
