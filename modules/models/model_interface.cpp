#include <qpair.h>
#include "model_interface.h"

#include "modules/core/media/cue/cue.h"
#include "modules/core/misc/fuzzy_comparison.h"
#include "dialogs/extension_dialog.h"
#include "dockbars.h"

using namespace Models;
using namespace Core::Web;

bool IModel::restoreUrl(IItem * itm) {
    qDebug() << "RESTORE" << itm -> title();
    QString newUrl = Web::Apis::restoreUrl(itm -> refresh_path(), itm -> dataType(), itm -> dataMediaType());

    qDebug() << itm -> refresh_path() << newUrl;
    if (!newUrl.isEmpty() && newUrl != OPERATION_BLOCKED && itm -> path().toString() != newUrl) {
        itm -> setPath(newUrl);
        return true;
    }

    return false;
}

IModel::IModel(const Params & settings, QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent), block_fetching(false), add_watcher(0), sttngs(settings) {
    sync = new QMutex(QMutex::NonRecursive);
    root_item = hash ? new Playlist(hash, 0, hash -> take(JSON_TYPE_CHILDS)) : new Playlist();
    qDebug() << this << " " << root_item -> itemsCountInBranch(); // REMOVE ME
}

IModel::~IModel() {
    delete root_item;
    delete add_watcher;
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
        return root_item -> data(section);

    return QVariant();
}

bool IModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = root_item -> setData(section, value);

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

    IItem * child_item = item(index);
    IItem * parent_item = child_item -> parent();

    if (!parent_item || parent_item == root_item)
        return QModelIndex();

    return createIndex(parent_item -> row(), 0, parent_item);
}

bool IModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem -> removeColumns(position, columns);
    success = true;
    endRemoveColumns();

    if (root_item -> columnCount() == 0)
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

    ///////////// for spoiling /////////////
    Playlist * playlist = item<Playlist>(parent);
    IItem * it = pos < 0 ? playlist -> lastChild() : playlist -> child(pos);
    ///////////////////////////////////////

    dropProcession(parent, pos, list);

    ///////////// for spoiling /////////////
    if (it)
        emit spoilNeeded(index(it));
    ///////////////////////////////////////

    Logger::obj().endMark("Drop", "new files");

    return res;
}

bool IModel::threadlyInsertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    add_watcher = new QFutureWatcher<DropData *>();
    connect(add_watcher, SIGNAL(finished()), this, SLOT(finishingItemsAdding()));
    add_watcher -> setFuture(QtConcurrent::run(this, &IModel::threadlyProcessingRowsInsertion, list, pos, parent));
    return true;
}

int IModel::proceedBlocks(const QJsonArray & blocks, Playlist * parent) {
    int (IModel::*proc_func)(const QJsonObject &, Playlist *, int &, QHash<Playlist *, QHash<QString, IItem *> > & , const DataMediaType &, const DataSubType &);
    int (IModel::*proc_set_func)(const QJsonObject &, Playlist *, int &, QHash<Playlist *, QHash<QString, IItem *> > &, const DataMediaType &, const DataSubType &);

    int start_index = parent -> childCount(), items_amount = 0, update_amount = 0, block_amount;

    QHash<Playlist *, QHash<QString, IItem *> > stores;
    parent -> accumulateUids(stores[parent]);

    for(QJsonArray::ConstIterator block = blocks.constBegin(); block != blocks.constEnd(); block++) {
        QJsonObject block_obj = (*block).toObject();

        if (JSON_BOOL(block_obj, tkn_is_empty) && !JSON_HAS_KEY(block_obj, tkn_loadable_cmd)) continue;

        QJsonObject first_item = EXTRACT_ITEMS(block_obj).first().toObject();
        if (JSON_HAS_KEY(first_item, tkn_error)) {
            qCritical() << "BLOCK WITH ERR:" << JSON_STR(first_item, tkn_error);
            continue;
        }

        Playlist * curr_parent = parent;       

        DataMediaType dmt_type = (DataMediaType)block_obj.value(tkn_media_type).toInt(); //(DataMediaType)EXTRACT_MEDIA_TYPE(pair.key().toInt());
        DataSubType wType = (DataSubType)block_obj.value(tkn_source_id).toInt();

        // if used other playlist - should updating (beginInsertRows) for it and add + 1 to parent :(
        if (block_obj.contains(tkn_dir_name)) {
            // we can use there building of subtree if needed
            QString name = block_obj.value(tkn_dir_name).toString();
            curr_parent = parent -> createPlaylist(wType, name);

            if (!stores.contains(curr_parent))
                curr_parent -> accumulateUids(stores[curr_parent]);

            update_amount++;
        }

        if (dmt_type & dmt_dir) { // INFO: construction of item, with more than second level deepness // only yandex use this at this moment
            if (JSON_HAS_KEY(block_obj, tkn_more_cmd))
                curr_parent -> setFetchableAttrs(JSON_STR(block_obj, tkn_more_cmd));

            proceedBlocks(EXTRACT_ITEMS(block_obj), curr_parent);
        } else {
            switch(wType) {
                case dt_web_vk: {
                    proc_func = &IModel::proceedVkList;
                    proc_set_func = &IModel::proceedVkList;
                break;}
                case dt_web_sc: {
                    proc_func = &IModel::proceedScList;
                    proc_set_func = &IModel::proceedScSet;
                break;}
                case dt_web_od: {
                    proc_func = &IModel::proceedOdList;
                    proc_set_func = &IModel::proceedOdSet;
                break;}
                case dt_web_yandex: {
                    proc_func = &IModel::proceedYandexList;
                    proc_set_func = &IModel::proceedYandexSet;
                break;}
                case dt_web_youtube: {
                    proc_func = &IModel::proceedYoutubeList;
                    proc_set_func = 0;
                break;}
                default:
                    proc_func = &IModel::proceedGrabberList;
                    proc_set_func = &IModel::proceedGrabberList;
            }

            if (dmt_type & dmt_any || dmt_type & dmt_set)
                block_amount = (*this.*proc_set_func)(block_obj, curr_parent, update_amount, stores, dmt_type, wType);
            else
                block_amount = (*this.*proc_func)(block_obj, curr_parent, update_amount, stores, dmt_type, wType);

            items_amount += block_amount;

            if (curr_parent == parent)
                update_amount += block_amount;
        }
    }

    if (update_amount > 0) {
        beginInsertRows(parent == root_item ? QModelIndex() : index(parent), start_index, start_index + (update_amount - 1));
        parent -> updateItemsCountInBranch(items_amount);
        endInsertRows();
    }

    updateIcon();

    return items_amount;
}

