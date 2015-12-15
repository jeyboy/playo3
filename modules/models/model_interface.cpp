#include "model_interface.h"

using namespace Models;
using namespace Core::Web;

bool IModel::restoreUrl(IItem * itm) {
    qDebug() << "RESTORE" << itm -> title();
    QString newUrl = Web::Apis::restoreUrl(itm -> refresh_path(), itm -> subtipe());

    qDebug() << itm -> refresh_path() << newUrl;
    if (!newUrl.isEmpty() && itm -> path().toString() != newUrl) {
        itm -> setPath(newUrl);
        return true;
    }

    return false;
}

IModel::IModel(QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent), addWatcher(0) {
    sync = new QMutex(QMutex::NonRecursive);
    rootItem = hash ? new Playlist(hash) : new Playlist();
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
        bool checked = !node -> is(ItemState::checked);
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
        result = iState != -ItemState::proceeded && iState != -ItemState::mark_on_removing;
    } else if (role == ISTATE) {
        Library::obj().setItemState(model_index, value.toInt());
        node -> setStates(value.toInt());
        result = true;
    } else if (role == Qt::EditRole)
        result = node -> setData(model_index.column(), value);

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
    recalcParentIndex(parent, pos, res -> eIndex, res -> eRow, list.first());
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

int IModel::proceedVkList(QJsonArray & collection, Playlist * parent) {
    if (collection.isEmpty()) return 0;

    int itemsAmount = 0;
    QJsonObject itm;
    VkFile * newItem;
    QString uri, id, owner, uid;
    QList<IItem *> items;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    int pos = parent -> playlistsAmount();
    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        id = QString::number(itm.value(Vk::tkn_id).toInt());
        owner = QString::number(itm.value(Vk::tkn_owner_id).toInt());
        uid = WebFile::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        uri = itm.value(Vk::tkn_url).toString();
        uri = uri.section('?', 0, 0); // remove extra info from url

        items = store.values(uid);

        if (items.isEmpty()) {
            itemsAmount++;
            newItem = new VkFile(
                id,
                uri,
                itm.value(Vk::tkn_artist).toString() % QStringLiteral(" - ") % itm.value(Vk::tkn_title).toString(),
                parent,
                pos
            );

            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromSeconds(itm.value(Vk::tkn_duration).toInt(0)));

//                if (itm.contains(Vk::genre_id_key))
//                    newItem -> setGenre(VkGenres::instance() -> toStandartId(itm.value(Vk::genre_id_key).toInt()));
        } else {
            QList<IItem *>::Iterator it_it = items.begin();

            for(; it_it != items.end(); it_it++)
                (*it_it) -> setPath(uri);
        }

        pos++;
    }

    return itemsAmount;
}

int IModel::proceedGrabberList(SubType wType, QJsonArray & collection, Playlist * parent) {
    if (collection.isEmpty()) return 0;

    int itemsAmount = 0;
    QJsonObject itm;
    WebFile * newItem;
    QString uri, refresh_url, id;

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        id = QString::number(itm.value(Grabber::id_key).toInt());

        uri = itm.value(Grabber::url_key).toString();
        refresh_url = itm.value(Grabber::refresh_key).toString();

        itemsAmount++;
        newItem = new WebFile(
            id,
            uri,
            itm.value(Grabber::title_key).toString(),
            parent
        );

        newItem -> setSubtype(wType);
        newItem -> setRefreshPath(refresh_url);
        newItem -> setExtension(itm.value(Grabber::extension_key).toString(Grabber::default_extension));

        if (itm.contains(Grabber::duration_key)) {
            if (itm.value(Grabber::duration_key).isDouble())
                newItem -> setDuration(Duration::fromMillis(itm.value(Grabber::duration_key).toInt(0)));
            else
                newItem -> setDuration(itm.value(Grabber::duration_key));
        }

        if (itm.contains(Grabber::genre_id_key))
            newItem -> setGenre(itm.value(Grabber::genre_id_key).toInt());

        if (itm.contains(Grabber::bpm_key))
            newItem -> setBpm(itm.value(Grabber::bpm_key).toInt());

        if (itm.contains(Grabber::size_key))
            newItem -> setSize(Info::fromUnits(itm.value(Grabber::size_key).toString()));

        if (!itm.contains(Grabber::skip_info_key))
            newItem -> setInfo(Info::str(
                    itm.value(Grabber::size_key).toString("?"),
                    newItem -> extension().toString(),
                    itm.value(Grabber::bitrate_key).toString("?"),
                    itm.value(Grabber::discretion_rate_key).toString("?"),
                    itm.value(Grabber::channels_key).toString("?")
                )
            );
    }

    return itemsAmount;
}

