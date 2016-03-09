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

int TreeModel::filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dir_it.fileName()), unproc_files, items);
        }
    }

    QString cue_ext = QStringLiteral(".cue");
    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        if (!unproc_files.contains(path)) {
            DataItem * item = DataCore::obj().dataItem(path);
            if (item) continue;

            if (name.endsWith(cue_ext, Qt::CaseInsensitive))
                res += proceedCue(path, name, node, -1, unproc_files, items);
            else {
                item = REGISTER_LOCAL_DATA(path, name);
                items.insert(path, new IItem(path, node));
                res++;
            }
        }
    }

    if (res > 0)
        node -> updateItemsCountInBranch(res);
    else
        node -> removeYouself();

    return res;
}

int TreeModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QString cue_ext = QStringLiteral("cue");
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString path = file.filePath();

        if (file.isDir()) {
            res += filesRoutine(path, node -> createPlaylist(file.fileName(), 0, pos), unproc_files, items);
        } else {
            if (unproc_files.contains(path)) continue;
            if (Extensions::obj().respondToExtension(file.suffix())) {
                DataItem * item = DataCore::obj().dataItem(path);
                if (item) continue;

                if (file.suffix().endsWith(cue_ext, Qt::CaseInsensitive))
                    res += proceedCue(path, file.fileName(), node, pos, unproc_files, items);
                else {
                    item = REGISTER_LOCAL_DATA(path, file.fileName());
                    items.insert(path, new IItem(path, node, pos));
                    res++;
                }
            }
        }
    }

    return res;
}
