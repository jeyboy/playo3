#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : IModel(hash, parent) {}

SearchModel::~SearchModel() {}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    emit moveInProcess();

    if (!params.predicates.isEmpty()) { // search by predicates
        QStringList::Iterator it = params.predicates.begin();

        beginInsertRows(QModelIndex(), 0, params.predicates.length());
        for(; it != params.predicates.end(); it++) {
            rootItem -> createFolder(*it);

            if (params.inVk) {
                VkApi::instance() -> audioSearch(
                    this, SLOT(proceedVk(QJsonObject &)), VkApi::instance() -> getUserID(), *it, params.type == artist, params.search_in_own, params.popular
                );
            }

            if (params.inTabs) {
                proceedTabs(params);
            }

            if (params.inComputer) {}

            if (params.inSc) {}

            if (params.inOther) {}
        }
    } else { // search by styles only
        if (!params.genres.isEmpty()) {
            QList<int>::Iterator it = params.genres.keys().begin();
            QList<int>::Iterator it_end = params.genres.keys().end();
            beginInsertRows(QModelIndex(), 0, params.genres.size());

            for(; it != it_end; it++) {
                rootItem -> createFolder(MusicGenres::instance() -> toString(*it));

                if (params.inVk) {
    //                VkApi::instance() -> audioSearch(
    //                    this, SLOT(proceedVk(QJsonObject &)), VkApi::instance() -> getUserID(), *it, params.type == artist, params.search_in_own, params.popular
    //                );
                }

                if (params.inTabs) {
                    proceedTabs(params);
                }

                if (params.inComputer) {}

                if (params.inSc) {}

                if (params.inOther) {}
            }
        } else if (params.popular) {  // returns popular
            beginInsertRows(QModelIndex(), 0, 5);

            if (params.inVk) {
//                VkApi::instance() -> audioSearch(
//                    this, SLOT(proceedVk(QJsonObject &)), VkApi::instance() -> getUserID(), *it, params.type == artist, params.search_in_own, params.popular
//                );
            }

            if (params.inTabs) {
                proceedTabs(params);
            }

            if (params.inComputer) {}

            if (params.inSc) {}

            if (params.inOther) {}
        }
    }

    endInsertRows();
}

void SearchModel::proceedVk(QJsonObject & objects) {
    QJsonArray collection = objects.value("audio_list").toArray();
    if (collection.isEmpty())
        return;

    int itemsAmount = 0;
    QJsonObject itm;
    VkItem * newItem;
    QString uri, id, owner;
    QVariant uid;

    QString predicate = objects.value("predicate").toString();
    FolderItem * pred_root = rootItem -> createFolder(predicate);
    FolderItem * parent = pred_root -> createFolder<VkFolder>("", "VK");
    QJsonArray::Iterator it = collection.begin();

    beginInsertRows(index(pred_root), parent -> row(), parent -> row());
        for(; it != collection.end(); it++) {
            itm = (*it).toObject();

            if (itm.isEmpty()) continue;
            int genre_id = itm.value("genre_id").toInt();

            if (!request.vkGenres.isEmpty() && !request.vkGenres.contains(genre_id)) continue;

            id = QString::number(itm.value("id").toInt());
            owner = QString::number(itm.value("owner_id").toInt());
            uid = WebItem::toUid(owner, id);

            uri = itm.value("url").toString();
            uri = uri.section('?', 0, 0); // remove extra info from url

            itemsAmount++;
            newItem = new VkItem(
                id,
                uri,
                itm.value("artist").toString() + " - " + itm.value("title").toString(),
                parent
            );

            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromSeconds(itm.value("duration").toInt(0)));
            if (itm.contains("genre_id"))
                newItem -> setGenre(VkGenres::instance() -> toStandartId(genre_id));
        }

        parent -> backPropagateItemsCountInBranch(itemsAmount);
    endInsertRows();
    emit moveOutProcess();
}

void SearchModel::proceedTabs(SearchSettings params) {
    QList<void *>::Iterator it = params.tabs.begin();
    for(; it != params.tabs.end(); it++) {
//        ((IModel *) *it) -> initiateSearch(params, );
    }
}

void SearchModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url) {
    QFileInfo file = QFileInfo(url.toLocalFile());
    QString fName;

    if (file.isDir())
        fName = file.fileName();
    else
        fName = Extensions::folderName(file);

    FolderItem * nearestNode = rootItem -> folderItem(fName);
    FolderItem * node;
    if (!nearestNode) {
        exIndex = index(rootItem);
        exRow = rootItem -> childCount();
        node = rootItem -> createFolder(fName);
    } else {
        node = nearestNode;
        exIndex = index(nearestNode);
        exRow = nearestNode -> row();
    }

    (const_cast<QModelIndex &>(dIndex)) = index(node);

    if (dIndex != exIndex)
        dRow = -1;
}

void SearchModel::dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list) {
    FolderItem * node = item<FolderItem>(ind);
    int count = filesRoutine(list, node, row);

    if (count > 0) {
        rootItem -> updateItemsCountInBranch(count);
        emit itemsCountChanged(count);
    }
    else node -> removeYouself();
}

int SearchModel::filesRoutine(QFileInfo & currFile, FolderItem * node, QHash<FolderItem *, int> & rels) {
    int res = 0;
    rels[node]++;

    QFileInfoList folderList = Extensions::instance() -> folderDirectories(currFile);
    if (!folderList.isEmpty()) {
        QFileInfoList::Iterator it = folderList.begin();

        for(; it != folderList.end(); it++)
            res += filesRoutine((*it), rootItem -> createFolder(Extensions::folderName((*it))), rels);
    }

    QFileInfoList fileList = Extensions::instance() -> folderFiles(currFile);

    if (!fileList.isEmpty()) {
        QFileInfoList::Iterator it = fileList.begin();

        res += fileList.size();
        for(; it != fileList.end(); it++)
            new FileItem((*it).path(), (*it).fileName(), node);

        node -> updateItemsCountInBranch(fileList.size());
    }

    if (node -> childCount() == 0) {
        if (--rels[node] == 0)
            node -> removeYouself();
    }

    return res;
}

int SearchModel::filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos) {
    int res = 0;
    QList<QUrl>::ConstIterator it = list.begin();
    QHash<FolderItem *, int> relations;

    for(; it != list.end(); it++) {
        QFileInfo file = QFileInfo((*it).toLocalFile());
        if (file.isDir())
            res += filesRoutine(file, rootItem -> createFolder(Extensions::folderName(file)), relations);
        else {
            if (Extensions::instance() -> respondToExtension(file.suffix())) {
                res++;
                new FileItem(file.path(), file.fileName(), node, pos);
            }
        }
    }

    return res;
}