int IModel::proceedScList(QJsonArray & collection, Playlist * parent) {
    if (collection.isEmpty()) return 0;

    int itemsAmount = 0;
    QJsonObject itm;
    SoundcloudFile * newItem;
    QString uri, id, owner, uid;
    QList<IItem *> items;
    bool original;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        id = QString::number(itm.value(Soundcloud::tkn_id).toInt());
        owner = QString::number(itm.value(Soundcloud::tkn_user_id).toInt());
        uid = WebFile::toUid(owner, id);
        if (ignoreListContainUid(uid)) continue;

        uri = itm.value(Soundcloud::tkn_download_url).toString();
        if (uri.isEmpty()) {
            uri = itm.value(Soundcloud::tkn_stream_url).toString();
            original = false;
        } else { original = true;}
        if (uri.isEmpty()) continue;

        items = store.values(uid);

        if (items.isEmpty()) {
            itemsAmount++;
            newItem = new SoundcloudFile(
                id,
                uri,
                itm.value(Soundcloud::tkn_title).toString(),
                parent
            );

            newItem -> setVideoPath(itm.value(Soundcloud::tkn_video_url).toString());
            newItem -> setExtension(original ? itm.value(Soundcloud::tkn_original_format).toString() : Soundcloud::tkn_default_extension);
            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromMillis(itm.value(Soundcloud::tkn_duration).toInt(0)));

//            Genre::instance() -> toInt(fileIterObj.value("genre").toString())
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

int IModel::proceedOdList(QJsonArray & collection, Playlist * parent) {
    // {"albumId":82297694950393,"duration":160,"ensemble":"Kaka 47","id":82297702323201,"masterArtistId":82297693897464,"name":"Бутылек (Cover Макс Корж)","size":6435304,"version":""}
    if (collection.isEmpty()) return 0;

    int itemsAmount = 0;
    QJsonObject itm;
    OdFile * newItem;
    QString id;
    QList<IItem *> items;

    QHash<QString, IItem *> store;
    parent -> accumulateUids(store);

    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++) {
        itm = (*it).toObject();

        if (itm.isEmpty()) continue;

        qint64 iid = ((qint64)itm.value(Od::tkn_id).toDouble());
        id = QString::number(iid);
        if (iid == 0 || ignoreListContainUid(id)) continue;

        items = store.values(id);

        if (items.isEmpty()) {
            itemsAmount++;
            newItem = new OdFile(
                id,
                QString(),
                itm.value(Od::tkn_ensemble).toString() % Od::tkn_dash % itm.value(Od::tkn_name).toString(),
                parent
            );

            newItem -> setRefreshPath(id);

            newItem -> setExtension(Od::tkn_default_extension);
            newItem -> setDuration(Duration::fromSeconds(itm.value(Od::tkn_duration).toInt(0)));
            newItem -> setSize(itm.value(Od::tkn_size).toInt(0));

        } else {
            for(QList<IItem *>::Iterator it_it = items.begin(); it_it != items.end(); it_it++)
                (*it_it) -> setRefreshPath(id);
        }
    }

    return itemsAmount;
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
    QString ret = "";

    QHash<int, QString> templates;
    templates.insert(VK_FILE, QString::number(VK_FILE));
    templates.insert(SOUNDCLOUD_FILE, QString::number(SOUNDCLOUD_FILE));

    for(QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); it++) {
        IItem * itm = item(*it);
        QString ident = templates[itm -> itemType()];

        if (!ident.isEmpty()) {
            QVariant uid = itm -> toUid();
            if (uid.isValid())
                ret += " " + (ident) + SHARE_DELIMITER + uid.toString();
        }
    }

    if (!ret.isEmpty())
        ret = ret.mid(1);

    QApplication::clipboard() -> setText(ret);
}

