#include "search_model.h"

using namespace Playo3;

SearchModel::SearchModel(QJsonObject * hash, QObject * parent) : LevelTreeModel(hash, parent) {}

SearchModel::~SearchModel() {}

void SearchModel::initiateSearch(SearchSettings & params) {
    request = params;

    QFutureWatcher<QList<FolderItem *> > * initiator = new QFutureWatcher<QList<FolderItem *> >();
    connect(initiator, SIGNAL(finished()), this, SLOT(searchFinished()));
    initiator -> setFuture(QtConcurrent::run(this, &SearchModel::searchRoutine, initiator));
}

void SearchModel::proceedTabs(SearchRequest & params, FolderItem * parent) {
    QList<void *>::Iterator it = request.tabs.begin();
    for(; it != request.tabs.end(); it++) {
        ((IModel *) *it) -> initiateSearch(params, parent);
    }
}

void SearchModel::proceedMyComputer(SearchRequest & params, FolderItem * parent) {
    QStringList filters;
    filters << params.spredicate;

    qDebug() << "OS" << filters;

    for(QStringList::Iterator it = request.drives.begin(); it != request.drives.end(); it++) {
        QDirIterator dir_it(*it, filters,  QDir::AllEntries | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);

        while(dir_it.hasNext()) {
            qDebug() << "COMP FIND" << dir_it.next();
            new FileItem(dir_it.filePath(), dir_it.fileName(), parent);
        }
    }

    qDebug() << "SOSA";
}

void SearchModel::searchFinished() {
    QFutureWatcher<QList<FolderItem *> > * watcher = (QFutureWatcher<QList<FolderItem *> > *)sender();
    QList<FolderItem *> folders = watcher -> result();

    beginInsertRows(QModelIndex(), 0, folders.count());
        for(QList<FolderItem *>::Iterator it = folders.begin(); it != folders.end(); it++)
            rootItem -> linkNode(*it);
    endInsertRows();

    delete watcher;
}

QList<FolderItem *> SearchModel::searchRoutine(QFutureWatcher<QList<FolderItem *> > * watcher) {
    QList<SearchRequest> requests;
    QHash<int, FolderItem *> res;

    emit moveInBackgroundProcess();

    qDebug() << "SEARCH DRIVES" << request.drives;
    qDebug() << "SEARCH GENRES" << request.genres;
    qDebug() << "SEARCH VK GENRES" << request.vkGenres;
    qDebug() << "SEARCH PREDICATES" << request.predicates;
    qDebug() << "SEARCH TABS" << request.tabs;

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
                    if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                    if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, request.genres.value(*genre_it), *genre_it, request.popular));
                }
            } else {
                if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, *it, QString(), -1, request.popular));

                if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, *it, QString(), -1, request.popular));
                if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, *it, QString(), -1, request.popular));
                if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, *it, QString(), -1, request.popular));
                if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, *it, QString(), -1, request.popular));
            }
        }
    } else if (!request.genres.isEmpty()) {
        if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk, QString(), QString(), 0, request.popular));

        if (!request.genres.isEmpty()) {
            QList<int>::Iterator genre_it = request.genres.keys().begin();
            QList<int>::Iterator genre_it_end = request.genres.keys().end();

            for(; genre_it != genre_it_end; genre_it++) {
                if (request.inTabs) requests.append(SearchRequest(SearchRequest::request_tabs, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inComputer) requests.append(SearchRequest(SearchRequest::request_computer, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
                if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other, QString(), request.genres.value(*genre_it), *genre_it, request.popular));
            }
        }
    } if (request.popular) {
        if (request.inVk) requests.append(SearchRequest(SearchRequest::request_vk));
        if (request.inSc) requests.append(SearchRequest(SearchRequest::request_sc));
        if (request.inOther) requests.append(SearchRequest(SearchRequest::request_other));
    }

    while(!requests.isEmpty()) {
        SearchRequest r = requests.takeFirst();
        FolderItem * parent = res.value(r.search_type) -> createFolder(r.token());

        switch(r.search_type) {
            case SearchRequest::request_vk: {
                QJsonArray items = VkApi::instance() -> audioSearchSync(
                    this, VkApi::instance() -> getUserID(), r.spredicate, request.type == artist, request.search_in_own, r.popular
                ).value("audio_list").toArray();
                proceedVkList(items, parent);
            break;}
            case SearchRequest::request_sc: {
                QJsonArray items = SoundcloudApi::instance() -> searchAudioSync(
                    this, r.spredicate, r.sgenre, r.popular
                ).value("audio_list").toArray();
                proceedScList(items, parent);
            break;}
            case SearchRequest::request_computer: {
                proceedMyComputer(r, parent);
            break;}
            case SearchRequest::request_tabs: {
                proceedTabs(r, parent);
            break;}
            case SearchRequest::request_other: {
                //TODO: realize
            break;}
        }
    }

    emit moveOutBackgroundProcess();
    return res.values();
}
