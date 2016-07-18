#include <qpair.h>
#include "model_interface.h"

#include "modules/core/media/cue/cue.h"
#include "modules/core/misc/fuzzy_comparison.h"

using namespace Models;
using namespace Core::Web;

bool IModel::restoreUrl(IItem * itm) {
    qDebug() << "RESTORE" << itm -> title();
    QString newUrl = Web::Apis::restoreUrl(itm -> refresh_path(), itm -> dataType(), itm -> mediaType());

    qDebug() << itm -> refresh_path() << newUrl;
    if (!newUrl.isEmpty() && itm -> path().toString() != newUrl) {
        itm -> setPath(newUrl);
        return true;
    }

    return false;
}

IModel::IModel(const Params & settings, QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent), addWatcher(0), sttngs(settings) {
    sync = new QMutex(QMutex::NonRecursive);
    rootItem = hash ? new Playlist(hash, 0, hash -> take(JSON_TYPE_CHILDS)) : new Playlist();
    qDebug() << this << " " << rootItem -> itemsCountInBranch(); // REMOVE ME
}

IModel::~IModel() {
    delete rootItem;
    delete addWatcher;
    delete sync;
}

QVariant IModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    return item(index) -> data(role);
}

bool IModel::setData(const QModelIndex & model_index, const QVariant & value, int role) {
    bool result = false;

    IItem * node = item(model_index);

    if (role == Qt::CheckStateRole) {
        bool checked = !node -> is(IItem::flag_checked);
        node -> updateCheckedState(checked);

        if (node -> isContainer()) {
            Playlist * it = dynamic_cast<Playlist *>(node);
            emit dataChanged(model_index, index(it -> child(it -> childCount() - 1)));
            return true;
        }

        result = true;
    } else if (role == ISTATERESTORE) {
        int iState = value.toInt();
        node -> setStates(iState);
        result = iState != IItem::flag_not_proceeded && iState != IItem::flag_not_mark_on_removing;
    } else if (role == ISTATE) {
        Library::obj().setItemState(model_index, value.toInt());
        node -> setStates(value.toInt());
        result = true;
    } else if (role == Qt::EditRole)
        result = node -> setData(model_index.column(), value);
    else if (role == ISPOILITEM) {
        emit spoilNeeded(model_index);
        result = true;
    } else if (role == IERROR) {
        node -> setError(value);
        result = true;
    }

    if (result)
        emit dataChanged(model_index, model_index);

    return result;
}

QVariant IModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

bool IModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

Qt::ItemFlags IModel::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    Qt::ItemFlags fl = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);


    if (Settings::obj().isCheckboxShow())
        fl |= Qt::ItemIsUserCheckable;

    if (!index.data(IFOLDER).toBool())
        fl |= Qt::ItemNeverHasChildren;

    return fl;
}

QModelIndex IModel::index(int row, int column, const QModelIndex & parent, bool orLastChild) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    Playlist * parentItem = item<Playlist>(parent);
    IItem * childItem = parentItem -> child(row);
    if (orLastChild && !childItem)
        childItem = parentItem -> child(parentItem -> childCount() - 1);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
bool IModel::insertColumns(int position, int columns, const QModelIndex & parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem -> insertColumns(position, columns);
    success = true;
    endInsertColumns();

    return success;
}

//void IModel::appendRow(IItem * item) {
////    int position = parentItem -> childCount();
////    beginInsertRows(index(parentItem), position, position);
//    if (!item -> isFolder()) {
//        emit itemsCountChanged(++count);
//    }
////    endInsertRows();

////    emit dataChanged(parent, parent);
//}

//bool IModel::insertRows(int position, int rows, const QModelIndex & parent) {
//    FolderItem * parentItem = item<FolderItem>(parent);
//    bool success = parentItem != 0;

//    if (success) {
//        beginInsertRows(parent, position, position + rows - 1);
//        success = parentItem -> insertChildren(position, rows, rootItem -> columnCount());
//        endInsertRows();
//    }

//    return success;
//}

QModelIndex IModel::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    IItem * childItem = item(index);
    IItem * parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem -> row(), 0, parentItem);
}

bool IModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem -> removeColumns(position, columns);
    success = true;
    endRemoveColumns();

    if (rootItem -> columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

DropData * IModel::threadlyProcessingRowsInsertion(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    if (list.isEmpty()) return 0;

    emit moveInProcess();
    Logger::obj().startMark();

    DropData * res = new DropData();
    QUrl url;

    switch(playlistType()) {
        case Core::dt_tree:
        case Core::dt_level_tree: { url = list.first(); break; }
        default: url = QUrl();
    }

    recalcParentIndex(parent, pos, res -> eIndex, res -> eRow, url);
    res -> limitRow = res -> eRow + (parent == res -> eIndex ? list.length() - 1 : 0);
    dropProcession(parent, pos, list);
    Logger::obj().endMark("Drop", "new files");

    return res;
}

bool IModel::threadlyInsertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    addWatcher = new QFutureWatcher<DropData *>();
    connect(addWatcher, SIGNAL(finished()), this, SLOT(finishingItemsAdding()));
    addWatcher -> setFuture(QtConcurrent::run(this, &IModel::threadlyProcessingRowsInsertion, list, pos, parent));
    return true;
}

