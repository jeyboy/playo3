#include "web_model.h"

using namespace Models;

void WebModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    switch(sttngs.type) { //TODO: move this to virt funcs in isource
        case dt_web_vk: {
            switch(sttngs.rec_type) {
                case rec_none: {
                    Vk::Queries::obj().userInfoAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_set: {
                    Vk::Queries::obj().openSetAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_user: {
                    Vk::Queries::obj().userRecommendationsAsync(
                        sttngs.uid,
                        true,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_song: {
                    Vk::Queries::obj().trackRecommendationsAsync(
                        sttngs.uid,
                        true,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                default: {}
            }
        break;}

        case dt_web_sc: {
            switch(sttngs.rec_type) {
                case rec_none: {
                    Soundcloud::Queries::obj().objectInfoAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_song: {
                    Soundcloud::Queries::obj().trackRecommendationsAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_set: {
                    Soundcloud::Queries::obj().openSetAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_user: {
                    Soundcloud::Queries::obj().userRecommendationsAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                default: {}
            }
        break;}

        case dt_web_od: {
            switch(sttngs.rec_type) {
                case rec_none: {
                    Od::Queries::obj().userInfoAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_set: {
                    Od::Queries::obj().openSetAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_user: {
                    Od::Queries::obj().userRecommendationsAsync(
//                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                default: {}
            }
        break;}

        case dt_web_fourshared: {
            switch(sttngs.rec_type) {
                case rec_none: {
                    Fourshared::Queries::obj().userInfoAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                case rec_set: {
                    Fourshared::Queries::obj().openSetAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

                default: {}
            }
        break;}

        case dt_web_yandex: {
            switch(sttngs.rec_type) {
    //            case rec_none: {
    //                Yandex::Queries::obj().objectInfoAsync(
    //                    sttngs.uid,
    //                    new Func(this, SLOT(proceedJson(QJsonValue &)))
    //                );
    //            return;}

    //            case rec_song: {
    //                Yandex::Queries::obj().trackRecommendationsAsync(
    //                    sttngs.uid,
    //                    new Func(this, SLOT(proceedJson(QJsonValue &)))
    //                );
    //            return;}

                case rec_set: {
                    Yandex::Queries::obj().openSetAsync(
                        sttngs.uid,
                        new Func(this, SLOT(proceedJson(QJsonValue &)))
                    );
                return;}

    //            case rec_user: {
    //                Yandex::Queries::obj().userRecommendationsAsync(
    //                    sttngs.uid,
    //                    new Func(this, SLOT(proceedJson(QJsonValue &)))
    //                );
    //            return;}

                default: {}
            }
        break;}

        default:;
    }

    emit moveOutProcess();
}

void WebModel::proceedJson(QJsonValue & hash) {
    proceedBlocks(hash.toArray(), root_item);
    emit moveOutProcess();
}

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

int WebModel::filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    int res = 0;

    {
        QDirIterator dir_it(filePath, (QDir::Filter)(FOLDER_FILTERS));
        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            res += filesRoutine(path, node -> createPlaylist(dt_playlist, dir_it.fileName()), unproc_files, items);
        }
    }

    QDirIterator dir_it(filePath, Extensions::obj().activeFilterList(), (QDir::Filter)(FILE_FILTERS));
    while(dir_it.hasNext()) {
        QString path = dir_it.next();
        QString name = dir_it.fileName();

        if (!unproc_files.contains(path)) {
            if (name.endsWith(Extensions::dot_cue_ext, Qt::CaseInsensitive))
                res += proceedCue(path, name, node, -1, unproc_files, items);
            else {
                res++;
                new IItem(node, LOCAL_ITEM_ATTRS(path, name));
            }
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
    QHash<QString, bool> unproc_files;
    QHash<QString, IItem *> items;

    for(QList<QUrl>::ConstIterator it = list.begin(); it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        QString path = file.filePath();
        QString name = file.fileName();

        if (file.isDir())
            res += filesRoutine(path, node -> createPlaylist(dt_playlist, name, 0, pos), unproc_files, items);
        else {
            if (unproc_files.contains(path)) continue;
            if (Extensions::obj().respondToExtension(file.suffix())) {
                if (file.suffix().endsWith(Extensions::cue_ext, Qt::CaseInsensitive))
                    res += proceedCue(path, name, node, pos, unproc_files, items);
                else {
                    res++;
                    new IItem(node, LOCAL_ITEM_ATTRS(path, name), pos);
                }
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
