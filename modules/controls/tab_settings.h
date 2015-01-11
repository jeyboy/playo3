#ifndef TAB_SETTINGS
#define TAB_SETTINGS

#include "views/view_types.h"

namespace Playo3 {
    struct TabSettings {
        bool deleteFile;
        bool playlist;
        bool interactive;

        ViewType type;
    };
}

#endif // TAB_SETTINGS
