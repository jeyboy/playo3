#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "modules/models/level_tree_model.h"
#include "search_settings.h"

#define SEARCH_JSON_KEY QStringLiteral("search")
#define SEARCH_SET_JSON_KEY QStringLiteral("search_set")

namespace Models {
    class SearchModel : public LevelTreeModel {
        Q_OBJECT

        void searchRoutine(QFutureWatcher<void> * watcher);
        void prepareRequests(QList<SearchRequest> & requests);
        void startSearch();

        SearchSettings request;
        QFutureWatcher<void> * initiator;
        QList<SearchRequest> requests;
    public:
        inline SearchModel(QJsonObject * hash = 0, QObject * parent = 0)
            : LevelTreeModel(hash, parent), initiator(0) {
            if (hash)
                resumeSearch(*hash);
        }

        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline Data::Type playlistType() const { return Data::search; }

        void initiateSearch(const SearchSettings & params);
        void declineSearch();
        void suspendSearch(QJsonObject & obj);
        void resumeSearch(const QJsonObject & obj);

        inline bool inProccess() { return initiator && initiator -> isRunning(); }
        inline QJsonObject toJson() {
            QJsonObject res = LevelTreeModel::toJson();
            suspendSearch(res);
            return res;
        }
    protected slots:
        int proceedTabs(SearchRequest & params, Playlist * parent);
        int proceedMyComputer(SearchRequest & params, Playlist * parent);
        void searchFinished();
    };
}

#endif // SEARCH_MODEL_H
