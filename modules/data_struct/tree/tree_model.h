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
        int dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list);
        void filesRoutine(ItemInterface * index, QFileInfo currFile);
        void filesRoutine(ItemInterface * index, QList<QUrl> list);
    };
}

#endif // TREE_MODEL_H
