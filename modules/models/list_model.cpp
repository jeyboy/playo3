#include "list_model.h"

using namespace Models;

void ListModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url) {
    if (!url.isEmpty()) {
        exIndex = (const_cast<QModelIndex &>(dIndex)) = index(rootItem);
        if (dRow == -1)
            exRow = rootItem -> childCount();
        else
            exRow = dRow;
    }
    else IModel::recalcParentIndex(dIndex, dRow, exIndex, exRow, url);
}

void ListModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {   
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int ListModel::filesRoutine(const QString & filePath, Playlist * node) {
    int res = 0;

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS), QDirIterator::Subdirectories);
    while(dir_it.hasNext()) {
        res++;
        QString path = dir_it.next();
        new File(path, dir_it.fileName(), node);
    }

    return res;
}

int ListModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file.filePath(), node);
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
