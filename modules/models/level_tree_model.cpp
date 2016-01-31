#include "level_tree_model.h"

using namespace Models;

void LevelTreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url) {
    if (!url.isEmpty()) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        QString fName;

        if (file.isDir())
            fName = file.fileName();
        else
            fName = Extensions::folderName(file);

        Playlist * nearestNode = rootItem -> playlist(fName);
        Playlist * node;
        if (!nearestNode) {
            exIndex = index(rootItem);
            exRow = rootItem -> childCount();
            node = rootItem -> createPlaylist(fName);
        } else {
            node = nearestNode;
            exIndex = index(nearestNode);
            exRow = nearestNode -> row();
        }

        (const_cast<QModelIndex &>(dIndex)) = index(node);

        if (dIndex != exIndex)
            dRow = -1;
    }
    else IModel::recalcParentIndex(dIndex, dRow, exIndex, exRow, url);
}

void LevelTreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    if (count > 0) {
        rootItem -> updateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int LevelTreeModel::filesRoutine(const QString & filePath, Playlist * node, QHash<Playlist *, int> & rels) {
    int res = 0;
    rels[node]++;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, rootItem -> createPlaylist(dir_it.fileName()), rels);
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    int local_res = 0;
    while(dir_it.hasNext()) {
        local_res++;
        QString path = dir_it.next();
        new File(path, dir_it.fileName(), node);
    }

    if (local_res > 0) {
        node -> updateItemsCountInBranch(local_res);
        res += local_res;
    }

    if (node -> childCount() == 0) {
        if (--rels[node] == 0)
            node -> removeYouself();
    }

    return res;
}

int LevelTreeModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QHash<Playlist *, int> relations;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file.filePath(), rootItem -> createPlaylist(Extensions::folderName(file)), relations);
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.filePath(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