int IModel::proceedList(const DataSubType & wType, const QJsonValue & json, Playlist * parent, const DataMediaType & dmtype) {
    int (IModel::*proc_func)(const QJsonArray &, Playlist *, const DataMediaType &, const DataSubType &);

    switch(wType) {
        case dt_site_vk: { proc_func = &IModel::proceedVkList; break;}
        case dt_site_sc: { proc_func = &IModel::proceedScList; break;}
        case dt_site_od: { proc_func = &IModel::proceedOdList; break;}
        case dt_site_yandex: { proc_func = &IModel::proceedYandexList; break;}
        case dt_site_youtube: { proc_func = &IModel::proceedYoutubeList; break;}
        default: proc_func = &IModel::proceedGrabberList;
    }

    if (json.isObject()) {
        QJsonObject obj = json.toObject();
        int amount = 0;

        for(QJsonObject::Iterator pair = obj.begin(); pair != obj.end(); pair++)
            amount += (*this.*proc_func)(
                pair.value().toArray(),
                parent,
                (DataMediaType)pair.key().toInt(),
                wType
            );

        return amount;
    }
    else return (*this.*proc_func)(json.toArray(), parent, dmtype, wType);
}

int IModel::proceedVkList(const QJsonArray & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & /*wType*/) {
    if (collection.isEmpty()) return 0;
    DataMediaType dm_type = dmtype;
    int itemsAmount = 0;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    int pos = parent -> playlistsAmount();
    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        if (itm.contains(tkn_media_type))
            dm_type = (DataMediaType)itm.value(tkn_media_type).toInt();

        QString id = QString::number(itm.value(Vk::tkn_id).toInt());
        QString owner = QString::number(itm.value(Vk::tkn_owner_id).toInt());
        QString uid = IItem::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        QVariant uri;

        if (dm_type == dmt_video) {
            QString player_url = itm.value(Vk::tkn_player).toString();

            if (!player_url.startsWith(QStringLiteral("http://vk.com")))
                uri = player_url;
            else
                uri = QString(); // store only embeded videos
        } else {
            uri = Vk::Queries::cleanUrl(itm.value(Vk::tkn_url).toString()); // remove extra info from url
        }

        QList<IItem *> items = store.values(uid);

        if (items.isEmpty()) {
            itemsAmount++;
            IItem * newItem = new IItem(parent, VK_ITEM_ATTRS(
                id, uri,
                QString(itm.value(Vk::tkn_artist).toString() % QStringLiteral(" - ") % itm.value(Vk::tkn_title).toString()),
                owner, uid,
                Duration::fromSeconds(itm.value(Vk::tkn_duration).toInt(0)),
                dm_type
            ), pos);

            if (dm_type == dmt_video)
                newItem -> setArtPath(itm.value(Vk::tkn_video_art).toString());

//                if (itm.contains(Vk::genre_id_key))
//                    newItem -> setGenre(VkGenres::instance() -> toStandartId(itm.value(Vk::genre_id_key).toInt()));
        } else {
            for(QList<IItem *>::Iterator it_it = items.begin(); it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }

        pos++;
    }

    return itemsAmount;
}

int IModel::proceedYandexList(const QJsonArray & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & /*wType*/) {
//    QJsonValue("album":{"artists":[],"available":true,"availableForPremiumUsers":true,"cover":1,"coverUri":"avatars.yandex.net/get-music-content/410d1df6.a.2327834-1/%%","genre":"rap","id":2327834,"originalReleaseYear":2014,"recent":false,"storageDir":"410d1df6.a.2327834","title":"Still Rich","trackCount":23,"veryImportant":false,"year":2014},"albums":[{"artists":[],"available":true,"availableForPremiumUsers":true,"cover":1,"coverUri":"avatars.yandex.net/get-music-content/410d1df6.a.2327834-1/%%","genre":"rap","id":2327834,"originalReleaseYear":2014,"recent":false,"storageDir":"410d1df6.a.2327834","title":"Still Rich","trackCount":23,"veryImportant":false,"year":2014}],"artists":[{"composer":false,"cover":{"prefix":"3c84dd0a.a.705443/1.","type":"from-album-cover","uri":"avatars.yandex.net/get-music-content/3c84dd0a.a.705443-1/%%"},"decomposed":[],"id":999162,"name":"Chief Keef","various":false}],"available":true,"durationMillis":201830,"durationMs":201830,"explicit":false,"id":20454067,"regions":["UKRAINE","UKRAINE_MOBILE_PREMIUM"],"storageDir":"11916_1b93d8e3.20454067","title":"Sosa")

    if (collection.isEmpty()) return 0;
    DataMediaType dm_type = dmtype;
    int itemsAmount = 0;

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (itm.contains(tkn_media_type))
            dm_type = (DataMediaType)itm.value(tkn_media_type).toInt();

        QStringList artistUids;
        QString artistStr;
        QJsonArray artistsAr = itm.value(Yandex::tkn_artists).toArray();
        for(QJsonArray::Iterator artist = artistsAr.begin(); artist != artistsAr.end(); artist++) {
            QJsonObject obj = (*artist).toObject();
            artistUids << QString::number(obj.value(Yandex::tkn_id).toInt());
            artistStr = artistStr % (artistStr.isEmpty() ? QString() : QStringLiteral(" & ")) % obj.value(Yandex::tkn_name).toString();
        };

        QJsonObject album = itm.value(Yandex::tkn_album).toObject();
        QString genre = album.value(Yandex::tkn_genre).toString();
        QString album_id = QString::number(album.value(Yandex::tkn_id).toInt());
        QString id = QString::number(itm.value(Yandex::tkn_id).toInt()) % QStringLiteral(":") % album_id;

        itemsAmount++;
        IItem * newItem = new IItem(parent, YANDEX_ITEM_ATTRS(id,
            QString(artistStr % QStringLiteral(" - ") % itm.value(Yandex::tkn_title).toString()),
            id,
            Duration::fromMillis(itm.value(Yandex::tkn_durationMs).toInt(0)),
            dm_type
        ));

        //newItem -> setGenre(genre); // need to convert genre to genre id
        if (itm.contains(Yandex::tkn_fileSize))
            newItem -> setSize(itm.value(Yandex::tkn_fileSize).toInt());
    }

    return itemsAmount;
}

