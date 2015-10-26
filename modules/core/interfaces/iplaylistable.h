#ifndef IPLAYLISTABLE
#define IPLAYLISTABLE

#include "modules/core/core_parts/item_interface.h"

#include <qabstractitemmodel.h>

namespace Core {
    class IPlaylistable {
    public:
        virtual QModelIndex index(IItem * /*item*/) const { return QModelIndex(); }
    };
}

#endif // IPLAYLISTABLE