int IModel::proceedVkList(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    int items_amount = 0;
    bool is_collapsed = parent != root_item;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));   

//    int pos = parent -> playlistsAmount();
//    for(QJsonArray::ConstIterator it = collection.constEnd(); it-- != collection.constBegin();) {
    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        DataMediaType curr_media_type = fdmtype;

        if (JSON_HAS_KEY(itm, tkn_media_type))
            curr_media_type = (DataMediaType)JSON_INT(itm, tkn_media_type);

        DataMediaType clear_media_type = EXTRACT_MEDIA_TYPE(curr_media_type);

        if (curr_media_type & dmt_set) {
            Playlist * folder;
            QString playlist_id = JSON_CSTR(itm, Vk::tkn_id);
            QString title = JSON_STR(itm, Vk::tkn_title);

            if (JSON_HAS_KEY(itm, tkn_loadable_cmd)) {
                if (parent -> createLoadablePlaylist(
                    folder,
                    JSON_STR(itm, tkn_loadable_cmd),
                    title,
                    playlist_id
                ))
                    update_amount++;
            } else {
                QJsonArray playlist_items = JSON_ARR(itm, Vk::tkn_items);

                if (playlist_items.size() > 0) {
                    if (parent -> createPlaylist(
                        folder,
                        wType,
                        playlist_id,
                        title/*,
                        pos*/
                    ))
                        update_amount++;

                    int sub_update_amount = 0;
                    folder -> accumulateUids(stores[folder]);
                    int amount = proceedVkList(
                        QJsonObject {{ tkn_content, playlist_items}},
                        folder, sub_update_amount, stores, clear_media_type
                    );
                    folder -> updateItemsCountInBranch(amount);
                    items_amount += amount;
                }
            }

            if (JSON_HAS_KEY(itm, tkn_more_cmd))
                folder -> setFetchableAttrs(JSON_STR(itm, tkn_more_cmd));

            if (is_collapsed)
                folder -> setStates(IItem::flag_not_expanded);
        } else {
            QString owner = JSON_CSTR(itm, Vk::tkn_owner_id);
            QString id = ID_TOKEN(owner, JSON_CSTR(itm, Vk::tkn_id), '_');
            QString uid = UID_CAT_EXT(wType, clear_media_type, id);

            if (ignoreListContainUid(uid)) continue;

            QVariant uri;

            if (clear_media_type == dmt_video) {
                QString player_url = JSON_STR(itm, Vk::tkn_player);

                if (QUrl(player_url).host().contains(LSTR("vk.com"), Qt::CaseInsensitive))
                    uri = player_url;
                else
                    uri = QString(); // store only embeded videos // not playable at this time
            } else {
                uri = Vk::Queries::cleanUrl(JSON_STR(itm, Vk::tkn_url)); // remove extra info from url
            }

            QList<IItem *> items = stores[parent].values(uid);

            if (items.isEmpty()) {
                items_amount++;
                IItem * new_item = new IItem(parent, VK_ITEM_ATTRS(
                    id, uri,
                    JSON_HAS_KEY(itm, Vk::tkn_artist) ? JSON_STR_CAT(itm, Vk::tkn_artist, tkn_dash, Vk::tkn_title) : JSON_STR(itm, Vk::tkn_title),
                    owner, id,
                    Duration::fromSeconds(JSON_INT(itm, Vk::tkn_duration)),
                    clear_media_type
                )/*, pos*/);

                stores[parent].insert(uid, new_item);

                if (clear_media_type == dmt_video)
                    new_item -> setArtPath(JSON_STR(itm, Vk::tkn_video_art));

    //                if (itm.contains(Vk::genre_id_key))
    //                    new_item -> setGenre(VkGenres::instance() -> toStandartId(itm.value(Vk::genre_id_key).toInt()));
            } else {
                for(QList<IItem *>::Iterator it_it = items.begin(); it_it != items.end(); it_it++)
                    (*it_it) -> setPath(uri);
            }

    //        pos++;
        }
    }

    return items_amount;
}
//int IModel::proceedVkSet(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
//    QJsonArray collection = EXTRACT_ITEMS(block);

