#include "model/tree/tree_model.h"
#include <QDebug>

///////////////////////////////////////////////////////////

TreeModel::TreeModel(QJsonObject * hash, QObject *parent) : Model(hash, parent) {

}

TreeModel::~TreeModel() {
//    delete rootItem;
}

QModelIndex TreeModel::dropProcession(const QList<QUrl> & list) {
    ModelItem * newIndex = buildPath(QFileInfo(list.first().toLocalFile()).path());
    filesRoutine(newIndex, list);
    return index(newIndex);
}

void TreeModel::filesRoutine(ModelItem * index, QFileInfo currFile){
    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);

    foreach(QFileInfo file, folderList) {
        ModelItem * new_index = addFolder(file.fileName(), index);
        filesRoutine(new_index, file);
    }

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    foreach(QFileInfo file, fileList) {
        appendRow(createItem(file.fileName(), index));
    }
}

void TreeModel::filesRoutine(ModelItem * index, QList<QUrl> list){
    qDebug() << list;

    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            ModelItem * new_index = addFolder(file.fileName(), index);
            filesRoutine(new_index, file);
        } else {
            if (Extensions::instance() -> respondToExtension(file.suffix()))
                appendRow(createItem(file.fileName(), index));
        }
    }
}
