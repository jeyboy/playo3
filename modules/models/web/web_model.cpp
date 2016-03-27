#include "web_model.h"

using namespace Models;
///////////////////////////////////////////////////////////
bool WebModel::removeRows(int position, int rows, const QModelIndex & parent) {
    Playlist * parentItem = item<Playlist>(parent);
    QStringList uids = parentItem -> childrenUids(position, rows);

    bool res = IModel::removeRows(position, rows, parent);
    if (res)
        ignoreListAddUid(uids);

    return res;
}

QJsonObject WebModel::toJson() {
    QJsonObject res = IModel::toJson();
    return ignoreListToJson(res);
}

int WebModel::filesRoutine(const QString & filePath, Playlist * node) {
    int res = 0;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dt_playlist, dir_it.fileName()));
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        res++;
        QString path = dir_it.next();
        new IItem(node, LOCAL_ITEM_ATTRS(path, dir_it.fileName()));
    }

    if (res > 0)
        node -> updateItemsCountInBranch(res);
    else
        node -> removeYouself();

    return res;
}

int WebModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString path = file.filePath();
        QString name = file.fileName();

        if (file.isDir())
            res += filesRoutine(path, node -> createPlaylist(dt_playlist, name, 0, pos));
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new IItem(node, LOCAL_ITEM_ATTRS(path, name), pos);
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
