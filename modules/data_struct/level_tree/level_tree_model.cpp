#include "level_tree_model.h"

using namespace Playo3;

LevelTreeModel::LevelTreeModel(QJsonObject * hash, QObject * parent) : IModel(hash, parent) {

}

LevelTreeModel::~LevelTreeModel() {

}

void LevelTreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
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

void LevelTreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    rootItem -> updateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int LevelTreeModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    int res = 0;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        res += filesRoutine(file, rootItem -> createFolder(Extensions::folderName(file)));

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    res += fileList.size();
    foreach(QFileInfo file, fileList)
        new FileItem(file.path(), file.fileName(), node);

    node -> updateItemsCountInBranch(fileList.size());
    return res;
}

int LevelTreeModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, rootItem -> createFolder(Extensions::folderName(file)));
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
