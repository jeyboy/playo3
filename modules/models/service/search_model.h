#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "modules/models/level_tree_model.h"
#include "search_settings.h"

namespace Models {
    class SearchModel : public LevelTreeModel {
        Q_OBJECT
    public:
        inline SearchModel(QJsonObject * hash = 0, QObject * parent = 0)
            : LevelTreeModel(hash, parent), initiator(0) {}

        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline Data::Type playlistType() const { return Data::search; }

        void initiateSearch(const SearchSettings & params);
    protected slots:
        int proceedTabs(SearchRequest & params, Playlist * parent);
        int proceedMyComputer(SearchRequest & params, Playlist * parent);
        void searchFinished();
    private:
        Playlist * searchRoutine(QFutureWatcher<Playlist *> * watcher);
        void prepareRequests(QList<SearchRequest> & requests);

        SearchSettings request;
        QFutureWatcher<Playlist *> * initiator;
    };
}

#endif // SEARCH_MODEL_H
