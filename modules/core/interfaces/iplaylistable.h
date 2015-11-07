#ifndef IPLAYLISTABLE
#define IPLAYLISTABLE

#include "modules/core/core_parts/item_interface.h"

#include <qabstractitemmodel.h>

namespace Core {
    class IPlaylistable {
    protected:
        virtual bool setData(QModelIndex ind, int state) = 0;
    public:
        virtual bool restoreItem(IItem * itm) = 0;
        virtual bool isPlaylist() const = 0;
        virtual void execNextIndex(bool deleteCurrent = false) = 0;
        virtual QModelIndex index(IItem * /*item*/) const { return QModelIndex(); }
        bool setState(QModelIndex ind, int state) {
            if (ind.isValid()) return setData(ind, state);
            else return false;
        }
    };
}

#endif // IPLAYLISTABLE
