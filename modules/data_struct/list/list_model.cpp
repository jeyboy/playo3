#include "list_model.h"

using namespace Playo3;

ListModel::ListModel(QJsonObject * hash, QObject *parent) : ModelInterface(hash, parent) {

}

ListModel::~ListModel() {

}

QModelIndex ListModel::dropProcession(const QModelIndex & /*ind*/, int /*row*/, const QList<QUrl> & /*list*/) {
//    ItemInterface * newIndex = root();
//    filesRoutine(newIndex, list);
//    return index(newIndex);
}

void ListModel::filesRoutine(QFileInfo & /*currFile*/, FolderItem * /*node*/) {
//    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

//    foreach(QFileInfo file, folderList) {
//        filesRoutine(index, file);
//    }

//    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

//    foreach(QFileInfo file, fileList) {
//        appendRow(createItem(file.filePath(), index));
//    }
}

void ListModel::filesRoutine(const QList<QUrl> & /*list*/, FolderItem * /*node*/, int /*pos*/){
//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir()) {
//            filesRoutine(index, file);
//        } else {
//            if (Extensions::instance() -> respondToExtension(file.suffix()))
//                appendRow(createItem(file.filePath(), index));
//        }
//    }
}
