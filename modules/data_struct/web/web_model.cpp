#include "web_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

WebModel::WebModel(QString uid, QJsonObject * hash, QObject * parent) :
    TreeModel(hash, parent), DeletedList(), tabUid(uid) {
    if (hash)
        deletedFromJson(hash -> take("deleted").toArray());
}

WebModel::~WebModel() {
}

QString WebModel::getTabUid() const {
    return tabUid;
}

void WebModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
    QFileInfo file = QFileInfo(url.toLocalFile());
    QString path = file.path();
    if (path.isEmpty()) path = Extensions::folderName(file);

    QStringList list = path.split('/', QString::SkipEmptyParts);
    FolderItem * nearestNode = rootItem -> findNearestFolder(&list);
    FolderItem * node = list.isEmpty() ? nearestNode : nearestNode -> createFolder(list.takeFirst(), &list);
    exIndex = index(nearestNode);

    (const_cast<QModelIndex &>(dIndex)) = index(node);
    exRow = nearestNode -> row();

    if (dIndex != exIndex)
        dRow = -1;
}

void WebModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int WebModel::filesRoutine(QFileInfo & currFile, FolderItem * node) {
    int res = 0;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    {
        QFileInfoList::Iterator it = folderList.begin();

        for(; it != folderList.end(); it++)
            res += filesRoutine((*it), node -> createFolder((*it).fileName()));
    }

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);
    QFileInfoList::Iterator it = fileList.begin();

    res += fileList.size();
    for(; it != fileList.end(); it++)
        new FileItem((*it).fileName(), node);

    node -> updateItemsCountInBranch(res);
    return res;
}

int WebModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, node -> createFolder(file.fileName(), 0, pos));
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.fileName(), node, pos);
            }
        }
    }

    return res;
}

//void WebModel::deleteRemoved(QHash<IItem *, QString> & store) {
//    qDebug() << "STORE LENGTH: " << store.count();
//    foreach(ModelItem * item, store.keys()) {
//        if (Player::instance() -> playedItem() == item)
//            Player::instance() -> setPlaylist(0);

//        qDebug() << "DELETE : " << item -> fullPath();
//        QModelIndex ind = index(item);
//        removeRow(ind.row(), ind.parent());
//    }
//}

void WebModel::errorReceived(int, QString msg) {
    emit showMessage("!!!!!!!!!!! Some shit happened :( " + msg);
//    emit hideSpinner();
}
