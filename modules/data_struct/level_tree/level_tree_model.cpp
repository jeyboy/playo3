#include "level_tree_model.h"

using namespace Playo3;

LevelTreeModel::LevelTreeModel(QJsonObject * hash, QObject * parent) : ModelInterface(hash, parent) {

}

LevelTreeModel::~LevelTreeModel() {

}

QModelIndex LevelTreeModel::dropProcession(const QModelIndex & /*parent*/, int /*row*/, const QList<QUrl> & /*list*/) {
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

void LevelTreeModel::filesRoutine(QFileInfo & /*currFile*/, FolderItem * /*index*/, int /*pos*/) {
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

void LevelTreeModel::filesRoutine(QList<QUrl> & /*list*/, FolderItem * /*index*/) {
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
