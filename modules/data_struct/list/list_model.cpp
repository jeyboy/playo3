#include "list_model.h"

using namespace Playo3;

ListModel::ListModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {

}

ListModel::~ListModel() {

}

void ListModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * newIndex = item<FolderItem>(ind);
    filesRoutine(list, newIndex, row);
}

void ListModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        filesRoutine(file, node);

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList)
        new FileItem(file.path(), file.fileName(), node);
}

void ListModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir())
            filesRoutine(file, node);
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                new FileItem(file.path(), file.fileName(), node, pos);
        }
    }
}
