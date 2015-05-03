#include "web_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

WebModel::WebModel(QString uid, QJsonObject * hash, QObject * parent) :
    IModel(hash, parent), IgnoreList(hash), tab_uid(uid)
{
    lastRefresh = QDateTime::currentMSecsSinceEpoch() - 300000;
}

WebModel::~WebModel() {}

bool WebModel::removeRows(int position, int rows, const QModelIndex & parent) {
    FolderItem * parentItem = item<FolderItem>(parent);
    QVariantList uids = parentItem -> childrenUids(position, rows);

    bool res = IModel::removeRows(position, rows, parent);

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
        new FileItem((*it).path(), (*it).fileName(), node);

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
                new FileItem(file.path(), file.fileName(), node, pos);
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