//    if (collection.isEmpty()) return 0;
//    int items_amount = 0;

//    DataMediaType dmt_type = EXTRACT_MEDIA_TYPE(fdmtype);
//    bool is_collapsed = parent != root_item;
////    int pos = 0;

//    if (JSON_HAS_KEY(block, tkn_more_cmd))
//        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

//    for(QJsonArray::Iterator playlist_obj = collection.begin(); playlist_obj != collection.end(); playlist_obj++/*, pos++*/) {
//        QJsonObject playlist = (*playlist_obj).toObject();
//        Playlist * folder;

//        QString playlist_id = JSON_CSTR(playlist, Vk::tkn_id);
//        QString title = JSON_STR(playlist, Vk::tkn_title);

//        if (JSON_HAS_KEY(playlist, tkn_loadable_cmd)) {
//            if (parent -> createLoadablePlaylist(
//                folder,
//                JSON_STR(playlist, tkn_loadable_cmd),
//                title,
//                playlist_id
//            ))
//                update_amount++;
//        } else {
//            QJsonArray playlist_items = JSON_ARR(playlist, Vk::tkn_items);

//            if (playlist_items.size() > 0) {
//                if (parent -> createPlaylist(
//                    folder,
//                    wType,
//                    playlist_id,
//                    title/*,
//                    pos*/
//                ))
//                    update_amount++;

//                int sub_update_amount = 0;
//                folder -> accumulateUids(stores[folder]);
//                int amount = proceedVkList(
//                    QJsonObject {{ tkn_content, playlist_items}},
//                    folder, sub_update_amount, stores, dmt_type
//                );
//                folder -> updateItemsCountInBranch(amount);
//                items_amount += amount;
//            }
//        }

//        if (JSON_HAS_KEY(playlist, tkn_more_cmd))
//            folder -> setFetchableAttrs(JSON_STR(playlist, tkn_more_cmd));

//        if (is_collapsed)
//            folder -> setStates(IItem::flag_not_expanded);
//    }

//    return items_amount;
//}

