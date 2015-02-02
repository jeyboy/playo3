#include "tree_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {
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

    (const_cast<QModelIndex&>(dIndex)) = index(node);
    exRow = nearestNode -> row();

    if (dIndex != exIndex)
        dRow = -1;
}

QModelIndex TreeModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    if (list.isEmpty()) return QModelIndex();
    FolderItem * node = item<FolderItem>(ind);
    filesRoutine(list, node, row);
    return index(node);
}

void TreeModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        filesRoutine(file, node -> createFolder(file.fileName()));

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList)
        new FileItem(file.fileName(), node);
}

void TreeModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir())
            filesRoutine(file, node -> createFolder(file.fileName(), 0, pos));
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                new FileItem(file.fileName(), node, pos);
        }
    }
}
