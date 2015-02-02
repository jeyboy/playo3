#include "level_tree_model.h"

using namespace Playo3;

LevelTreeModel::LevelTreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {

}

LevelTreeModel::~LevelTreeModel() {

}

void LevelTreeModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) { //TODO: test needed
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

    (const_cast<QModelIndex&>(dIndex)) = index(node);

    if (dIndex != exIndex)
        dRow = -1;
}




//void ListModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
//    FolderItem * newIndex = item(ind);
//    filesRoutine(list, newIndex, row);
//}

//void ListModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
//    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

//    foreach(QFileInfo file, folderList)
//        filesRoutine(file, node);

//    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

//    foreach(QFileInfo file, fileList)
//        new FileItem(file.filePath(), node);
//}

//void ListModel::filesRoutine(const QList<QUrl> & /*list*/, FolderItem * /*node*/, int /*pos*/){
//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir())
//            filesRoutine(file, node);
//        else {
//            if (Extensions::instance() -> respondToExtension(file.suffix()))
//                new FileItem(file.filePath(), node, pos);
//        }
//    }
//}



void LevelTreeModel::dropProcession(const QModelIndex & /*ind*/, int /*row*/, const QList<QUrl> & /*list*/) {
//    QFileInfo file = QFileInfo(list.first().toLocalFile());
//    QString fName;

//    if (file.isDir())
//        fName = file.fileName();
//    else
//        fName = Extensions::folderName(file);

//    ItemInterface * newIndex = addFolder("", fName, rootItem);

//    filesRoutine(newIndex, list);
//    if (newIndex -> childCount() == 0) {
//        removeFolderPrebuild(newIndex);
//        return QModelIndex();
//    }
//    else return index(newIndex);
}

void LevelTreeModel::filesRoutine(QFileInfo & /*currFile*/, FolderItem * /*node*/) {
//    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
//    ItemInterface * newFolder;
//    bool already_exist;

//    foreach(QFileInfo file, folderList) {
//        already_exist = isFolderExist(file.fileName(), rootItem);
//        newFolder = addFolder("", file.fileName(), rootItem);
//        filesRoutine(newFolder, file);
//        if (!already_exist && newFolder -> childCount() == 0)
//            removeFolderPrebuild(newFolder);
//    }


//    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

//    foreach(QFileInfo file, fileList) {
//        appendRow(createItem(file.filePath(), index));
//    }
}

void LevelTreeModel::filesRoutine(const QList<QUrl> & /*list*/, FolderItem * /*node*/, int /*pos*/) {
//    ItemInterface * newFolder;
//    bool already_exist;

//    foreach(QUrl url, list) {
//        QFileInfo file = QFileInfo(url.toLocalFile());
//        if (file.isDir()) {
//            already_exist = isFolderExist(file.fileName(), rootItem);
//            newFolder = addFolder("", file.fileName(), rootItem);
//            filesRoutine(newFolder, file);
//            if (!already_exist && newFolder -> childCount() == 0)
//                removeFolderPrebuild(newFolder);
//        } else {
//            if (Extensions::instance() -> respondToExtension(file.suffix()))
//                appendRow(createItem(file.filePath(), index));
//        }
//    }
}