int IModel::proceedScList(const QJsonObject & block, Playlist * parent, int & /*update_amount*/, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    DataMediaType dm_type = EXTRACT_MEDIA_TYPE(fdmtype);
    int itemsAmount = 0;

    QList<IItem *> items;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    QString uid_prefix = UID_CAT_EXT(wType, dm_type, QString());

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (JSON_HAS_KEY(itm, tkn_media_type))
            dm_type = (DataMediaType)JSON_INT(itm, tkn_media_type);

        bool original = true;
        QString owner = JSON_CSTR(itm, Soundcloud::tkn_user_id);
        QString refresh_id = JSON_CSTR(itm, Soundcloud::tkn_id);
        QString id = ID_TOKEN(owner, refresh_id, '_');
        QString uid = uid_prefix % id;

        if (ignoreListContainUid(uid)) continue;

        QString uri = JSON_STR(itm, Soundcloud::tkn_download_url);
        if (uri.isEmpty()) {
            uri = JSON_STR(itm, Soundcloud::tkn_stream_url);
            original = false;
        }

        items = stores[parent].values(uid);

        if (items.isEmpty()) {
            itemsAmount++;
            IItem * new_item = new IItem(
                parent,
                SC_ITEM_ATTRS(id, refresh_id, uri,
                    JSON_STR(itm, Soundcloud::tkn_title),
                    owner,
                    Duration::fromMillis(JSON_INT(itm, Soundcloud::tkn_duration)),
                    original ? JSON_STR(itm, Soundcloud::tkn_original_format) : val_def_extension,
                    dm_type
                )
            );

            stores[parent].insert(uid, new_item);

//            Genre::instance() -> toInt(fileIterObj.value("genre").toString())

//            if (itm.contains(Soundcloud::tkn_video_url))
//                new_item -> setVideoPath(itm.value(Soundcloud::tkn_video_url).toString());

            if (JSON_HAS_KEY(itm, Soundcloud::tkn_genre_id))
                new_item -> setGenreId(JSON_INT(itm, Soundcloud::tkn_genre_id));
        } else {
            QList<IItem *>::Iterator it_it = items.begin();

            for(; it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }
    }

    return itemsAmount;
}
int IModel::proceedScSet(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;
    int items_amount = 0;

    DataMediaType dmt_type = EXTRACT_MEDIA_TYPE(fdmtype);
    bool is_collapsed = parent != root_item;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        QJsonObject playlist = (*it).toObject();
        Playlist * folder;

        QString playlist_id = JSON_CSTR(playlist, Soundcloud::tkn_id);
        QString title = JSON_STR(playlist, Soundcloud::tkn_title);

        if (playlist.contains(tkn_loadable_cmd)) {
             if (parent -> createLoadablePlaylist(
                folder,
                JSON_STR(playlist, tkn_loadable_cmd),
                title,
                playlist_id
             ))
                update_amount++;
        } else {
            QJsonArray playlist_items = JSON_ARR(playlist, Soundcloud::tkn_tracks);
            if (!playlist_items.isEmpty()) {
                if (parent -> createPlaylist(folder, wType, playlist_id, title))
                    update_amount++;

                int sub_update_amount = 0;
                folder -> accumulateUids(stores[folder]);
                int amount = proceedScList(
                    QJsonObject {{tkn_content, playlist_items}},
                    folder, sub_update_amount, stores, dmt_type
                );
                folder -> updateItemsCountInBranch(amount);
                items_amount += amount;
            }
        }

        if (JSON_HAS_KEY(playlist, tkn_more_cmd))
            folder -> setFetchableAttrs(JSON_STR(playlist, tkn_more_cmd));

        if (is_collapsed)
            folder -> setStates(IItem::flag_not_expanded);
    }

    return items_amount;
}

int IModel::proceedOdList(const QJsonObject & block, Playlist * parent, int & /*update_amount*/, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    // {"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}
    // {"albumId":-544493822,"duration":340,"ensemble":"Unity Power feat. Rozlyne Clarke","id":51059525931389,"imageUrl":"http://mid.odnoklassniki.ru/getImage?photoId=144184&type=2","masterArtistId":-1332246915,"name":"Eddy Steady Go (House Vocal Attack)","size":11004741}

    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    DataMediaType dm_type = EXTRACT_MEDIA_TYPE(fdmtype);
    int items_amount = 0;

    QString uid_prefix = UID_CAT_EXT(wType, dm_type, QString());

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (JSON_HAS_KEY(itm, tkn_media_type))
            dm_type = (DataMediaType)JSON_INT(itm, tkn_media_type);

        QString id = JSON_CSTR(itm, Od::tkn_id);
        QString uid = uid_prefix % id;
        if ((id.length() == 1 && id[0] == '0') || ignoreListContainUid(uid)) continue;

        QList<IItem *> items = stores[parent].values(uid);

        if (items.isEmpty()) {
            QString title =
                dm_type == dmt_audio ?
                    JSON_STR_CAT(itm, Od::tkn_ensemble, tkn_dash, Od::tkn_name) :
                    JSON_STR(itm, Od::tkn_name);

            items_amount++;
            IItem * new_item = new IItem(parent, OD_ITEM_ATTRS(
                id,
                title,
                id,
                Duration::fromSeconds(JSON_INT(itm, Od::tkn_duration)),
                JSON_INT(itm, Od::tkn_size),
                dm_type
            ));

            stores[parent].insert(uid, new_item);
        } else {
            for(QList<IItem *>::Iterator it_it = items.begin(); it_it != items.end(); it_it++)
                (*it_it) -> setRefreshPath(id);
        }
    }

    if (items_amount > 5 && JSON_HAS_KEY(block, tkn_more_cmd)) // od returns to many duplicates - checking on new items amount is prevent from infinite looping
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    return items_amount;
}
int IModel::proceedOdSet(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;
    int items_amount = 0;

    DataMediaType dmt_type = EXTRACT_MEDIA_TYPE(fdmtype);
    bool is_collapsed = parent != root_item;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        QJsonObject playlist = (*it).toObject();

        int amount = JSON_INT(playlist, Od::tkn_count);
        bool is_loadable = JSON_HAS_KEY(playlist, tkn_loadable_cmd);

        if (is_loadable || amount > 0) {
            QString pid = JSON_CSTR(playlist, Od::tkn_id);
            QString name = JSON_STR(playlist, Od::tkn_name);
            Playlist * folder;

            if (is_loadable) {
                if (parent -> createLoadablePlaylist(
                    folder,
                    JSON_STR(playlist, tkn_loadable_cmd),
                    name, pid
                ))
                    update_amount++;
            } else {
                if (parent -> createPlaylist(folder, wType, pid, name))
                    update_amount++;

                QJsonObject items_block = Od::Queries::obj().audioByPlaylist(pid).toObject();
                int sub_update_amount = 0;
                folder -> accumulateUids(stores[folder]);
                int amount = proceedOdList(items_block, folder, sub_update_amount, stores, dmt_type);
                folder -> updateItemsCountInBranch(amount);
                items_amount += amount;
            }

            if (JSON_HAS_KEY(playlist, tkn_more_cmd))
                folder -> setFetchableAttrs(JSON_STR(playlist, tkn_more_cmd));

            folder -> setOwnerId(JSON_CSTR(playlist, Od::tkn_owner));

            if (is_collapsed)
                folder -> setStates(IItem::flag_not_expanded);
        }
    }

    return items_amount;
}

