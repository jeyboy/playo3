#ifndef LEVEL_TREE_MODEL_H
#define LEVEL_TREE_MODEL_H

#include "model_interface.h"

namespace Models {
    class LevelTreeModel : public IModel {
        Q_OBJECT
    public:
        inline LevelTreeModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : IModel(settings, hash, parent) {}

        inline bool isRelative() const { return false; }
        inline Core::DataSubType playlistType() const { return Core::dt_level_tree; }
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(const QString & filePath, Playlist * node, QHash<Playlist *, int> & rels, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);
    };
}

#endif // LEVEL_TREE_MODEL_H
