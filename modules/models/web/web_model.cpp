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

void WebModel::toJson(QJsonObject & res) {
    IModel::toJson(res);
    ignoreListToJson(res);
}

int WebModel::filesRoutine(const QString & filePath, Playlist * node) {
    int res = 0;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dir_it.fileName()));
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        res++;
        QString path = dir_it.next();
        new File(path, dir_it.fileName(), node);
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
        if (file.isDir())
            res += filesRoutine(file.filePath(), node -> createPlaylist(file.fileName(), 0, pos));
        else {
            if (Extensions::obj().respondToExtension(file.suffix())) {
                res++;
                new File(file.filePath(), file.fileName(), node, pos);
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