int IModel::proceedYandexList(const QJsonObject & block, Playlist * parent, int & /*update_amount*/, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    DataMediaType dm_type = EXTRACT_MEDIA_TYPE(fdmtype);
    int items_amount = 0;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    QString uid_prefix = UID_CAT_EXT(wType, dm_type, QString());

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();
        if (itm.isEmpty()) continue;

        if (JSON_HAS_KEY(itm, tkn_media_type))
            dm_type = (DataMediaType)JSON_INT(itm, tkn_media_type);

        QVariantMap artists;
        QString artistStr;
        QJsonArray artists_arr = JSON_ARR(itm, Yandex::tkn_artists);
        for(QJsonArray::Iterator artist = artists_arr.begin(); artist != artists_arr.end(); artist++) {
            QJsonObject obj = (*artist).toObject();
            QString name = JSON_STR(obj, Yandex::tkn_name);

            artists.insert(JSON_CSTR(obj, Yandex::tkn_id), name);
            artistStr = artistStr % (artistStr.isEmpty() ? QString() : LSTR(" & ")) % name;
        };

        QVariantMap albums;
        QJsonArray albums_arr = JSON_HAS_KEY(itm, Yandex::tkn_album) ? (QJsonArray() << JSON_OBJ(itm, Yandex::tkn_album)) : JSON_ARR(itm, Yandex::tkn_albums);
        for(QJsonArray::Iterator album = albums_arr.begin(); album != albums_arr.end(); album++) {
            QJsonObject obj = (*album).toObject();

            albums.insert(JSON_CSTR(obj, Yandex::tkn_id), JSON_STR(obj, Yandex::tkn_name));
        }

//        QString genre = album.value(Yandex::tkn_genre).toString();
        QString album_id = JSON_CSTR(albums_arr.first().toObject(), Yandex::tkn_id);
        QString id = YANDEX_ITEM_UID_ALB(itm, album_id); // JSON_CSTR(itm, Yandex::tkn_id) % ':' % album_id;
        QString uid = uid_prefix % id;

        QList<IItem *> items = stores[parent].values(uid);

        if (items.isEmpty()) {
            QString title = QString(artistStr % tkn_dash % JSON_STR(itm, Yandex::tkn_title));

            QString version = JSON_STR(itm, LSTR("version"));
            if (!version.isEmpty())
                title = title % ' ' % '(' % version % ')';

            items_amount++;
            IItem * new_item = new IItem(parent, YANDEX_ITEM_ATTRS(id,
                title,
                id,
                Duration::fromMillis(JSON_INT(itm, Yandex::tkn_duration_ms)),
                dm_type
            ));

            stores[parent].insert(uid, new_item);

            new_item -> setArtists(artists);
            new_item -> setAlbums(albums);

            //new_item -> setGenre(genre); // need to convert genre to genre id
            if (JSON_HAS_KEY(itm, Yandex::tkn_fileSize))
                new_item -> setSize(JSON_INT(itm, Yandex::tkn_fileSize));
        }
    }

    return items_amount;
}
int IModel::proceedYandexSet(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & /*stores*/, const DataMediaType & /*fdmtype*/, const DataSubType & /*wType*/) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;
    int items_amount = 0;

//    DataMediaType dmt_type = EXTRACT_MEDIA_TYPE(fdmtype);
    bool is_collapsed = parent != root_item;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        QJsonObject playlist = (*it).toObject();