int IModel::proceedYoutubeList(const QJsonArray & collection, Playlist * parent, const DataMediaType & /*dmtype*/, const DataSubType & /*wType*/) {
    if (collection.isEmpty()) return 0;
    int itemsAmount = 0;

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        QJsonObject snippet = itm.value(QStringLiteral("snippet")).toObject();
        QJsonValue idVal = itm.value(QStringLiteral("id"));
        QString id = idVal.isString() ? idVal.toString() : idVal.toObject().value(QStringLiteral("videoId")).toString();

        itemsAmount++;
        IItem * newItem = new IItem(parent, YOUTUBE_ITEM_ATTRS(id,
            snippet.value(QStringLiteral("title")).toString(),
            id
        ));

        //snippet.value(QStringLiteral("thumbnails")).toObject().value(QStringLiteral("default")).toObject().value(QStringLiteral("url")); // "medium" // "high"


        snippet = itm.value(QStringLiteral("contentDetails")).toObject();
//        "contentDetails": {
//            "duration": "PT3M48S",
//            "dimension": "2d",
//            "definition": "hd",
//            "caption": "false",
//            "licensedContent": true,
//            "regionRestriction": {
//             "blocked": [
//              "DE"
//             ]
//            }
//           }
        if (!snippet.isEmpty()) {
            qint64 durMillis = Duration::ISO8601StrtoMillis(snippet.value(QStringLiteral("duration")).toString());
            if (durMillis > 1200000) // 20 min
                newItem -> setError(ItemErrors::warn_not_permitted);
            newItem -> setDuration(Duration::fromMillis(durMillis));
        }
    }

    return itemsAmount;
}

int IModel::proceedGrabberList(const QJsonArray & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & wType) {
    if (collection.isEmpty()) return 0;
    DataMediaType dm_type = dmtype;
    int itemsAmount = 0;

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        if (itm.contains(tkn_media_type))
            dm_type = (DataMediaType)itm.value(tkn_media_type).toInt();

        if (itm.contains(tkn_grab_is_set)) {
            if (itm.contains(tkn_grab_set_items)) {
                Playlist * playlist = parent -> createPlaylist(wType, itm.value(tkn_grab_id).toString(), itm.value(tkn_grab_title).toString());
                int taked_amount = proceedGrabberList(itm.value(tkn_grab_set_items).toArray(), playlist, dm_type, wType);
                itemsAmount += taked_amount;
                playlist -> updateItemsCountInBranch(taked_amount);
            } else {
                parent -> createLoadablePlaylist(
                    {
                        {tkn_media_type, dm_type},
                        {JSON_TYPE_ITEM_TYPE, wType},
                        {tkn_grab_refresh, itm.value(tkn_grab_refresh).toString()},
                        {tkn_grab_set_parser, itm.value(tkn_grab_set_parser).toInt()}
                    },
                    itm.value(tkn_grab_title).toString(),
                    itm.value(tkn_grab_id).toString()
                );

                itemsAmount++;
            }
        } else {
            QString id = QString::number(itm.value(tkn_grab_id).toInt());
            QString uri = itm.value(tkn_grab_url).toString();
            QString refresh_url = itm.value(tkn_grab_refresh).toString();

            itemsAmount++;
            IItem * newItem = new IItem(parent, WEB_ITEM_ATTRS(id, uri,
                itm.value(tkn_grab_title).toString(),
                wType, refresh_url,
                itm.value(tkn_grab_extension).toString(val_def_extension),
                dm_type
            ));

            if (itm.contains(tkn_grab_duration)) {
                if (itm.value(tkn_grab_duration).isDouble())
                    newItem -> setDuration(Duration::fromMillis(itm.value(tkn_grab_duration).toInt(0)));
                else
                    newItem -> setDuration(itm.value(tkn_grab_duration));
            }

            if (itm.contains(tkn_grab_genre_id))
                newItem -> setGenre(itm.value(tkn_grab_genre_id).toInt());

            if (itm.contains(tkn_grab_bpm))
                newItem -> setBpm(itm.value(tkn_grab_bpm).toInt());

            if (itm.contains(tkn_grab_size))
                newItem -> setSize(Info::fromUnits(itm.value(tkn_grab_size).toString()));

            if (!itm.contains(tkn_skip_info))
                newItem -> setInfo(Info::str(
                        itm.value(tkn_grab_size).toString("?"),
                        newItem -> extension().toString(),
                        itm.value(tkn_grab_bitrate).toString("?"),
                        itm.value(tkn_grab_discretion_rate).toString("?"),
                        itm.value(tkn_grab_channels).toString("?")
                    )
                );
        }
    }

    return itemsAmount;
}

