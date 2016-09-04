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

        Playlist * nearestNode = root_item -> playlist(fName);
        Playlist * node;
        if (!nearestNode) {
            exIndex = index(root_item);
            exRow = root_item -> childCount();
            node = root_item -> createPlaylist(dt_playlist, fName);
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
        root_item -> updateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int LevelTreeModel::filesRoutine(const QString & filePath, Playlist * node, QHash<Playlist *, int> & rels, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;

    rels[node]++;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, root_item -> createPlaylist(dt_playlist, dir_it.fileName()), rels, unproc_files, items);
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    int local_res = 0;

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        if (!unproc_files.contains(path)) {
            if (name.endsWith(Extensions::dot_cue_ext, Qt::CaseInsensitive))
                local_res += proceedCue(path, name, node, -1, unproc_files, items);
            else {
                local_res++;
                items.insert(path, new IItem(node, LOCAL_ITEM_ATTRS(path, name)));
            }
        }
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
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString path = file.filePath();
        QString name = file.fileName();

        if (file.isDir())
            res += filesRoutine(path, root_item -> createPlaylist(dt_playlist, Extensions::folderName(file)), relations, unproc_files, items);
        else { 
            if (unproc_files.contains(path)) continue;
            if (Extensions::obj().respondToExtension(file.suffix())) {
                if (file.suffix().endsWith(Extensions::cue_ext, Qt::CaseInsensitive))
                    res += proceedCue(path, name, node, pos, unproc_files, items);
                else {
                    res++;
                    items.insert(path, new IItem(node, LOCAL_ITEM_ATTRS(path, name), pos));
                }
            }
        }
    }

    return res;
}
