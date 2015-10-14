#include "list_model.h"

using namespace Models;

void ListModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/) {
    exIndex = (const_cast<QModelIndex &>(dIndex)) = index(rootItem);
    if (dRow == -1)
        exRow = rootItem -> childCount();
    else
        exRow = dRow;
}

void ListModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {   
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int ListModel::filesRoutine(QFileInfo & currFile, Playlist * node) {
    int res = 0;

    QFileInfoList folderList = Extensions::obj().folderDirectories(currFile);
    {
        QFileInfoList::Iterator it = folderList.begin();

        for(; it != folderList.end(); it++)
            res += filesRoutine((*it), node);
    }

    QFileInfoList fileList = Extensions::obj().folderFiles(currFile);
    QFileInfoList::Iterator it = fileList.begin();

    res += fileList.size();
    for(; it != fileList.end(); it++)
        new File((*it).path(), (*it).fileName(), node);

    return res;
}

int ListModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, node);
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
