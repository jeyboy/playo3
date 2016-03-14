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

int WebModel::filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;
    QString cue_ext = QStringLiteral(".cue");

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dir_it.fileName()), unproc_files, items);
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        DataItem * item = DataCore::obj().dataItem(path);
        if (item) continue;

        if (name.endsWith(cue_ext, Qt::CaseInsensitive))
            res += proceedCue(path, name, node, -1, unproc_files, items);
        else {
            QString _title = name, ext;
            Extensions::obj().extractExtension(_title, ext);

            REGISTER_LOCAL_DATA(path, _title, ext);
            new IItem(path, node);
            res++;
        }
    }

    if (res > 0)
        node -> updateItemsCountInBranch(res);
    else
        node -> removeYouself();

    return res;
}

int WebModel::filesRoutine(const QList<QUrl> & list, Playlist * node, int pos) {
    int res = 0;
    QString cue_ext = QStringLiteral("cue");
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString && path = file.filePath();
        QString && name = file.fileName();

        if (file.isDir())
            res += filesRoutine(path, node -> createPlaylist(name, 0, pos), unproc_files, items);
        else if (Extensions::obj().respondToExtension(file.suffix())) {
            DataItem * item = DataCore::obj().dataItem(path);
            if (item) continue;

            if (file.suffix().endsWith(cue_ext, Qt::CaseInsensitive))
                res += proceedCue(path, name, node, pos, unproc_files, items);
            else {
                QString _title = name, ext;
                Extensions::obj().extractExtension(_title, ext);

                REGISTER_LOCAL_DATA(path, _title, ext);
                new IItem(path, node, pos);
                res++;
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
