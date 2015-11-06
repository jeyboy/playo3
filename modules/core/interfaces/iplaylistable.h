#ifndef IPLAYLISTABLE
#define IPLAYLISTABLE

#include "modules/core/core_parts/item_interface.h"

#include <qabstractitemmodel.h>

namespace Core {
    class IPlaylistable {
    protected:
        virtual bool setData(QModelIndex ind, int state) = 0;
    public:
        virtual void execNextIndex(bool deleteCurrent = false) = 0;
        virtual QModelIndex index(IItem * /*item*/) const { return QModelIndex(); }
        bool setState(QModelIndex ind, int state) {
            if (ind.isValid()) setData(ind, state);
        }
    };
}

#endif // IPLAYLISTABLE
