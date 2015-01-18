#include "tree_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {
}

TreeModel::~TreeModel() {
}

int TreeModel::dropProcession(const QModelIndex & /*parent*/, int /*row*/, const QList<QUrl> & list) {
    if (list.isEmpty()) return 0;
//    FolderItem * currRoot = item<FolderItem>(parent);
    FolderItem * newIndex = rootItem -> createFolderPath(QFileInfo(list.first().toLocalFile()).path());
    return filesRoutine(newIndex, list);
}

int TreeModel::filesRoutine(FolderItem * index, QFileInfo currFile){
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        filesRoutine(index -> createFolder(file.fileName()), file);

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList)
        new FileItem(file.fileName(), index);
}

int TreeModel::filesRoutine(FolderItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            filesRoutine(index -> createFolder(file.fileName()), file);
        } else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                new FileItem(file.fileName(), index);
        }
    }
}