void IModel::importIds(QStringList ids) {
    using namespace Data;

    emit moveInBackgroundProcess();
    QHash<int, QStringList> uidsMap;

    for(QStringList::Iterator it = ids.begin(); it != ids.end(); it++) {
        QStringList parts = (*it).split(SHARE_DELIMITER);
        uidsMap[parts.first().toInt()].append(parts.last());
    }

    Playlist * parentNode = 0;
    bool is_new = false;

    switch(playlistType()) {
        case soundcloud:
        case vk:
        case vk_rel:
        case level: { parentNode = rootItem; break; }
        case search:
        case tree:
        case level_tree: {
            QFileInfo file = QFileInfo(REMOTE_DND_URL.toLocalFile());
            QString path = file.path();
            if (path.isEmpty()) path = Extensions::folderName(file);
            parentNode = rootItem -> playlist(path);
            is_new = !parentNode;
            if (is_new) parentNode = rootItem -> createPlaylist(path);
        break;}
        default:;
    }

    if (!parentNode)
        qDebug() << "UNDEF FOLDER";

    if (is_new) {
        int from = parentNode -> row();
        beginInsertRows(index(parentNode -> parent()), from, from + ids.size() - 1);
    } else {
        int from = parentNode -> childCount();
        beginInsertRows(index(parentNode), from, from + ids.size() - 1);
    }

    for(QHash<int, QStringList>::Iterator map_it = uidsMap.begin(); map_it != uidsMap.end(); map_it++) {
        switch(map_it.key()) {
            case VK_FILE: {
                Vk::Api::obj().connection();

                if (Vk::Api::obj().isConnected()) {
                    QJsonArray obj = Vk::Api::obj().getAudiosInfo(map_it.value());
                    proceedVkList(obj, parentNode);
                }
            break;}

            case SOUNDCLOUD_FILE: {
                Soundcloud::Api::obj().connection();

                if (Soundcloud::Api::obj().isConnected()) {
                    QJsonArray obj = Soundcloud::Api::obj().audioInfo(map_it.value());
                    proceedScList(obj, parentNode);
                }
            break;}

            default: qDebug() << "UNSUPPORTED EXPORT TYPE";
        }
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
    rootItem -> propagatePlaylistSetFlag(ItemState::expanded);
}

void IModel::expanded(const QModelIndex & index) {
    IItem * node = item(index);
    node -> set(ItemState::expanded);
}

void IModel::collapseAll() {
    rootItem -> propagatePlaylistUnsetFlag(ItemState::expanded);
}

void IModel::collapsed(const QModelIndex & index) {
    IItem * node = item(index);
    node -> unset(IItem::expanded);
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
}

/////////////////////////////////////////////////////////

void IModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/) {
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

bool IModel::decodeInnerData(int row, int /*column*/, const QModelIndex & parent, QDataStream & stream) {
    int totalAdded = 0;
    QModelIndex dIndex;
    InnerData * data;
    Playlist * parentFolder;
    QHash<Playlist *, int> counts;
    bool containPath, isRemote, requirePath = !isRelative();

    while (!stream.atEnd()) {
        data = new InnerData();
        stream >> data -> url >> isRemote >> data -> attrs;

        if (!isRemote) {
            containPath = data -> attrs.contains(JSON_TYPE_PATH);

            if (requirePath) {
                if (!containPath)
                    data -> attrs.insert(JSON_TYPE_PATH, data -> url.toLocalFile().section('/', 0, -2));
            } else {
                if (containPath)
                    data -> attrs.remove(JSON_TYPE_PATH);
            }
        }
        else data -> url = REMOTE_DND_URL;

        data -> dRow = row;
        dIndex = parent;

        recalcParentIndex(dIndex, data -> dRow, data -> eIndex, data -> eRow, data -> url);

        parentFolder = item<Playlist>(dIndex);

        beginInsertRows(data -> eIndex, data -> eRow, data -> eRow);
            counts[parentFolder] += Playlist::restoreItem(data -> attrs.take(JSON_TYPE_ITEM_TYPE).toInt(), parentFolder, data -> dRow, data -> attrs);
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

    if (row == -1 && !parentIndex.data(IFOLDER).toBool()) {
        row = parentIndex.row();
        (const_cast<QModelIndex &>(parentIndex)) = parentIndex.parent();
    }

    int row_count = rowCount(parentIndex);
    if (row > row_count) row = -1;

    if (dropKeyModifiers & Qt::ControlModifier)
        Dialogs::ExtensionDialog(QApplication::activeWindow()).exec();

    if (data -> hasFormat(DROP_INNER_FORMAT)) {
        QByteArray encoded = data -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        return decodeInnerData(row, column, parentIndex, stream);
    } else if (data -> hasUrls())
        return /*insertRows*/threadlyInsertRows(data -> urls(), row, parentIndex);

    return false;
}

int IModel::initiateSearch(SearchRequest & params, Playlist * destination, Playlist * search_source) {
    int amount = 0;
    if (search_source == 0)
        search_source = rootItem;

    QList<IItem *> child = search_source -> childrenList();
    bool has_empty_predicate = params.spredicate.isEmpty();

    for(QList<IItem *>::Iterator it = child.begin(); it != child.end(); it++) {
        if ((*it) -> isContainer()) {
            initiateSearch(params, destination, (Playlist *) *it);
        } else {
            bool is_valid = has_empty_predicate || (*it) -> title().toString().contains(params.spredicate, Qt::CaseInsensitive);

            if (is_valid) {
                if (!params.sgenre.isEmpty()) {
//                    int genre_id = (*it) -> genreID().toInt();
//                    is_valid |= params.sgenre_id == genre_id;
                }

                if (is_valid) {
                    QVariantMap attrs = (*it) -> toInnerAttrs((*it) -> itemType());
                    if (!attrs.contains(JSON_TYPE_PATH))
                        attrs.insert(JSON_TYPE_PATH, (*it) -> toUrl().toLocalFile().section('/', 0, -2));

                    amount += Playlist::restoreItem(attrs.take(JSON_TYPE_ITEM_TYPE).toInt(), destination, -1, attrs);
                }
            }
        }
    }

    return amount;
}