//        int amount = JSON_INT(playlist, Od::tkn_count);
        bool is_loadable = JSON_HAS_KEY(playlist, tkn_loadable_cmd);

        QString pid = JSON_CSTR(playlist, Yandex::tkn_id);
        QString name = JSON_STR(playlist, JSON_HAS_KEY(playlist, Yandex::tkn_full_title) ? Yandex::tkn_full_title : Yandex::tkn_title);
        Playlist * folder;

        if (is_loadable) {
            if (parent -> createLoadablePlaylist(
                folder,
                JSON_STR(playlist, tkn_loadable_cmd),
                name, pid
            ))
                update_amount++;
        } else {
            continue;
//            if (parent -> createPlaylist(folder, wType, pid, name))
//                update_amount++;

//            QJsonObject items_block = Yandex::Queries::obj().audioByPlaylist(pid).toObject();
//            int sub_update_amount = 0;
//            int amount = proceedOdList(items_block, folder, sub_update_amount, dmt_type);
//            folder -> updateItemsCountInBranch(amount);
//            items_amount += amount;
        }

        if (JSON_HAS_KEY(playlist, tkn_more_cmd))
            folder -> setFetchableAttrs(JSON_STR(playlist, tkn_more_cmd));

//        folder -> setOwner(JSON_CSTR(playlist, Od::tkn_owner));
        folder -> setArtPath(JSON_STR(playlist, Yandex::tkn_coverUri));

        if (is_collapsed)
            folder -> setStates(IItem::flag_not_expanded);
    }

    return items_amount;
}


int IModel::proceedYoutubeList(const QJsonObject & block, Playlist * parent, int & /*update_amount*/, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    int items_amount = 0;
    DataMediaType dm_type = EXTRACT_MEDIA_TYPE(fdmtype);

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    QString uid_prefix = UID_CAT_EXT(wType, dm_type, QString());

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        QJsonObject snippet = JSON_OBJ(itm, LSTR("snippet"));
        QJsonValue idVal = JSON_VAL(itm, LSTR("id"));
        QString id = idVal.isString() ? idVal.toString() : JSON_CSTR(idVal.toObject(), LSTR("videoId"));
        QString uid = uid_prefix % id;

        QList<IItem *> items = stores[parent].values(uid);

        if (items.isEmpty()) {
            items_amount++;
            IItem * new_item = new IItem(parent, YOUTUBE_ITEM_ATTRS(id,
                JSON_STR(snippet, LSTR("title")),
                id
            ));

            stores[parent].insert(uid, new_item);

            //snippet.value(LSTR("thumbnails")).toObject().value(LSTR("default")).toObject().value(LSTR("url")); // "medium" // "high"


            snippet = JSON_OBJ(itm, LSTR("contentDetails"));
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
                qint64 durMillis = Duration::ISO8601StrtoMillis(JSON_CSTR(snippet, LSTR("duration")));
                if (durMillis > 1200000) // 20 min
                    new_item -> setError(ItemErrors::warn_not_permitted);
                new_item -> setDuration(Duration::fromMillis(durMillis));
            }
        }
    }

    return items_amount;
}

