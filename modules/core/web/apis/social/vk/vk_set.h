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
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {

                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_SET
