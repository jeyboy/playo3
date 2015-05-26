#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : LevelTreeModel(hash, parent) {}

SearchModel::~SearchModel() {}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    emit moveInBackgroundProcess();

    qDebug() << "SEARCH DRIVES" << params.drives;
    qDebug() << "SEARCH GENRES" << params.genres;
    qDebug() << "SEARCH VK GENRES" << params.vkGenres;
    qDebug() << "SEARCH PREDICATES" << params.predicates;
    qDebug() << "SEARCH TABS" << params.tabs;

    if (!params.predicates.isEmpty()) { // search by predicates
        for(QStringList::Iterator it = params.predicates.begin(); it != params.predicates.end(); it++) {
            params.activePredicate = *it;

            if (!params.genres.isEmpty()) {
                QList<int>::Iterator genre_it = params.genres.keys().begin();
                QList<int>::Iterator genre_it_end = params.genres.keys().end();

                if (params.inVk) requests.append(SearchRequest(SearchRequest::request_vk, *it, QString(), 0, params.popular));

                for(; genre_it != genre_it_end; genre_it++) {
                    if (params.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, params.genres.value(*genre_it), *genre_it, params.popular));
                    if (params.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, params.genres.value(*genre_it), *genre_it, params.popular));
                    if (params.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, params.genres.value(*genre_it), *genre_it, params.popular));
                    if (params.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, params.genres.value(*genre_it), *genre_it, params.popular));
                }
            } else {
                if (params.inVk) requests.append(SearchRequest(SearchRequest::request_vk, *it, QString(), -1, params.popular));

                if (params.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, QString(), -1, params.popular));
                if (params.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, QString(), -1, params.popular));
                if (params.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, QString(), -1, params.popular));
                if (params.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, QString(), -1, params.popular));
            }
        }
    } else if (!params.genres.isEmpty()) {
        if (params.inVk) requests.append(SearchRequest(SearchRequest::request_vk, QString(), QString(), 0, params.popular));

        if (!params.genres.isEmpty()) {
            QList<int>::Iterator genre_it = params.genres.keys().begin();
            QList<int>::Iterator genre_it_end = params.genres.keys().end();

            for(; genre_it != genre_it_end; genre_it++) {
                if (params.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, QString(), params.genres.value(*genre_it), *genre_it, params.popular));
                if (params.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, QString(), params.genres.value(*genre_it), *genre_it, params.popular));
                if (params.inSc) requests.append(SearchRequest(SearchRequest::request_sc, QString(), params.genres.value(*genre_it), *genre_it, params.popular));
                if (params.inOther) requests.append(SearchRequest(SearchRequest::request_other, QString(), params.genres.value(*genre_it), *genre_it, params.popular));
            }
        }
    } if (params.popular) {
        if (params.inVk) requests.append(SearchRequest(SearchRequest::request_vk));
        if (params.inSc) requests.append(SearchRequest(SearchRequest::request_sc));
        if (params.inOther) requests.append(SearchRequest(SearchRequest::request_other));
    }

    while(!requests.isEmpty()) {
        SearchRequest r = requests.takeFirst();

        switch(r.search_type) {
            case SearchRequest::request_vk: {
                VkApi::instance() -> audioSearch(
                    this, SLOT(proceedVk(QJsonObject &)), VkApi::instance() -> getUserID(), r.spredicate, params.type == artist, params.search_in_own, r.popular
                );
            break;}
            case SearchRequest::request_sc: {
                SoundcloudApi::instance() -> search(
                    this, SLOT(proceedSoundcloud(QJsonObject&)), r.spredicate, r.sgenre, r.popular
                );
            break;}
            case SearchRequest::request_computer: {
                proceedMyComputer(r);
            break;}
            case SearchRequest::request_tabs: {
                proceedTabs(r);
            break;}
            case SearchRequest::request_other: {

            break;}
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

void SearchModel::proceedSoundcloud(QJsonObject & objects) {
    QJsonArray collection = objects.value("audio_list").toArray();
    if (collection.isEmpty())
        return;

    int itemsAmount = 0;
    QJsonObject itm;
    SoundcloudItem * newItem;
    QString uri, id, owner;
    QVariant uid;
    bool original;

    QString predicate = objects.value("predicate").toString();
    FolderItem * pred_root = rootItem -> createFolder(predicate);
    FolderItem * parent = pred_root -> createFolder<SoundcloudFolder>("", "Soundcloud");
    QJsonArray::Iterator it = collection.begin();

    beginInsertRows(index(pred_root), parent -> row(), parent -> row());
        for(; it != collection.end(); it++) {
            itm = (*it).toObject();

            if (itm.isEmpty()) continue;
            int genre_id = itm.value("genre_id").toInt(-1);

            if (!request.genres.isEmpty() && !request.genres.contains(genre_id)) continue;

            id = QString::number(itm.value("id").toInt());
            owner = QString::number(itm.value("user_id").toInt());
            uid = WebItem::toUid(owner, id);

            uri = itm.value("download_url").toString();
            if (uri.isEmpty()) {
                uri = itm.value("stream_url").toString();
                original = false;
            } else { original = true;}
            if (uri.isEmpty()) continue;


            itemsAmount++;
            newItem = new SoundcloudItem(
                id,
                uri,
                itm.value("title").toString(),
                parent
            );

            newItem -> setVideoPath(itm.value("video_url").toString());
            newItem -> setExtension(original ? itm.value("original_format").toString() : "mp3");
            newItem -> setOwner(owner);
            newItem -> setDuration(Duration::fromMillis(itm.value("duration").toInt(0)));

            if (genre_id != -1)
                newItem -> setGenre(genre_id);
        }

        parent -> backPropagateItemsCountInBranch(itemsAmount);
    endInsertRows();
    emit moveOutProcess();
}

void SearchModel::proceedTabs(SearchRequest & params) {
    FolderItem * pred_root = rootItem -> createFolder(params.spredicate);
    FolderItem * parent = pred_root -> createFolder<VkFolder>("", "Tabs");

    QList<void *>::Iterator it = request.tabs.begin();
    for(; it != request.tabs.end(); it++) {
        ((IModel *) *it) -> initiateSearch(params, parent);
    }
}

void SearchModel::proceedMyComputer(SearchRequest & params) {
    FolderItem * parent = new FolderItem("My computer");

    QStringList filters;
    filters << params.spredicate;

    qDebug() << filters;

    for(QStringList::Iterator it = request.drives.begin(); it != request.drives.end(); it++) {
        QDirIterator dir_it(*it, filters,  QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);

        while(dir_it.hasNext()) {
            qDebug() << "COMP FIND" << dir_it.next();
            new FileItem(dir_it.filePath(), dir_it.fileName(), parent);
        }
    }
}