int IModel::proceedGrabberList(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType) {
    QJsonArray collection = EXTRACT_ITEMS(block);
    if (collection.isEmpty()) return 0;

    DataMediaType dm_type = EXTRACT_MEDIA_TYPE(fdmtype);
    bool is_collapsed = parent != root_item;
    int items_amount = 0;

    if (JSON_HAS_KEY(block, tkn_more_cmd))
        parent -> setFetchableAttrs(JSON_STR(block, tkn_more_cmd));

    for(QJsonArray::ConstIterator it = collection.constBegin(); it != collection.constEnd(); it++) {
        QJsonObject itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        if (JSON_HAS_KEY(itm, tkn_media_type))
            dm_type = (DataMediaType)JSON_INT(itm, tkn_media_type);

        if (dm_type & dmt_set) {
            QString pid = JSON_STR(itm, tkn_grab_id);
            QString ptitle = JSON_STR(itm, tkn_grab_title);
            Playlist * folder;

            if (JSON_HAS_KEY(itm, tkn_loadable_cmd)) {
                if (parent -> createLoadablePlaylist(
                    folder,
                    JSON_STR(itm, tkn_loadable_cmd),
                    ptitle,
                    pid
                ))
                    update_amount++;
            } else {
                if (parent -> createPlaylist(folder, wType, pid, ptitle))
                    update_amount++;

                int sub_update_amount = 0;
                folder -> accumulateUids(stores[folder]);
                int taked_amount = proceedGrabberList(
                    QJsonObject {{tkn_content, itm.value(tkn_content)}},
                    folder, sub_update_amount, stores, EXTRACT_MEDIA_TYPE(dm_type), wType
                );

                if (JSON_HAS_KEY(itm, tkn_more_cmd))
                    folder -> setFetchableAttrs(JSON_STR(itm, tkn_more_cmd));

                items_amount += taked_amount;
                folder -> updateItemsCountInBranch(taked_amount);
            }

            if (is_collapsed)
                folder -> setStates(IItem::flag_not_expanded);
        } else {
            QString id = JSON_CSTR(itm, tkn_grab_id);
            QString uid = UID_CAT_EXT(wType, dm_type, id);

            QList<IItem *> items = stores[parent].values(uid);

            if (items.isEmpty()) {
                QString uri = JSON_STR(itm, tkn_grab_url);
                QString refresh_url = JSON_STR(itm, tkn_grab_refresh);

                items_amount++;
                IItem * new_item = new IItem(parent, WEB_ITEM_ATTRS(id, uri,
                    JSON_STR(itm, tkn_grab_title),
                    wType, refresh_url,
                    JSON_STR_DEF(itm, tkn_grab_extension, val_def_extension),
                    dm_type
                ));

                stores[parent].insert(uid, new_item);

                //CHECKME: some shit written here
                if (JSON_HAS_KEY(itm, tkn_grab_duration)) {
                    if (JSON_VAL(itm, tkn_grab_duration).isDouble())
                        new_item -> setDuration(Duration::fromMillis(JSON_INT(itm, tkn_grab_duration)));
                    else
                        new_item -> setDuration(JSON_VAL(itm, tkn_grab_duration));
                }

                if (JSON_HAS_KEY(itm, tkn_grab_genre_id))
                    new_item -> setGenreId(JSON_INT(itm, tkn_grab_genre_id));

                if (JSON_HAS_KEY(itm, tkn_grab_bpm))
                    new_item -> setBpm(JSON_INT(itm, tkn_grab_bpm));

                if (JSON_HAS_KEY(itm, tkn_grab_size))
                    new_item -> setSize(Info::fromUnits(JSON_CSTR(itm, tkn_grab_size)));

                if (!JSON_HAS_KEY(itm, tkn_skip_info))
                    new_item -> setInfo(Info::str(
                            JSON_STR_DEF(itm, tkn_grab_size, LSTR("?")),
                            new_item -> extension().toString(),
                            JSON_STR_DEF(itm, tkn_grab_bitrate, LSTR("?")),
                            JSON_STR_DEF(itm, tkn_grab_discretion_rate, LSTR("?")),
                            JSON_STR_DEF(itm, tkn_grab_channels, LSTR("?"))
                        )
                    );
            }
        }
    }

    return items_amount;
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

bool IModel::removeRows(int position, int amount, const QModelIndex & parent) {
    Playlist * parentItem = item<Playlist>(parent);
    bool success = parentItem != 0;
    int deleted;

    if (success) {
        if (parentItem -> parent() && position == 0 && parentItem -> childCount() == amount) { // remove empty parents for current deletion
            amount = 1;

            while(true) {
                position = parentItem -> parent() -> childRow(parentItem);
                parentItem = parentItem -> parent();

                if (!parentItem -> parent() || parentItem -> childCount() != 1) break;
            }

            (const_cast<QModelIndex &>(parent)) = index(parentItem);
        }

        beginRemoveRows(parent, position, position + amount - 1);
        bool blockSignal = signalsBlocked();
        blockSignals(true);
        deleted = parentItem -> removeChildren(position, amount, &deleted_items);
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
    root_item -> shuffle();
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
            QVariant uid = itm -> id();
            if (uid.isValid())
                ret = ret % ' ' % UID_CAT(itm, uid.toString());
        }
    }

    if (!ret.isEmpty())
        ret = ret.mid(1);

    QApplication::clipboard() -> setText(ret);
}

void IModel::importIds(const QStringList & ids) { //TODO: fix me
    emit moveInBackgroundProcess();
    QHash<int, QHash<int, QStringList> > uids_map;

    for(QStringList::ConstIterator it = ids.constBegin(); it != ids.constEnd(); it++) {
        QStringList parts = (*it).split(SHARE_DELIMITER);
        if (parts.length() != 3)
            qCritical() << "importIds" << "wrong id" << (*it);
        else
            uids_map[parts[0].toInt()][parts[1].toInt()] << parts[2];
    }

    Playlist * parentNode = root_item;
    bool is_new = false;

    if (!hasFreeMoving()) {
        QFileInfo file = QFileInfo(REMOTE_DND_URL.toLocalFile());
        QString path = file.path();
        if (path.isEmpty()) path = Extensions::folderName(file);
        parentNode = root_item -> playlist(path);
        is_new = !parentNode;
        if (is_new) parentNode = root_item -> createPlaylist(dt_playlist, path);
    }

//    switch(playlistType()) {
//        case dt_search:
//        case dt_site_sc:
//        case dt_site_vk:
//        case dt_level: { parentNode = rootItem; break; }
//        case dt_tree:
//        case dt_level_tree: {
//            QFileInfo file = QFileInfo(REMOTE_DND_URL.toLocalFile());
//            QString path = file.path();
//            if (path.isEmpty()) path = Extensions::folderName(file);
//            parentNode = rootItem -> playlist(path);
//            is_new = !parentNode;
//            if (is_new) parentNode = rootItem -> createPlaylist(dt_playlist, path);
//        break;}
//        default:;
//    }

//    if (!parentNode) {
//        qDebug() << "UNDEF FOLDER";
//        return;
//    }

    QJsonArray blocks;

    for(QHash<int, QHash<int, QStringList> >::Iterator source_it = uids_map.begin(); source_it != uids_map.end(); source_it++) {
        ISource * source = Web::Apis::source((DataSubType)source_it.key());

        if (source -> isShareable()) {
            if (source -> permissions(sf_is_shareable, true) > 0) {
                QHash<int, QStringList> map = source_it.value();
                for(QHash<int, QStringList>::Iterator map_it = map.begin(); map_it != map.end(); map_it++)
                    blocks << source -> itemsInfo(map_it.value(), (DataMediaType)map_it.key());
            }
        }
        else qDebug() << "UNSUPPORTED EXPORT TYPE";
    }

    if (!blocks.isEmpty()) {
        if (is_new) { // registering new folder node // a little dirty
            int from = parentNode -> row();
            beginInsertRows(index(parentNode -> parent()), from, from);
            endInsertRows();
        }

        proceedBlocks(blocks, parentNode);
    }

    emit moveOutBackgroundProcess();
}

