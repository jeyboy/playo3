#include "list_model.h"

using namespace Playo3;

ListModel::ListModel(QJsonObject * hash, QObject * parent) : IModel(hash, parent) {

}

ListModel::~ListModel() {

}

void ListModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/) {
    exIndex = (const_cast<QModelIndex &>(dIndex)) = index(rootItem);
    exRow = rootItem -> childCount();
    dRow = -1;
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

    for(int loop1 = 0; loop1 < folderList.size(); loop1++)
        res += filesRoutine(folderList[loop1], node);

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    res += fileList.size();
    QFileInfo file;
    for(int loop1 = 0; loop1 < fileList.size(); loop1++) {
        file = fileList[loop1];
        new FileItem(file.path(), file.fileName(), node);
    }

    return res;
}

int ListModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;

    for(int loop1 = 0; loop1 < list.size(); loop1++) {
        QFileInfo file = QFileInfo(list[loop1].toLocalFile());
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
