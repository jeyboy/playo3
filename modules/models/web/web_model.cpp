#include "web_model.h"

#include <qdebug.h>

using namespace Models;
///////////////////////////////////////////////////////////
bool WebModel::removeRows(int position, int rows, const QModelIndex & parent) {
    Playlist * parentItem = item<Playlist>(parent);
    QStringList uids = parentItem -> childrenUids(position, rows);

    bool res = IModel::removeRows(position, rows, parent);

    qDebug() << "IGNORE LIST UPDATE" << res << uids;
    if (res)
        ignoreListAddUid(uids);

    return res;
}

QJsonObject WebModel::toJson() {
    QJsonObject res = IModel::toJson();
    return ignoreListToJson(res);
}

void WebModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/) {
    exIndex = (const_cast<QModelIndex &>(dIndex)) = index(rootItem);
    if (dRow == -1)
        exRow = rootItem -> childCount();
    else
        exRow = dRow;
}

void WebModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    Playlist * node = item<Playlist>(ind);
    int count = filesRoutine(list, node, row);

    node -> backPropagateItemsCountInBranch(count);
    if (count > 0) emit itemsCountChanged(count);
}

int WebModel::filesRoutine(QFileInfo & currFile, Playlist * node) {
    int res = 0;
    QStringList nameFilters = Extensions::obj().activeFilterList();

    QFileInfoList folderList;
    FileSystemWatcher::foldersList(currFile, folderList, nameFilters);
    {
        for(QFileInfoList::Iterator it = folderList.begin(); it != folderList.end(); it++)
            res += filesRoutine((*it), node -> createPlaylist((*it).fileName()));
    }

    QFileInfoList fileList;
    FileSystemWatcher::filesList(currFile, fileList, nameFilters);

    res += fileList.size();
    for(QFileInfoList::Iterator it = fileList.begin(); it != fileList.end(); it++)
        new File((*it).path(), (*it).fileName(), node);

    node -> updateItemsCountInBranch(res);
    return res;
}

int WebModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, node -> createPlaylist(file.fileName(), 0, pos));
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}

void WebModel::errorReceived(int /*code*/, QString msg) {
    qDebug() << "ERROR " << msg;
//    emit showMessage("!!!!!!!!!!! Some shit happened :( " + msg);
    emit moveOutProcess();
}
