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
        QModelIndex dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list);
        void filesRoutine(QFileInfo & currFile, FolderItem * index, int pos = -1);
        void filesRoutine(QList<QUrl> & list, FolderItem * index);
    };
}

#endif // LIST_MODEL_H