int IModel::proceedScList(const QJsonArray & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & /*wType*/) {
    if (collection.isEmpty()) return 0;
    DataMediaType dm_type = dmtype;
    int itemsAmount = 0;

    QList<IItem *> items;
    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (itm.contains(tkn_media_type))
            dm_type = (DataMediaType)itm.value(tkn_media_type).toInt();

        bool original = true;
        QString id = QString::number(itm.value(Soundcloud::tkn_id).toInt());
        QString owner = QString::number(itm.value(Soundcloud::tkn_user_id).toInt());
        QString uid = IItem::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        QString uri = itm.value(Soundcloud::tkn_download_url).toString();
        if (uri.isEmpty()) {
            uri = itm.value(Soundcloud::tkn_stream_url).toString();
            original = false;
        }

        items = store.values(uid);

        if (items.isEmpty()) {
            itemsAmount++;
            IItem * newItem = new IItem(
                parent,
                SC_ITEM_ATTRS(id, uri,
                    itm.value(Soundcloud::tkn_title).toString(),
                    owner,
                    Duration::fromMillis(itm.value(Soundcloud::tkn_duration).toInt(0)),
                    original ? itm.value(Soundcloud::tkn_original_format).toString() : val_def_extension,
                    dm_type
                )
            );

//            Genre::instance() -> toInt(fileIterObj.value("genre").toString())

//            if (itm.contains(Soundcloud::tkn_video_url))
//                newItem -> setVideoPath(itm.value(Soundcloud::tkn_video_url).toString());

            if (itm.contains(Soundcloud::tkn_genre_id))
                newItem -> setGenre(itm.value(Soundcloud::tkn_genre_id).toInt());
        } else {
            QList<IItem *>::Iterator it_it = items.begin();

            for(; it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }
    }

    return itemsAmount;
}

int IModel::proceedOdList(const QJsonArray & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & /*wType*/) {
    // {"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}
    // {"albumId":-544493822,"duration":340,"ensemble":"Unity Power feat. Rozlyne Clarke","id":51059525931389,"imageUrl":"http://mid.odnoklassniki.ru/getImage?photoId=144184&type=2","masterArtistId":-1332246915,"name":"Eddy Steady Go (House Vocal Attack)","size":11004741}

    if (collection.isEmpty()) return 0;
    DataMediaType dm_type = dmtype;
    int itemsAmount = 0;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (itm.contains(tkn_media_type))
            dm_type = (DataMediaType)itm.value(tkn_media_type).toInt();

        qint64 iid = ((qint64)itm.value(Od::tkn_id).toDouble());
        QString id = QString::number(iid);
        if (iid == 0 || ignoreListContainUid(id)) continue;

        QList<IItem *> items = store.values(id);

        if (items.isEmpty()) {
            itemsAmount++;
            new IItem(parent, OD_ITEM_ATTRS(
                id,
                QString(itm.value(Od::tkn_ensemble).toString() % Od::tkn_dash % itm.value(Od::tkn_name).toString()),
                id,
                Duration::fromSeconds(itm.value(Od::tkn_duration).toInt(0)),
                itm.value(Od::tkn_size).toInt(0),
                dm_type
            ));
        } else {
            for(QList<IItem *>::Iterator it_it = items.begin(); it_it != items.end(); it_it++)
                (*it_it) -> setRefreshPath(id);
        }
    }

    return itemsAmount;
}

int IModel::proceedCue(const QString & path, const QString & name, Playlist * newParent, int insertPos, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) {
    Playlist * cuePlaylist = new Playlist(LOCAL_CONTAINER_ATTRS(dt_playlist_cue, path, name), newParent, insertPos);

    Media::Cue * cue = Media::Cue::fromPath(path);
    int amount = cue -> buildItems(cuePlaylist, unproc_files, items);
    cuePlaylist -> updateItemsCountInBranch(amount);
    delete cue;
    return amount;
}

