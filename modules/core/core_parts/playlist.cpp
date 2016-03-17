#include "playlist.h"

using namespace Core;

///////////////////////////////////////////////////////////
Playlist::Playlist(QJsonObject * hash, Playlist * parent)
    : IItem(parent), filesCount(hash -> take(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {

    if (hash -> contains(JSON_TYPE_CONTAINER_CHILDS)) {
        QJsonArray ar = hash -> take(JSON_TYPE_CONTAINER_CHILDS).toArray();

        for(QJsonArray::Iterator it = ar.begin(); it!= ar.end(); it++) {
            QJsonObject iterObj = (*it).toObject();
            if (iterObj.value(JSON_TYPE_DATA_SUB_TYPE).toInt() < dt_none)
                new Playlist(&iterObj, this);
            else
                new IItem(this, &iterObj);
        }
    }

    attrs = new QVariantHash(hash -> toVariantHash());
    ItemState::setStates(attrs -> value(JSON_TYPE_ITEM_STATE).toInt());

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const DataSubType & subType, const QString & folderPath, const QString & folderTitle, Playlist * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    setPath(folderPath);
    setDatatype(subType);

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const DataSubType & subType, const QString & folderTitle, Playlist * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    setDatatype(subType);

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const DataSubType & subType, const QString & folderTitle, Playlist * parent, const QString & uid, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    setId(uid);
    setDatatype(subType);

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::~Playlist() {
    if (_parent)
        _parent -> undeclarePlaylist(playlistUid());

    bool remove_marked = has(flag_mark_on_removing);

    if (!children.isEmpty()) {
        QList<IItem *>::Iterator begin = children.begin(), end = children.end();
        while (begin != end) {
            if (remove_marked) (*begin) -> set(flag_mark_on_removing);
            delete *begin;
            ++begin;
        }

        children.clear();
    }

    if (remove_marked)
        removePhysicalObject();
}

void Playlist::linkNode(Playlist * node) {
    node -> setParent(this);
    declarePlaylist(node -> playlistUid(), node);
    declareChild(node);
    node -> backPropagateItemsCountInBranch(node -> childCount() - node -> playlistsAmount());
}

void Playlist::accumulateUids(QHash<QString, IItem *> & store) {
    QString item_uid;

    for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++) {
        if ((*it) -> isRemote()) {
            if (!(*it) -> isContainer()) {
                item_uid = (*it) -> toUid();
                if (!item_uid.isEmpty())
                    store.insert(item_uid, (*it));
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
            QString item_uid = it -> toUid();
            if (!item_uid.isEmpty())
                uids << item_uid;
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

void Playlist::toJson(QJsonObject & obj) {
    ItemFields::toJson(obj);

    if (children.length() > 0) {
        obj[JSON_TYPE_CONTAINER_ITEMS_COUNT] = filesCount;

        QJsonArray ar = QJsonArray();
        for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++) {
            QJsonObject tobj;
            (*it) -> toJson(tobj);
            ar << tobj;
        }

        obj[JSON_TYPE_CONTAINER_CHILDS] = ar;
    }
}

Playlist * Playlist::createPlaylistPath(const DataSubType & subType, const QString & path) { // usable only for tree
    QStringList list = path.split('/', QString::SkipEmptyParts);
    if (list.isEmpty())
        return this;
    return createPlaylist(subType, list.takeFirst(), &list);
}

Playlist * Playlist::createPlaylist(const DataSubType & subType, const QString & uid, const QString & name, int pos) {
    Playlist * curr = playlists.value(playlistUid(name, uid), 0);
    if (curr) return curr;
    return new Playlist(subType, name, this, uid, pos);
}

Playlist * Playlist::createPlaylist(const DataSubType & subType, const QString & name, QStringList * list, int pos) {
    Playlist * curr = playlists.value(name, 0);

    if (!curr)
        curr = new Playlist(subType, name, this, pos);

    if (list && !list -> isEmpty())
        return curr -> createPlaylist(subType, list -> takeFirst(), list, pos);
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

int Playlist::removeChildren(int position, int count) {
    if (position < 0 || position + count > children.size())
        return 1;

    IItem * it;
    int totalItems = 0;

    for (int row = 0; row < count; ++row) {
        it = children.takeAt(position);
        totalItems -= it -> itemsCountInBranch();
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
