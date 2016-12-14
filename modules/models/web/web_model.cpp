#include "web_model.h"

using namespace Models;

void WebModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    ISource * source = Apis::source(sttngs.data_type);
    const char * res_slot = SLOT(proceedJson(QJsonValue &));

    switch(sttngs.rec_type) {
        case rec_obj_info: {
            source -> userInfoAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_set: {
            source -> openSetAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_user_audio: {
            source -> userAudioRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_user_video: {
            source -> userVideoRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_audio: {
            source -> audioRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_video: {
            source -> videoRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_artist: {
            source -> artistRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_artist_audio: {
            source -> audioByArtistAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_artist_video: {
            source -> videoByArtistAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_artist_album: {
            source -> albumByArtistAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_label_audio: {
            source -> audioByLabelAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_label_album: {
            source -> albumByLabelAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_tag_audio: {
            source -> tagsAudioRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        case rec_tag_video: {
            source -> tagsVideoRecommendationsAsync(sttngs.uid, new Func(this, res_slot));
        return;}

        default: {}
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
    QStringList uids = sttngs.isUseIgnoreList() ? parentItem -> childrenUids(position, rows) : QStringList();

    bool res = IModel::removeRows(position, rows, parent);
    if (res && !uids.isEmpty())
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
