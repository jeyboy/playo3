#include "list_model.h"

using namespace Playo3;

ListModel::ListModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {

}

ListModel::~ListModel() {

}

void ListModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {   
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int ListModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    int res = 0;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList)
        res += filesRoutine(file, node);

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    res += fileList.size();
    foreach(QFileInfo file, fileList)
        new FileItem(file.path(), file.fileName(), node);

    return res;
}

int ListModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, node);
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
