#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include "../model_interface.h"

namespace Playo3 {
    class ListModel : public IModel {
        Q_OBJECT
    public:
        inline ListModel(const Json::Obj & hash = Json::Obj(), QObject * parent = 0) : IModel(hash, parent) {}

        inline bool isRelative() const { return false; }
        inline ContainerType containerType() const { return list; }
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, FolderItem * node);
        int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);
    };
}

#endif // LIST_MODEL_H
