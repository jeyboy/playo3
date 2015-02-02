#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include "../model_interface.h"

namespace Playo3 {
    class TreeModel : public ModelInterface {
        Q_OBJECT

    public:
        TreeModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~TreeModel();

    protected:
        QModelIndex recalcParentIndex(QModelIndex & ind, int & row, QUrl url);
        QModelIndex dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        void filesRoutine(QFileInfo & currFile, FolderItem * node);
        void filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);
    };
}

#endif // TREE_MODEL_H
