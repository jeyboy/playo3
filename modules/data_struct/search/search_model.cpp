#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : LevelTreeModel(hash, parent) {}

SearchModel::~SearchModel() {}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    emit moveInProcess();

    if (!params.predicates.isEmpty()) { // search by predicates
        QStringList::Iterator it = params.predicates.begin();

        beginInsertRows(QModelIndex(), 0, params.predicates.length());
        for(; it != params.predicates.end(); it++, params.activePredicate = *it) {
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

        endInsertRows();
    } else { // search by styles only
        if (!params.genres.isEmpty()) {
            QList<int>::Iterator it = params.genres.keys().begin();
            QList<int>::Iterator it_end = params.genres.keys().end();
            beginInsertRows(QModelIndex(), 0, params.genres.size());

            for(; it != it_end; it++, params.activePredicate = *it) {
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

            endInsertRows();
        } else if (params.popular) {  // returns popular
            beginInsertRows(QModelIndex(), 0, 5);

            if (params.inVk) {
//                VkApi::instance() -> audioSearch(
//                    this, SLOT(proceedVk(QJsonObject &)), VkApi::instance() -> getUserID(), *it, params.type == artist, params.search_in_own, params.popular
//                );
            }

//            if (params.inTabs) {
//                proceedTabs(params);
//            }

//            if (params.inComputer) {}

            if (params.inSc) {}

            if (params.inOther) {}

            endInsertRows();
        }
    }
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
    FolderItem * pred_root = rootItem -> createFolder(params.activePredicate);
    FolderItem * parent = pred_root -> createFolder<VkFolder>("", "Tabs");

    QList<void *>::Iterator it = params.tabs.begin();
    for(; it != params.tabs.end(); it++) {
        ((IModel *) *it) -> initiateSearch(params, parent);
    }
}

void SearchModel::proceedMyComputer(SearchSettings params) {
//    FolderItem * pred_root = rootItem -> createFolder(params.activePredicate);
//    FolderItem * parent = pred_root -> createFolder<VkFolder>("", "Tabs");

    QStringList filters;
    filters << params.activePredicate;

    for(QStringList::Iterator it = params.drives.begin(); it != params.drives.end(); it++) {
        QDirIterator dir_it(*it, filters,  QDir::AllEntries | QDir::NoSymLinks, QDirIterator::Subdirectories);

        while(dir_it.hasNext()) {
            dir_it.next();
        }
    }
}
