#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : IModel(hash, parent) {}

SearchModel::~SearchModel() {}

void SearchModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
    QFileInfo file = QFileInfo(url.toLocalFile());
    QString fName;

    if (file.isDir())
        fName = file.fileName();
    else
        fName = Extensions::folderName(file);

    FolderItem * nearestNode = rootItem -> folderItem(fName);
    FolderItem * node;
    if (!nearestNode) {
        exIndex = index(rootItem);
        exRow = rootItem -> childCount();
        node = rootItem -> createFolder(fName);
    } else {
        node = nearestNode;
        exIndex = index(nearestNode);
        exRow = nearestNode -> row();
    }

    (const_cast<QModelIndex &>(dIndex)) = index(node);

    if (dIndex != exIndex)
        dRow = -1;
}

void SearchModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    if (count > 0) {
        rootItem -> updateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int SearchModel::filesRoutine(QFileInfo & currFile, FolderItem * node, QHash<FolderItem *, int> & rels) {
    int res = 0;
    rels[node]++;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    if (!folderList.isEmpty()) {
        QFileInfoList::Iterator it = folderList.begin();

        for(; it != folderList.end(); it++)
            res += filesRoutine((*it), rootItem -> createFolder(Extensions::folderName((*it))), rels);
    }

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    if (!fileList.isEmpty()) {
        QFileInfoList::Iterator it = fileList.begin();

        res += fileList.size();
        for(; it != fileList.end(); it++)
            new FileItem((*it).path(), (*it).fileName(), node);

        node -> updateItemsCountInBranch(fileList.size());
    }

    if (node -> childCount() == 0) {
        if (--rels[node] == 0)
            node -> removeYouself();
    }

    return res;
}

int SearchModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();
    QHash<FolderItem *, int> relations;

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, rootItem -> createFolder(Extensions::folderName(file)), relations);
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
