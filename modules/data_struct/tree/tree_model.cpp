#include "tree_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {

}

TreeModel::~TreeModel() {

}

int TreeModel::dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list) {
//    ModelItem * newIndex = buildPath(QFileInfo(list.first().toLocalFile()).path());
//    filesRoutine(newIndex, list);
//    return index(newIndex);
}

void TreeModel::filesRoutine(ItemInterface * index, QFileInfo currFile){
//    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

//    foreach(QFileInfo file, folderList) {
//        ItemInterface * new_index = addFolder(file.fileName(), index);
//        filesRoutine(new_index, file);
//    }

//    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

//    foreach(QFileInfo file, fileList) {
//        appendRow(createItem(file.fileName(), index));
//    }
}

void TreeModel::filesRoutine(ItemInterface * index, QList<QUrl> list){
//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir()) {
//            ItemInterface * new_index = addFolder(file.fileName(), index);
//            filesRoutine(new_index, file);
//        } else {
//            if (Extensions::instance() -> respondToExtension(file.suffix()))
//                appendRow(createItem(file.fileName(), index));
//        }
//    }
}
