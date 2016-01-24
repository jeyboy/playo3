#include "tree_model.h"

using namespace Models;
///////////////////////////////////////////////////////////

void TreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url) {
    if (!url.isEmpty()) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        QString path = file.path();
        if (path.isEmpty()) path = Extensions::folderName(file);

        QStringList list = path.split('/', QString::SkipEmptyParts);
        Playlist * nearestNode = rootItem -> findCompatblePlaylist(&list);
        Playlist * node = list.isEmpty() ? nearestNode : nearestNode -> createPlaylist(list.takeFirst(), &list);
        exIndex = index(nearestNode);

        (const_cast<QModelIndex &>(dIndex)) = index(node);
        exRow = nearestNode -> row();

        if (dIndex != exIndex)
            dRow = -1;
    }
    else IModel::recalcParentIndex(dIndex, dRow, exIndex, exRow, url);
}

void TreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    if (count > 0) {
        node -> backPropagateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int TreeModel::filesRoutine(QFileInfo & currFile, Playlist * node) {
    int res = 0;
    QStringList nameFilters = Extensions::obj().activeFilterList();

    QFileInfoList folderList;
    FileSystemWatcher::foldersList(currFile, folderList, nameFilters);

    if (!folderList.isEmpty()) {
        for(QFileInfoList::Iterator it = folderList.begin(); it != folderList.end(); it++)
            res += filesRoutine((*it), node -> createPlaylist((*it).fileName()));
    }

    QFileInfoList fileList;
    FileSystemWatcher::filesList(currFile, fileList, nameFilters);

    if (!fileList.isEmpty()) {
        res += fileList.size();
        for(QFileInfoList::Iterator it = fileList.begin(); it != fileList.end(); it++)
            new File((*it).fileName(), node);
    }

    if (res > 0)
        node -> updateItemsCountInBranch(res);
    else
        node -> removeYouself();
    return res;
}

int TreeModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir()) {
            res += filesRoutine(file, node -> createPlaylist(file.fileName(), 0, pos));
        } else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.fileName(), node, pos);
            }
        }
    }

    return res;
}