bool IModel::insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    if (list.isEmpty()) return false;

    QModelIndex exIndex;
    int exRow;

    recalcParentIndex(parent, pos, exIndex, exRow, list.first());
    beginInsertRows(exIndex, exRow, exRow + (parent == exIndex ? list.length() - 1 : 0));
    dropProcession(parent, pos, list);
    endInsertRows();
    emit spoilNeeded(parent);
    return true;
}

bool IModel::removeRows(int position, int rows, const QModelIndex & parent) {
    Playlist * parentItem = item<Playlist>(parent);
    bool success = parentItem != 0;
    int deleted;

    if (success) {
        if (parentItem -> parent() && position == 0 && parentItem -> childCount() == rows) { // remove empty parents for current deletion
            rows = 1;

            while(true) {
                position = parentItem -> parent() -> childRow(parentItem);
                parentItem = parentItem -> parent();

                if (!parentItem -> parent() || parentItem -> childCount() != 1) break;
            }

            (const_cast<QModelIndex &>(parent)) = index(parentItem);
        }

        beginRemoveRows(parent, position, position + rows - 1);
        bool blockSignal = signalsBlocked();
        blockSignals(true);
        deleted = parentItem -> removeChildren(position, rows);
        blockSignals(blockSignal);
        endRemoveRows();

        success = deleted != 1;

        if (deleted < 0)
            emit itemsCountChanged(deleted);
    }

    return success;
}

int IModel::rowCount(const QModelIndex & parent) const {
    //        if (parent.column() > 0)
    //            return 0;

    Playlist * parentItem = item<Playlist>(parent);
    return parentItem ? parentItem -> childCount() : 0;
}

void IModel::shuffle() {
    beginResetModel();
    rootItem -> shuffle();
    endResetModel();
}

//    bool Model::removeRow(int row, const QModelIndex &parentIndex) {
//        int removeCount = 1;
//        ModelItem * parentItem = item(parentIndex);
//        ModelItem * item = parentItem -> child(row);
//        QString folderName;
//        bool isUnprocessed = item -> isFolder();
//        bool res;

//        if (isUnprocessed) {
//            folderName = item -> data(TITLEID).toString();

//            removeCount = item -> childTreeCount();
//        }

//        beginRemoveRows(parentIndex, row, row);
//        res = parentItem -> removeChildren(row, 1);
//        endRemoveRows();

//        if (res) {
//            if (isUnprocessed) {
//                parentItem -> removeFolder(folderName);
//            }

//            if (removeCount > 0)
//                emit itemsCountChanged(count -= removeCount);
//        }

//        return res;
//    }

//    bool Model::removeRows(int position, int rows, const QModelIndex &parent) {
//        ModelItem *parentItem = item(parent);
//        bool success = true;

//        beginRemoveRows(parent, position, position + rows - 1);
//        success = parentItem -> removeChildren(position, rows);
//        endRemoveRows();

//        if (success)
//            emit itemsCountChanged((count -= rows));

//        return success;
//    }

//    void Model::clearAll(bool refresh) {
//        if(hasChildren()) {
//            emit updated();
//            removeRows(0, rowCount());
//            count = 0;
//            root() -> foldersList() -> clear();
//        }

//        if (refresh)
//            this -> refresh();
//    }


//    // no idea how to realize this correctly
//    void Model::libraryResponse() {
//        QFutureWatcher<ModelItem *> * obj = (QFutureWatcher<ModelItem *> *)sender();

//        if (obj -> result() != 0)
//            refreshItem(obj -> result());

//        obj -> deleteLater();
//    }

//    void Model::refresh() {
//        beginResetModel();
//        endResetModel();
//    //    emit itemsCountChanged(count);
//    }

//    void Model::refreshItem(ModelItem * item) {
//        qDebug() << item -> fullPath();
//        QModelIndex ind = index(item);
//        if (ind.isValid()) {
//    //        emit dataChanged(ind.parent(), ind);
//            emit dataChanged(ind, ind);
//        }
//    }

//    // remove temp item - folder
//    void Model::removeFolderPrebuild(ModelItem * temp) {
//        temp -> parent() -> removeFolder(temp -> data(TITLEID).toString());
//        temp -> parent() -> removeChildren(temp -> row(), 1);
//    }

//////////////////////// slots //////////////////////////

void IModel::copyTitleToClipboard(const QModelIndex & index) {
    if (index.isValid())
        QApplication::clipboard() -> setText(index.data().toString());
}

void IModel::copyIdsToClipboard(const QModelIndexList & indexes) {
    QString ret;

    for(QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); it++) {
        IItem * itm = item(*it);
        if (itm -> isShareable()) {
            QVariant uid = itm -> toUid();
            if (uid.isValid())
                ret = ret % QStringLiteral(" ") % QString::number(itm -> dataType()) % SHARE_DELIMITER % uid.toString();
        }
    }

    if (!ret.isEmpty())
        ret = ret.mid(1);

    QApplication::clipboard() -> setText(ret);
}

