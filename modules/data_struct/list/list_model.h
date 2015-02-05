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
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, FolderItem * node);
        int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);
    };
}

#endif // LIST_MODEL_H
