#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include "../model_interface.h"

namespace Playo3 {
    class ListModel : public ModelInterface {
        Q_OBJECT

    public:
        ListModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~ListModel();
    protected:
        int dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list);
        void filesRoutine(ItemInterface * index, QFileInfo currFile);
        void filesRoutine(ItemInterface * index, QList<QUrl> list);
    };
}

#endif // LIST_MODEL_H
