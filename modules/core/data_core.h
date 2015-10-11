#ifndef DATA_CORE_H
#define DATA_CORE_H

#include <qdebug.h>
#include <qobject.h>
#include <qhash.h>
#include <qabstractitemmodel.h>

#include "core_parts_index.h"
#include "modules/core/media/library.h"
#include "modules/core/interfaces/singleton.h"

namespace Core {
    class DataFactory : public QObject, public QHash<QString, IItem *>, public Singleton<DataFactory> {
        Q_OBJECT
    public:
        inline DataFactory() : QObject() { }
        ~DataFactory() {}
    public slots:
        void proceedInfo(const QModelIndex & ind) {
            IItem * node = static_cast<IItem *>(ind.internalPointer());

            bool is_interactive = Settings::instance() -> isInteractiveProc();
            bool is_proceeded = node -> is(ItemState::proceeded);

            if (!is_proceeded) {
                if (!is_interactive)
                    node -> set(ItemState::proceeded);
                if (!node -> isContainer())
                    Library::instance() -> restoreItemState(ind);
            }
        }
    };
}

#endif // DATA_CORE_H
