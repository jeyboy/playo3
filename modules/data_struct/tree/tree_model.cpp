#include "tree_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject * parent) : IModel(hash, parent) {
}

TreeModel::~TreeModel() {
}


void TreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
    QFileInfo file = QFileInfo(url.toLocalFile());
    QString path = file.path();
    if (path.isEmpty()) path = Extensions::folderName(file);

    QStringList list = path.split('/', QString::SkipEmptyParts);
    FolderItem * nearestNode = rootItem -> findNearestFolder(&list);
    FolderItem * node = list.isEmpty() ? nearestNode : nearestNode -> createFolder(list.takeFirst(), &list);
    exIndex = index(nearestNode);

    (const_cast<QModelIndex &>(dIndex)) = index(node);
    exRow = nearestNode -> row();

    if (dIndex != exIndex)
        dRow = -1;
}

void TreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int TreeModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    int res = 0;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        res += filesRoutine(file, node -> createFolder(file.fileName()));

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    res += fileList.size();
    foreach(QFileInfo file, fileList)
        new FileItem(file.fileName(), node);

    node -> updateItemsCountInBranch(res);
    return res;
}

int TreeModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, node -> createFolder(file.fileName(), 0, pos));
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.fileName(), node, pos);
            }
        }
    }

    return res;
}
