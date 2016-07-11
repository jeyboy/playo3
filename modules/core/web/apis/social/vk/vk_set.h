#ifndef VK_SET
#define VK_SET

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Set : public Base {
            public:
                enum SetType { popular_tracks = 1, popular_video };

                QJsonValue setByType(const SetType & /*set_type*/) {

                }
            };
        }
    }
}

#endif // VK_SET
