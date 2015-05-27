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

    if (params.inVk) res.insert(SearchRequest::request_vk, rootItem -> createFolder("VK"));
    if (params.inTabs) res.insert(SearchRequest::request_tabs, rootItem -> createFolder("Tabs"));
    if (params.inComputer) res.insert(SearchRequest::request_computer, rootItem -> createFolder("Computer"));
    if (params.inSc) res.insert(SearchRequest::request_sc, rootItem -> createFolder("SC"));
    if (params.inOther) res.insert(SearchRequest::request_other, rootItem -> createFolder("OTHER"));

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
        FolderItem * parent = res.value(r.search_type) -> createFolder(r.token());

        switch(r.search_type) {
            case SearchRequest::request_vk: {
                QJsonArray items = VkApi::instance() -> audioSearchSync(
                    this, VkApi::instance() -> getUserID(), r.spredicate, params.type == artist, params.search_in_own, r.popular
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
    return res;
}