void IModel::importIds(const QStringList & ids) {
    emit moveInBackgroundProcess();
    QHash<int, QStringList> uidsMap;

    for(QStringList::ConstIterator it = ids.constBegin(); it != ids.constEnd(); it++) {
        QStringList parts = (*it).split(SHARE_DELIMITER);
        uidsMap[parts.first().toInt()].append(parts.last());
    }

    Playlist * parentNode = 0;
    bool is_new = false;

    switch(playlistType()) {
        case dt_search:
        case dt_site_sc:
        case dt_site_vk:
        case dt_level: { parentNode = rootItem; break; }
        case dt_tree:
        case dt_level_tree: {
            QFileInfo file = QFileInfo(REMOTE_DND_URL.toLocalFile());
            QString path = file.path();
            if (path.isEmpty()) path = Extensions::folderName(file);
            parentNode = rootItem -> playlist(path);
            is_new = !parentNode;
            if (is_new) parentNode = rootItem -> createPlaylist(dt_playlist, path);
        break;}
        default:;
    }

    if (!parentNode) qDebug() << "UNDEF FOLDER";

    if (is_new) {
        int from = parentNode -> row();
        beginInsertRows(index(parentNode -> parent()), from, from + ids.size() - 1);
    } else {
        int from = parentNode -> childCount();
        beginInsertRows(index(parentNode), from, from + ids.size() - 1);
    }

    for(QHash<int, QStringList>::Iterator map_it = uidsMap.begin(); map_it != uidsMap.end(); map_it++) {
        ISource * source = Web::Apis::source((DataSubType)map_it.key());

        if (source -> isShareable()) {
            source -> connectUser(); // check connection and ask user to connect if it not

            if (source -> isConnected()) {
                QJsonValue obj = source -> itemsInfo(map_it.value());
                proceedList(source -> siteType(), obj, parentNode);
            }
//            else // some actions
        }
        else qDebug() << "UNSUPPORTED EXPORT TYPE";
    }

    endInsertRows();
    emit moveOutBackgroundProcess();
}

void IModel::markAllAsChecked() {
    rootItem -> updateCheckedState(true);
    emit dataChanged(QModelIndex(), index(rootItem -> child(rootItem -> childCount() - 1)));
}
void IModel::markAllAsUnchecked() {
    rootItem -> updateCheckedState(false);
    emit dataChanged(QModelIndex(), index(rootItem -> child(rootItem -> childCount() - 1)));
}

//void IModel::removeChecked() {
//    beginResetModel();
//    //TODO: realisation needed
//    endResetModel();
//}



void IModel::expandeAll() {
    rootItem -> propagatePlaylistSetFlag(IItem::flag_expanded);
}

void IModel::expanded(const QModelIndex & index) {
    IItem * node = item(index);
    node -> set(IItem::flag_expanded);
}

void IModel::collapseAll() {
    rootItem -> propagatePlaylistUnsetFlag(IItem::flag_expanded);
}

void IModel::collapsed(const QModelIndex & index) {
    IItem * node = item(index);
    node -> unset(IItem::flag_expanded);
}

void IModel::finishingItemsAdding() {
    DropData * res = addWatcher -> result();
    delete addWatcher;
    addWatcher = 0;
    if (!res) return;


    beginInsertRows(res -> eIndex, res -> eRow, res -> limitRow);
    endInsertRows();

    emit moveOutProcess();
    emit spoilNeeded(res -> eIndex);
    delete res;
}

void IModel::finishSetLoading(QJsonValue & json, void * _playlist) {
    Playlist * playlist = (Playlist *)_playlist;

    QVariantMap hash = playlist -> loadableAttrs().toMap();
    DataSubType data_type = (DataSubType)hash.value(JSON_TYPE_ITEM_TYPE).toInt();
    DataMediaType media_type = (DataMediaType)hash.value(tkn_media_type).toInt();
    playlist -> unset(IItem::flag_in_proc);
    playlist -> removeLoadability();

    IItem * temp_item = playlist -> child(0);
    int added = proceedList(data_type, json, playlist, media_type);

    if (added > 0) {
        temp_item -> removeYouself();
        playlist -> backPropagateItemsCountInBranch(added);

        beginInsertRows(index(playlist), 0, added);
        endInsertRows();
    } else {
        temp_item -> setTitle(QStringLiteral("Error!!"));
    }
}

/////////////////////////////////////////////////////////

void IModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & /*url*/) {
    exIndex = dIndex;
    exRow = dRow < 0 ? 0 : dRow;
}

