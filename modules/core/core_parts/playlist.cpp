#include "modules/core/core_parts_index.h"

using namespace Core;

int Playlist::restoreItem(int item_type, Playlist * parent, int pos, QVariantMap & attrs) {
    switch(item_type) {
        case SIMPLE_FILE:       { new File(attrs, parent, pos); return 1; }
        case VK_FILE:           { new VkFile(attrs, parent, pos); return 1; }
        case SOUNDCLOUD_FILE:   { new SoundcloudFile(attrs, parent, pos); return 1; }
        case OD_FILE:           { new OdFile(attrs, parent, pos); return 1; }
        case WEB_FILE:          { new WebFile(attrs, parent, pos); return 1; }
        case CUE_FILE:          { new CueFile(attrs, parent, pos); return 1; }
        default:                qDebug() << "ITEM TYPE NOT SUPPORTED YET";
    }

    return 0;
}

IItem * Playlist::restoreItem(Playlist * playlist, QJsonObject & iterObj) {
    switch(iterObj.take(JSON_TYPE_ITEM_TYPE).toInt()) {
        case SIMPLE_FILE:           return new File(&iterObj, playlist);
        case PLAYLIST:              return new Playlist(&iterObj, playlist);

        case WEB_FILE:              return new WebFile(&iterObj, playlist);
        case VK_FILE:               return new VkFile(&iterObj, playlist);
        case VK_PLAYLIST:           return new VkPlaylist(&iterObj, playlist);
        case SOUNDCLOUD_FILE:       return new SoundcloudFile(&iterObj, playlist);
        case SOUNDCLOUD_PLAYLIST:   return new SoundcloudPlaylist(&iterObj, playlist);
        case OD_FILE:               return new OdFile(&iterObj, playlist);
        case OD_PLAYLIST:           return new OdPlaylist(&iterObj, playlist);

        case CUE_FILE:              return new CueFile(&iterObj, playlist);
        case CUE_PLAYLIST:          return new CuePlaylist(&iterObj, playlist);

        default: return 0;
    }
}

///////////////////////////////////////////////////////////
Playlist::Playlist(QJsonObject * hash, Playlist * parent)
    : IItem(parent, hash -> take(JSON_TYPE_STATE).toInt()),
      filesCount(hash -> take(JSON_TYPE_CONTAINER_ITEMS_COUNT).toInt()) {

    if (hash -> contains(JSON_TYPE_CHILDS)) {
        QJsonArray ar = hash -> take(JSON_TYPE_CHILDS).toArray();
        QJsonObject iterObj;

        for(QJsonArray::Iterator it = ar.begin(); it!= ar.end(); it++) {
            iterObj = (*it).toObject();
            restoreItem(this, iterObj);
//            switch(iterObj.take(JSON_TYPE_ITEM_TYPE).toInt()) {
//                case SIMPLE_FILE: {
//                    new File(&iterObj, this);
//                break;}
//                case PLAYLIST: {
//                    new Playlist(&iterObj, this);
//                break;}

//                case WEB_FILE: {
//                    new WebFile(&iterObj, this);
//                break;}

//                case VK_FILE: {
//                    new VkFile(&iterObj, this);
//                break;}
//                case VK_PLAYLIST: {
//                    new VkPlaylist(&iterObj, this);
//                break;}
//                case SOUNDCLOUD_FILE: {
//                    new SoundcloudFile(&iterObj, this);
//                break;}
//                case SOUNDCLOUD_PLAYLIST: {
//                    new SoundcloudPlaylist(&iterObj, this);
//                break;}
//                case OD_FILE: {
//                    new OdFile(&iterObj, this);
//                break;}
//                case OD_PLAYLIST: {
//                    new OdPlaylist(&iterObj, this);
//                break;}

//                case CUE_FILE: {
//                    new CueFile(&iterObj, this);
//                break;}
//                case CUE_PLAYLIST: {
//                   new CuePlaylist(&iterObj, this);
//                break;}
//            }
        }
    }

    attrs = hash -> toVariantMap();
    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const QString & folderPath, const QString & folderTitle, Playlist * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    setPath(folderPath);

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const QString & folderTitle, Playlist * parent, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::Playlist(const QString & folderTitle, Playlist * parent, const QString & uid, int pos, int initState)
    : IItem(parent, folderTitle, pos, initState), filesCount(0) {

    setUid(uid);

    if (parent != 0)
        parent -> declarePlaylist(playlistUid(), this);
}

Playlist::~Playlist() {
    if (_parent)
        _parent -> undeclarePlaylist(playlistUid());

    bool remove_marked = is(mark_on_removing);

    QList<IItem *>::Iterator begin = children.begin(), end = children.end();
    while (begin != end) {
        if (remove_marked) (*begin) -> set(mark_on_removing);
        delete *begin;
        ++begin;
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

// cue is not removable yet
bool Playlist::removePhysicalObject() { // this is a little dangerous (   
//    QDir delDir(fullPath());
//    if (delDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System).count() == 0)
//        return delDir.removeRecursively();
    return false;
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

Playlist * Playlist::createPlaylistPath(QString path) { // usable only for tree
    QStringList list = path.split('/', QString::SkipEmptyParts);
    if (list.isEmpty())
        return this;
    return createPlaylist(list.takeFirst(), &list);
}

Playlist * Playlist::createPlaylist(const QString & name, QStringList * list, int pos) {
    Playlist * curr = playlists.value(name, 0);

    if (!curr)
        curr = new Playlist(name, this, pos);

    if (list && !list -> isEmpty())
        return curr -> createPlaylist(list -> takeFirst(), list, pos);
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
