#include "playlist.h"

#include "modules/models/service/removed_items.h"

using namespace Core;

///////////////////////////////////////////////////////////
Playlist::Playlist(QJsonObject * hash, Playlist * parent, const QJsonValue & childArr)
    : IItem(parent, hash), filesCount(attrs.take(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {

    if (!childArr.isNull()) {
        QJsonArray ar = childArr.toArray();

        for(QJsonArray::Iterator it = ar.begin(); it!= ar.end(); it++) {
            QJsonObject iterObj = (*it).toObject();

            DataSubType item_data_type = (DataSubType)iterObj.value(JSON_TYPE_ITEM_TYPE).toInt();
            if (DST_IS_PLAYLIST(item_data_type))
                new Playlist(&iterObj, this, iterObj.take(JSON_TYPE_CHILDS));
            else
                new IItem(this, &iterObj);
        }
    }
    if (parent != 0) parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const QVariantHash & hash, Playlist * parent, int pos) : IItem(parent, hash, pos), filesCount(0) {
    if (parent != 0) parent -> declarePlaylist(playlistUid(), this);
}

Playlist::~Playlist() {
    if (_parent)
        _parent -> undeclarePlaylist(playlistUid());

    bool remove_marked = is(flag_mark_on_removing);
    QVariantMap cue_map = takeCueMap();
    bool has_cue_map = !cue_map.isEmpty();

    QList<IItem *>::Iterator begin = children.begin(), end = children.end();
    while (begin != end) {
        if (remove_marked) (*begin) -> set(flag_mark_on_removing);
        if (has_cue_map) {
            QString fpath = (*begin) -> fullPath();
            if (cue_map.contains(fpath)) {
                int counter = cue_map[fpath].toInt();
                (*begin) -> setCueMap(--counter <= 0);
                cue_map[fpath] = counter;
            }
        }
        delete *begin;
        ++begin;
    }

    if (remove_marked)
        removePhysicalObject();
}

//void Playlist::addPlaylist(Playlist * node) {
//    node -> setParent(this);
//    declarePlaylist(node -> playlistUid(), node);
//    declareChild(node);
//    node -> backPropagateItemsCountInBranch(node -> childCount() - node -> playlistsAmount());
//}

void Playlist::accumulateUids(QHash<QString, IItem *> & store) {
    if (!store.isEmpty()) return;

    QVariant item_uid;

    for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++) {
        if ((*it) -> isRemote()) {
            if (!(*it) -> isContainer()) {
                item_uid = (*it) -> id();
                if (item_uid.isValid())
                    store.insert(UID_CAT((*it), item_uid.toString()), (*it));
            }
        }
    }
}

QStringList Playlist::childrenUids(int position, int count) {
    QStringList uids;

    if (position < 0 || position + count > children.size())
        return uids;

    IItem * it;

    for (int row = 0; row < count; ++row) {
        it = children.at(position);
        if (it -> isRemote()) {
            QVariant item_uid = it -> id();
            if (item_uid.isValid())
                uids << UID_CAT(it, item_uid.toString());
        }
    }

    return uids;
}

void Playlist::backPropagateItemsCountInBranch(int offset) {
    filesCount += offset;

    if (_parent)
        _parent -> backPropagateItemsCountInBranch(offset);
}

QVariant Playlist::data(int column) const {
    switch(column) {
        case Qt::ToolTipRole:  return QVariant(title().toString() % QStringLiteral("(") % QString::number(filesCount) % QStringLiteral(" items)"));
        case IEXECCOUNTS:      return filesCount;
        default:               return IItem::data(column);
    }
}

QJsonObject Playlist::toJson() {
    QJsonObject root = IItem::toJson();

    if (children.length() > 0) {
        root[JSON_TYPE_CONTAINER_ITEMS_COUNT] = filesCount;

        QJsonArray ar = QJsonArray();
        QList<IItem *>::Iterator it = children.begin();

        for( ;it != children.end(); it++)
            ar.append((*it) -> toJson());

        root[JSON_TYPE_CHILDS] = ar;
    }

    return root;
}

Playlist * Playlist::createPlaylistPath(const DataSubType & sub_type, const QString & path) { // usable only for tree
    QStringList list = path.split('/', QString::SkipEmptyParts);
    if (list.isEmpty())
        return this;
    return createPlaylist(DST_SET_FOLDER(sub_type), list.takeFirst(), &list);
}

Playlist * Playlist::createPlaylist(const DataSubType & sub_type, const QString & name, QStringList * list, int pos) {
    Playlist * curr = playlists.value(name, 0);

    if (!curr)
        curr = new Playlist(CONTAINER_ATTRS(DST_SET_FOLDER(sub_type), name), this, pos);

    if (list && !list -> isEmpty())
        return curr -> createPlaylist(DST_SET_FOLDER(sub_type), list -> takeFirst(), list, pos);
    else
        return curr;
}

Playlist * Playlist::findCompatblePlaylist(QStringList * list) { // find last exist folder in the path
    if (list -> isEmpty()) return this;
    Playlist * curr = playlists.value(list -> at(0), 0);
    if (!curr) return this;
    list -> removeFirst();
    return curr -> findCompatblePlaylist(list);
}

////    int ret = 0;
////    foreach(IItem * childItem, childItems) {
////        if (childItem -> folders == 0) // not is unprocessed
////            ret += 1;
////        else
////            ret += childItem -> childTreeCount();
////    }

//    return ret;
//}

//bool Playlist::insertChildren(int position, int count, int columns) {
//    if (position < 0 || position > childItems.size())
//        return false;

//    for (int row = 0; row < count; ++row) {
//        QVector<QVariant> data(columns);
//        IItem * item = new IItem(data, this);
//        childItems.insert(position, item);
//    }

//    return true;
//}

int Playlist::removeChildren(int position, int count, RemovedItems * deleted_items) {
    if (position < 0 || position + count > children.size())
        return 1;

    IItem * it;
    int totalItems = 0;

    for (int row = 0; row < count; ++row) {
        it = children.takeAt(position);
        totalItems -= it -> itemsCountInBranch();
        if (deleted_items) deleted_items -> registerItem(it);
        delete it;
    }

    backPropagateItemsCountInBranch(totalItems);
    return totalItems;
}

void Playlist::propagatePlaylistSetFlag(ItemStateFlag flag) {
    set(flag);
    for(QHash<QString, Playlist *>::Iterator it = playlists.begin(); it!= playlists.end(); it++)
        it.value() -> propagatePlaylistSetFlag(flag);
}
void Playlist::propagatePlaylistUnsetFlag(ItemStateFlag flag) {
    unset(flag);
    for(QHash<QString, Playlist *>::Iterator it = playlists.begin(); it!= playlists.end(); it++)
        it.value() -> propagatePlaylistUnsetFlag(flag);
}

void Playlist::updateCheckedState(bool checked) {
    IItem::updateCheckedState(checked);

    for(QList<IItem *>::Iterator it = children.begin(); it!= children.end(); it++)
        (*it) -> updateCheckedState(checked);
}

bool Playlist::updateCheckedStateByPredicate(ItemStateFlag pred_state) {
    bool valid = false;

    for(QList<IItem *>::Iterator it = children.begin(); it!= children.end(); it++)
        valid |= (*it) -> updateCheckedStateByPredicate(pred_state);

    IItem::updateCheckedState(valid);

    return valid;
}

void Playlist::shuffle() {
    qsrand((uint)QTime::currentTime().msec());
    int n = children.count() - 1;

    for (int i = 0; i < n; ++i)
        children.swap(i, qrand() % n);/*((n + 1) - i) + i)*/;

    for(QHash<QString, Playlist *>::Iterator it = playlists.begin(); it!= playlists.end(); it++)
        it.value() -> shuffle();
}

void Playlist::packToStream(QHash<QUrl, int> & urls, QDataStream & stream) {
    QList<IItem *>::Iterator child = children.begin();

    for(; child != children.end(); child++)
        (*child) -> packToStream(urls, stream);
}
