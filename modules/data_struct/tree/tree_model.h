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
        void recalcParentIndex(QModelIndex & index, int & row, QUrl & url);
        QModelIndex dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list);
        void filesRoutine(QFileInfo & currFile, FolderItem * index, int pos = -1);
        void filesRoutine(QList<QUrl> & list, FolderItem * index);
    };
}

#endif // TREE_MODEL_H
