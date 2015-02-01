#include "tree_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {
}

TreeModel::~TreeModel() {
}

void TreeModel::recalcParentIndex(QModelIndex & index, int & row, QUrl & url) {
    QString path;

    QFileInfo file = QFileInfo(url.toLocalFile());
    path =  file.isDir() ? Extensions::folderName(file) : file.path();

    FolderItem * node = rootItem -> createFolderPath(path);

    QModelIndex newParent = index(node);
    if (newParent != parent)
        row = -1;
}

QModelIndex TreeModel::dropProcession(const QModelIndex & parent, int /*row*/, const QList<QUrl> & list) {
    if (list.isEmpty()) return QModelIndex();
//    FolderItem * currRoot = item<FolderItem>(parent);
    FolderItem * node = item(parent);//rootItem -> createFolderPath(QFileInfo(list.first().toLocalFile()).path());
    filesRoutine(list, node);
    return index(node);
}

void TreeModel::filesRoutine(QFileInfo & currFile, FolderItem * index, int pos) {
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        filesRoutine(file, index -> createFolder(file.fileName()));

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList)
        new FileItem(file.fileName(), index);
}

void TreeModel::filesRoutine(QList<QUrl> & list, FolderItem * index) {
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(file, index -> createFolder(file.fileName()));
        } else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                new FileItem(file.fileName(), index);
        }
    }
}
