#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include "model_interface.h"

namespace Models {
    class TreeModel : public IModel {
        Q_OBJECT
    public:
        inline TreeModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : IModel(settings, hash, parent) {}

        inline bool isRelative() const { return true; }
        inline Core::DataSubType playlistType() const { return dt_tree; }

    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);
    };
}

#endif // TREE_MODEL_H
