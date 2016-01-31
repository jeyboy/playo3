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

int TreeModel::filesRoutine(const QString & filePath, Playlist * node) {
    int res = 0;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dir_it.fileName()));
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        res++;
        dir_it.next();

        new File(dir_it.fileName(), node);
    }

    if (res > 0)
        node -> updateItemsCountInBranch(res);
    else
        node -> removeYouself();

    return res;
}

int TreeModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;

    Logger::obj().startMark();
    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir()) {
            res += filesRoutine(file.filePath(), node -> createPlaylist(file.fileName(), 0, pos));
        } else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.fileName(), node, pos);
            }
        }
    }
    Logger::obj().endMark("list", "parsing");

    return res;
}
