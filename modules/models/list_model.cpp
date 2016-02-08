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

int ListModel::filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;
    QString cue_ext = QStringLiteral(".cue");

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS), QDirIterator::Subdirectories);
    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        if (!unproc_files.contains(path)) {
            if (name.endsWith(cue_ext, Qt::CaseInsensitive))
                res += proceedCue(path, name, node, -1, unproc_files, items);
            else {
                res++;
                items.insert(path, new File(path, name, node));
            }
        }
    }

    return res;
}

int ListModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QString cue_ext = QStringLiteral("cue");
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file.filePath(), node, unproc_files, items);
        else {
            if (unproc_files.contains(file.filePath())) continue;
            if (Extensions::obj().respondToExtension(file.suffix())) {
                if (file.suffix().endsWith(cue_ext, Qt::CaseInsensitive))
                    res += proceedCue(file.filePath(), file.fileName(), node, pos, unproc_files, items);
                else {
                    res++;
                    items.insert(file.filePath(), new File(file.filePath(), file.fileName(), node, pos));
                }
            }
        }
    }

    return res;
}
