#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include "model_interface.h"

namespace Model {
    class TreeModel : public IModel {
        Q_OBJECT
    public:
        inline TreeModel(QJsonObject * hash = 0, QObject * parent = 0) : IModel(hash, parent) {}

        inline bool isRelative() const { return true; }
        inline Data::Type playlistType() const { return Data::tree; }

    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, Playlist * node);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);
    };
}

#endif // TREE_MODEL_H
