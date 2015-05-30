#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : LevelTreeModel(hash, parent), initiator(0) {}

SearchModel::~SearchModel() {
    if (initiator)
        initiator -> cancel();

    delete initiator;
}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    initiator = new QFutureWatcher<QList<FolderItem *> >();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, &SearchModel::searchRoutine, initiator));
}

int SearchModel::proceedTabs(SearchRequest & params, FolderItem * parent) {
    int amount = 0;
    for(QList<void *>::Iterator it = request.tabs.begin(); it != request.tabs.end(); it++)
        amount += ((IModel *) *it) -> initiateSearch(params, parent);

    return amount;
}

int SearchModel::proceedMyComputer(SearchRequest & params, FolderItem * parent) {
    int amount = 0;
    QStringList filters;

    if (params.spredicate.isEmpty()) {
        filters = Extensions::instance() -> filterList("music");
    } else {
        QStringList res = Extensions::instance() -> filterList("music");
        for(QStringList::Iterator it = res.begin(); it != res.end(); it++)
            filters.append("*" + params.spredicate + (*it));
    }

    //System.Music.Artist:(Beethoven OR Mozart)

    for(QStringList::Iterator it = request.drives.begin(); it != request.drives.end(); it++) {
        QDirIterator dir_it(*it, filters,  QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);

        while(dir_it.hasNext()) {
            QString path = dir_it.next();
            qDebug() << "COMP FIND" << path;
            bool valid = params.sgenre_id == -1;

            if (!valid) {
                MediaInfo m(QUrl::fromLocalFile(path), true, true);
                valid = m.getGenre() == params.sgenre_id;
            }

            if (valid) {
                QFileInfo file = dir_it.fileInfo();
                new FileItem(file.path(), file.fileName(), parent);
                amount++;
            }
        }
    }
    return amount;
}

void SearchModel::searchFinished() {
    if (!initiator -> isCanceled()) {
        QList<FolderItem *> folders = initiator -> result();

        beginInsertRows(QModelIndex(), 0, folders.count());
            for(QList<FolderItem *>::Iterator it = folders.begin(); it != folders.end(); it++)
                rootItem -> linkNode(*it);
        endInsertRows();
    }

    delete initiator;
    initiator = 0;
}

QList<FolderItem *> SearchModel::searchRoutine(QFutureWatcher<QList<FolderItem *> > * watcher) {
    QList<SearchRequest> requests;
    QHash<int, FolderItem *> res;

    emit moveInBackgroundProcess();

    if (request.inVk) res.insert(SearchRequest::request_vk, new FolderItem("VK"));
    if (request.inTabs) res.insert(SearchRequest::request_tabs, new FolderItem("Tabs"));
    if (request.inComputer) res.insert(SearchRequest::request_computer, new FolderItem("Computer"));
    if (request.inSc) res.insert(SearchRequest::request_sc, new FolderItem("SC"));
    if (request.inOther) res.insert(SearchRequest::request_other, new FolderItem("Other"));

    if (!request.predicates.isEmpty()) { // search by predicates
        for(QStringList::Iterator it = request.predicates.begin(); it != request.predicates.end(); it++) {
            if (!request.genres.isEmpty()) {
                QList<int>::Iterator genre_it = request.genres.keys().begin();
                QList<int>::Iterator genre_it_end = request.genres.keys().end();

                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, *it, QString(), 0, request.popular));

                for(; genre_it != genre_it_end; genre_it++) {
                    if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                }
            } else {
                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, *it, QString(), -1, request.popular));

                if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, QString(), -1, request.popular));
                if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, QString(), -1, request.popular));
                if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, QString(), -1, request.popular));
                if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, QString(), -1, request.popular));
            }
        }
    } else if (!request.genres.isEmpty()) {
        if (!request.genres.isEmpty()) {
            QList<int>::Iterator genre_it = request.genres.keys().begin();
            QList<int>::Iterator genre_it_end = request.genres.keys().end();

            for(; genre_it != genre_it_end; genre_it++) {
                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, QString(), QString(), VkGenres::instance() -> fromStandartId(*genre_it), request.popular));
                if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
            }
        }
    } if (request.popular) {
        if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk));
        if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc));
        if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other));
        if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs));
        if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer));
    } else {
        if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs));
        if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer));
    }

    while(!requests.isEmpty()) {
        if (watcher -> isCanceled())
            return res.values();

        SearchRequest r = requests.takeFirst();
        FolderItem * parent = res.value(r.search_type) -> createFolder(r.token());

        switch(r.search_type) {
            case SearchRequest::request_vk: {
                QJsonArray items;
                if (r.spredicate.isEmpty() && r.popular) {
                    qDebug() << "VK POPULAR";
                    items = VkApi::instance() -> audioPopularSync(this, true, r.sgenre_id).value("audio_list").toArray();
                } else {
                    qDebug() << "VK SEARCH";
                    items = VkApi::instance() -> audioSearchSync(
                        this, VkApi::instance() -> getUserID(), r.spredicate, request.type == artist, request.search_in_own, r.popular
                    ).value("audio_list").toArray();
                }
                parent -> backPropagateItemsCountInBranch(proceedVkList(items, parent));
            break;}
            case SearchRequest::request_sc: {
                QJsonArray items = SoundcloudApi::instance() -> searchAudioSync(
                    this, r.spredicate, r.sgenre, r.popular
                ).value("audio_list").toArray();
                parent -> backPropagateItemsCountInBranch(proceedScList(items, parent));
            break;}
            case SearchRequest::request_computer: {
                parent -> backPropagateItemsCountInBranch(proceedMyComputer(r, parent));
            break;}
            case SearchRequest::request_tabs: {
                parent -> backPropagateItemsCountInBranch(proceedTabs(r, parent));
            break;}
            case SearchRequest::request_other: {
                //TODO: realize
            break;}
        }
    }

    emit moveOutBackgroundProcess();
    return res.values();
}
