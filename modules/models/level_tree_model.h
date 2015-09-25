#ifndef LEVEL_TREE_MODEL_H
#define LEVEL_TREE_MODEL_H

#include "model_interface.h"

namespace Model {
    class LevelTreeModel : public IModel {
        Q_OBJECT
    public:
        inline LevelTreeModel(QJsonObject * hash = 0, QObject * parent = 0) : IModel(hash, parent) {}

        inline bool isRelative() const { return false; }
        inline Data::Type playlistType() const { return level_tree; }
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, Playlist * node, QHash<Playlist *, int> & rels);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);
    };
}

#endif // LEVEL_TREE_MODEL_H