void IModel::markAllAsChecked() {
    root_item -> updateCheckedState(true);
    emit dataChanged(QModelIndex(), index(root_item -> lastChild()));
}
void IModel::markAllAsUnchecked() {
    root_item -> updateCheckedState(false);
    emit dataChanged(QModelIndex(), index(root_item -> lastChild()));
}

//void IModel::removeChecked() {
//    beginResetModel();
//    //TODO: realisation needed
//    endResetModel();
//}



void IModel::expandeAll() {
    root_item -> propagatePlaylistSetFlag(IItem::flag_expanded);
}

void IModel::expanded(const QModelIndex & index) {
    IItem * node = item(index);
    node -> set(IItem::flag_expanded);
}

void IModel::collapseAll() {
    root_item -> propagatePlaylistUnsetFlag(IItem::flag_expanded);
}

void IModel::collapsed(const QModelIndex & index) {
    IItem * node = item(index);
    node -> unset(IItem::flag_expanded);
}

void IModel::finishingItemsAdding() {
    DropData * res = add_watcher -> result();
    delete add_watcher;
    add_watcher = 0;
    if (!res) return;


    beginInsertRows(res -> eIndex, res -> eRow, res -> limitRow);
    endInsertRows();

    emit moveOutProcess();
    emit spoilNeeded(res -> eIndex);
    delete res;
}

void IModel::finishSetLoading(const QJsonValue & json, void * params) {
    QPair<Playlist *, QString> * pars = (QPair<Playlist *, QString> *)params;
    Playlist * playlist = pars -> first;
    QString cmd = pars -> second;

    delete pars;

    if (!deleted_items.containsItem(playlist)) {
        IItem * temp_item = playlist -> child(0);
        QJsonArray blocks = json.isArray() ? json.toArray() : QJsonArray() << json.toObject();
        QString err_str = EXTRACT_ITEMS(blocks.first().toObject()).first().toObject().value(tkn_error).toString();
        bool no_errors = err_str.isEmpty();
        QModelIndex pind = index(playlist);

        if (no_errors) {
            playlist -> removeLoader(cmd); // remove old loader before blocks proc

            proceedBlocks(blocks, playlist);

            if (temp_item -> dataType() == dt_dummy) {

                QModelIndex res_pind = pind;
                removeRows(temp_item -> row(), 1, res_pind);
                if (pind != res_pind)
                    playlist = 0;
            }

            if (playlist && !playlist -> is(IItem::flag_expanded))
                emit expandNeeded(pind);
        } else {
            if (temp_item -> dataType() == dt_dummy) {
                temp_item -> setTitle(LSTR("Error: %1").arg(err_str));
                emit dataChanged(pind, pind);
            }
        }

        if (playlist)
            playlist -> setStates(IItem::flag_not_in_proc);
    }

    emit moveOutBackgroundProcess();
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

    Playlist * curr = root_item;

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

    for(QModelIndex ind : dnd_list) {
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

    dnd_list.clear();
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

    if (drop_key_modifiers & Qt::ControlModifier)
        Dialogs::ExtensionDialog(QApplication::activeWindow()).exec();

    if (data -> hasFormat(DROP_INNER_FORMAT)) {
        if (!dnd_list.isEmpty()) {
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

int IModel::search(const SearchLimit & params, Playlist * destination, Playlist * search_source) {
    int amount = 0;

    if (search_source == 0)
        search_source = root_item;

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
        search_source = root_item;

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