QModelIndex IModel::fromPath(QString path, Direction direction) {
    QStringList parts = path.split(' ', QString::SkipEmptyParts);

    if (parts.isEmpty())
        return QModelIndex();

    Playlist * curr = rootItem;

    while(parts.length() > 1) {
        curr = dynamic_cast<Playlist *>(curr -> child(parts.takeFirst().toInt()));
        if (curr == 0) return QModelIndex(); // while not fixed correct played item removing
    }

    int ind = parts.takeFirst().toInt();
    IItem * res = curr -> child(ind);

    if (!res)
        switch (direction) {
            case forward:
                while(!res && curr -> parent()) {
                    res = curr -> parent() -> child(curr -> row() + 1);
                    curr = curr -> parent();
                }

                break;
            case backward:
                res = curr -> parent();
                break;
            default: ;
        }

    return res ? index(res) : QModelIndex();
}

Qt::DropActions IModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList IModel::mimeTypes() const {
    QStringList types;
    types << DROP_INNER_FORMAT;
    types << DROP_OUTER_FORMAT;
    return types;
}

bool IModel::proceedSelfDnd(int row, int /*column*/, const QModelIndex & parent) { // not tested
    QModelIndex eIndex, dIndex, dropIndex;
    int eRow, dRow, totalAdded = 0;
    bool has_free_moving = hasFreeMoving();

    Playlist * globParent = item<Playlist>(parent);
    dRow = globParent -> childCount() - 1;
    if (row == -1 || row >= dRow + 1) {
        dropIndex = QModelIndex();
        row = dRow;
    }
    else dropIndex = index(globParent -> child(row));

    QHash<Playlist *, QList<IItem *> > moveItems;
    QHash<Playlist *, Playlist * > links;
    QHash<IItem *, int> insertPos;

    for(QModelIndex ind : dndList) {
        if (ind == dropIndex)
            continue;

        IItem * itm = item(ind);
        dIndex = parent; dRow = row;
        recalcParentIndex(dIndex, dRow, eIndex, eRow, itm -> isRemote() || has_free_moving || !parent.isValid() ? QUrl() : itm -> toUrl());

        if (dRow == -1) continue;

        Playlist * parentFolder = item<Playlist>(dIndex);
        if (parentFolder == itm) {
            parentFolder = itm -> parent();
            eIndex = index(parentFolder);
        }

        moveItems[parentFolder].append(itm);
        if (eIndex != dIndex)
            links.insert(parentFolder, item<Playlist>(eIndex));
        insertPos.insert(itm, dRow);
    }

    for(QHash<Playlist *, QList<IItem *> >::Iterator position = moveItems.begin(); position != moveItems.end(); position++) {
        Playlist * newParentItm = links.value(position.key(), position.key());
        QModelIndex newParent = index(newParentItm);
        int listTotal = 0;

        QList<IItem *> items_list = position.value();
        qSort(items_list.begin(), items_list.end(), itemsComparator());

        for(IItem * itm : items_list) {
            int itm_row = itm -> row(), insert_row = insertPos[itm];
            bool same_parent = itm -> parent() == newParentItm;

            if (same_parent && itm_row == insert_row) continue;
            if (same_parent && (itm_row + 1 == insert_row)) insert_row += 1;

            beginMoveRows(index(itm -> parent()), itm_row, itm_row, newParent, insert_row);
                if (!same_parent) {
                    itm -> setParent(newParentItm, insertPos[itm]);
                    listTotal += (itm -> isContainer()) ? ((Playlist *)itm) -> childCount() : 1;
                }
                else newParentItm -> moveChild(itm, insertPos[itm]);
            endMoveRows();
        }

        newParentItm -> backPropagateItemsCountInBranch(listTotal);
        totalAdded += listTotal;
    }

    if (totalAdded > 0) emit itemsCountChanged(totalAdded);

    dndList.clear();
    return true;
}

bool IModel::decodeInnerData(int row, int /*column*/, const QModelIndex & parent, QDataStream & stream) {
    int totalAdded = 0;
    QModelIndex dIndex;
    InnerData * data;
    Playlist * parentFolder;
    QHash<Playlist *, int> counts;
    bool containPath, isRemote, requirePath = !isRelative()/*, free_drop*/;

//    switch(playlistType()) {
//        case Data::Type::tree:
//        case Data::Type::level_tree: { free_drop = false; break; }
//        default: free_drop = true;
//    }

    while (!stream.atEnd()) {
        data = new InnerData();
        stream >> data -> url >> isRemote >> data -> attrs;

        if (!isRemote) {
            containPath = data -> attrs.contains(JSON_TYPE_PATH);

            if (requirePath) {
                if (!containPath)
                    data -> attrs.insert(JSON_TYPE_PATH, data -> url.toLocalFile().section('/', 0, -2));

                data -> url = QUrl();
            } else {
                if (containPath)
                    data -> attrs.remove(JSON_TYPE_PATH);
            }
        }
        else {
            data -> url = /*free_drop ?*/ QUrl()/* : REMOTE_DND_URL*/;
        }

        data -> dRow = row;
        dIndex = parent;

        recalcParentIndex(dIndex, data -> dRow, data -> eIndex, data -> eRow, data -> url);

        parentFolder = item<Playlist>(dIndex);

        beginInsertRows(data -> eIndex, data -> eRow, data -> eRow);
            new IItem(parentFolder, data -> attrs, data -> dRow);
//            Playlist::restoreItem(data -> attrs.take(JSON_TYPE_ITEM_TYPE).toInt(), parentFolder, data -> dRow, data -> attrs);
            counts[parentFolder] ++;
        endInsertRows();
        delete data;
    }

    QHash<Playlist *, int>::Iterator it = counts.begin();

    for(; it != counts.end(); it++) {
        it.key() -> backPropagateItemsCountInBranch(it.value());
        totalAdded += it.value();
    }

    if (totalAdded > 0) emit itemsCountChanged(totalAdded);

    emit spoilNeeded(parent);
    return true;
}

