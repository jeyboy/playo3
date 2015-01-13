#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include "modules/data_struct/view_types.h"

namespace Playo3 { //TODO: maybe change on bitset ?
    struct ViewSettings {
        bool deleteFile;
        bool playlist;
        bool interactive;
        bool common;

        ViewType type;
    };
}

#endif // VIEW_SETTINGS
