#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include "model_interface.h"

namespace Models {
    class ListModel : public IModel {
        Q_OBJECT
    public:
        inline ListModel(QJsonObject * hash = 0, QObject * parent = 0) : IModel(hash, parent) {}
        virtual ~ListModel() {}

        inline bool isRelative() const { return false; }
        inline Data::Type playlistType() const { return Data::level; }
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        virtual int filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files);
        virtual int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);
    };
}

#endif // LIST_MODEL_H