QMimeData * IModel::mimeData(const QModelIndexList & indexes) const {
    if (indexes.isEmpty())
        return 0;

    QMimeData * mimeData = new QMimeData();
    QHash<QUrl, int> list;

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);

    QModelIndexList::ConstIterator it = indexes.begin();
    for (; it != indexes.end(); ++it)
        item((*it)) -> packToStream(list, stream);

    mimeData -> setData(DROP_INNER_FORMAT, encoded);
    mimeData -> setUrls(list.keys());

    return mimeData;
}

//Если бросили точно на ноду - в row и column -1 а index - нода.
//Если выше ноды - index - родитель ноды а row = нода.row.
//Если ниже ноды - index - родитель ноды а row = нода.row + 1.
//Ежели бросили мимо - index = root.
bool IModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parentIndex) {
    if (!data || !(action == Qt::CopyAction || action == Qt::MoveAction))
        return false;

    if (row == -1/* && !parentIndex.data(IFOLDER).toBool()*/) {
        row = parentIndex.row();
        (const_cast<QModelIndex &>(parentIndex)) = parentIndex.parent();
    }

    int row_count = rowCount(parentIndex);
    if (row > row_count) row = -1;

    if (dropKeyModifiers & Qt::ControlModifier)
        Dialogs::ExtensionDialog(QApplication::activeWindow()).exec();

    if (data -> hasFormat(DROP_INNER_FORMAT)) {
        if (!dndList.isEmpty()) {
            return proceedSelfDnd(row, column, parentIndex);
        } else {
            QByteArray encoded = data -> data(DROP_INNER_FORMAT);
            QDataStream stream(&encoded, QIODevice::ReadOnly);
            return decodeInnerData(row, column, parentIndex, stream);
        }
    } else if (data -> hasUrls())
        return /*insertRows*/threadlyInsertRows(data -> urls(), row, parentIndex);

    return false;
}

int IModel::search(SearchLimit & params, Playlist * destination, Playlist * search_source) {
    int amount = 0;

    if (search_source == 0)
        search_source = rootItem;

    QList<IItem *> child = search_source -> childrenList();
    bool has_empty_predicate = params.predicate.isEmpty();

    for(QList<IItem *>::Iterator it = child.begin(); it != child.end(); it++) {
        if ((*it) -> isContainer()) {
            search(params, destination, (Playlist *) *it);
        } else {
            bool is_valid = has_empty_predicate || (*it) -> title().toString().contains(params.predicate, Qt::CaseInsensitive);

            if (is_valid) {
                if (!params.genre.isEmpty()) {
//                    int genre_id = (*it) -> genreID().toInt();
//                    is_valid |= params.sgenre_id == genre_id;
                }

                if (is_valid) {
                    QVariantHash attrs = (*it) -> toInnerAttrs();
                    if (!(*it) -> isRemote() && !attrs.contains(JSON_TYPE_PATH))
                        attrs.insert(JSON_TYPE_PATH, (*it) -> toUrl().toLocalFile().section('/', 0, -2));

                    new IItem(destination, attrs);
//                    Playlist::restoreItem(attrs.take(JSON_TYPE_ITEM_TYPE).toInt(), destination, -1, attrs);
                    amount ++;
                }
            }
        }
    }

    return amount;
}

int IModel::search(const QString & predicate, Playlist * destination, Playlist * search_source, int count) {
    int amount = 0;

    if (search_source == 0)
        search_source = rootItem;

    QList<IItem *> child = search_source -> childrenList();

    for(QList<IItem *>::Iterator it = child.begin(); it != child.end(); it++) {
        if ((*it) -> isContainer()) {
            search(predicate, destination, (Playlist *) *it, count);
        } else {
            int comparity = FuzzyComparison::compareStrings((*it) -> title().toString(), predicate);
            qDebug() << "COMPATIBILITY CHECK" << predicate << " (VS) " << (*it) -> title().toString() << " ||| " << comparity;
            bool is_valid = comparity >= 80;

            if (is_valid) {
                QVariantHash attrs = (*it) -> toInnerAttrs();
                if (!(*it) -> isRemote() && !attrs.contains(JSON_TYPE_PATH))
                    attrs.insert(JSON_TYPE_PATH, (*it) -> toUrl().toLocalFile().section('/', 0, -2));

                new IItem(destination, attrs);
                if (++amount == count) break;
            }
        }
    }

    return amount;
}
