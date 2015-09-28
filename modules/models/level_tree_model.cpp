#include "level_tree_model.h"

using namespace Models;

void LevelTreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
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

void LevelTreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    if (count > 0) {
        rootItem -> updateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int LevelTreeModel::filesRoutine(QFileInfo & currFile, Playlist * node, QHash<Playlist *, int> & rels) {
    int res = 0;
    rels[node]++;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    if (!folderList.isEmpty()) {
        QFileInfoList::Iterator it = folderList.begin();

        for(; it != folderList.end(); it++)
            res += filesRoutine((*it), rootItem -> createPlaylist(Extensions::folderName((*it))), rels);
    }

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    if (!fileList.isEmpty()) {
        QFileInfoList::Iterator it = fileList.begin();

        res += fileList.size();
        for(; it != fileList.end(); it++)
            new File((*it).path(), (*it).fileName(), node);

        node -> updateItemsCountInBranch(fileList.size());
    }

    if (node -> childCount() == 0) {
        if (--rels[node] == 0)
            node -> removeYouself();
    }

    return res;
}

int LevelTreeModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();
    QHash<Playlist *, int> relations;

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, rootItem -> createPlaylist(Extensions::folderName(file)), relations);
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new File(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
