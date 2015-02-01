#ifndef LEVEL_TREE_MODEL_H
#define LEVEL_TREE_MODEL_H

#include "../model_interface.h"

namespace Playo3 {
    class LevelTreeModel : public ModelInterface {
        Q_OBJECT

    public:
        LevelTreeModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~LevelTreeModel();

    protected:
        QModelIndex dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list);
        void filesRoutine(QFileInfo & currFile, FolderItem * index, int pos = -1);
        void filesRoutine(QList<QUrl> & list, FolderItem * index);
    };
}

#endif // LEVEL_TREE_MODEL_H
