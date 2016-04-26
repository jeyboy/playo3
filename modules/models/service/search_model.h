#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "modules/models/level_tree_model.h"
#include "search_settings.h"

#define SEARCH_JSON_KEY QStringLiteral("search")
#define SEARCH_SET_JSON_KEY QStringLiteral("search_set")
#define SEARCH_REGLAMENT_JSON_KEY QStringLiteral("search_reglament")

namespace Models {
    class SearchModel : public LevelTreeModel {
        Q_OBJECT

        void searchRoutine(QFutureWatcher<void> * watcher);
        void searchSingleRoutine(QFutureWatcher<void> * watcher);
        void prepareRequests(QList<SearchRequest> & requests);
        void startSearch(bool continues = false);

        SearchSettings request;
        QFutureWatcher<void> * initiator;
        QList<SearchRequest> requests;
        QVariantHash search_reglament;
    public:
        inline SearchModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0)
            : LevelTreeModel(settings, hash, parent), initiator(0) {
            if (hash)
                resumeSearch(*hash);
        }

        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline Core::DataSubType playlistType() const { return Core::dt_search; }

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
