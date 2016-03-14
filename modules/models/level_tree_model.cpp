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

int LevelTreeModel::filesRoutine(const QString & filePath, Playlist * node, QHash<Playlist *, int> & rels, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;

    rels[node]++;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, rootItem -> createPlaylist(dir_it.fileName()), rels, unproc_files, items);
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    int local_res = 0;
    QString cue_ext = QStringLiteral(".cue");

    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        if (!unproc_files.contains(path)) {
            DataItem * item = DataCore::obj().dataItem(path);
            if (item) continue;

            if (name.endsWith(cue_ext, Qt::CaseInsensitive))
                local_res += proceedCue(path, name, node, -1, unproc_files, items);
            else {
                QString _title = name, ext;
                Extensions::obj().extractExtension(_title, ext);
                REGISTER_LOCAL_DATA(path, _title, ext);
                items.insert(path, new IItem(path, node));
                local_res++;
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
    QString cue_ext = QStringLiteral("cue");
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString path = file.filePath();

        if (file.isDir())
            res += filesRoutine(path, rootItem -> createPlaylist(Extensions::folderName(file)), relations, unproc_files, items);
        else { 
            if (unproc_files.contains(path)) continue;
            if (Extensions::obj().respondToExtension(file.suffix())) {
                DataItem * item = DataCore::obj().dataItem(path);
                if (item) continue;

                if (file.suffix().endsWith(cue_ext, Qt::CaseInsensitive))
                    res += proceedCue(path, file.fileName(), node, pos, unproc_files, items);
                else {
                    QString _title = file.fileName(), ext;
                    Extensions::obj().extractExtension(_title, ext);
                    REGISTER_LOCAL_DATA(path, _title, ext);
                    items.insert(path, new IItem(path, node, pos));
                    res++;
                }
            }
        }
    }

    return res;
}
