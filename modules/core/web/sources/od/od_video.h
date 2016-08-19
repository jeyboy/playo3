#ifndef OD_VIDEO
#define OD_VIDEO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Video : public virtual Base {
            protected:
                QJsonValue videoSearch(const SearchLimit & limits) {
                    QueriableResponse response;

                    //TODO: finish me

                    return prepareBlock(dmt_video, cmd_mtd_video_search, response, limits);
                }
            };
        }
    }
}

#endif // OD_VIDEO
