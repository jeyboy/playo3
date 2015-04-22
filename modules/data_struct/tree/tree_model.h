#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include "../model_interface.h"

namespace Playo3 {
    class TreeModel : public IModel {
        Q_OBJECT

    public:
        TreeModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~TreeModel();

        inline bool isRelative() const { return true; }
        inline ContainerType containerType() const { return tree; }

    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, FolderItem * node);
        int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);
    };
}

#endif // TREE_MODEL_H
