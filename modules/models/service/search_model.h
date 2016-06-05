#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "modules/models/level_tree_model.h"

#define SEARCH_REQUESTS_JSON_KEY QStringLiteral("search_requests")
#define SEARCH_REGLAMENT_JSON_KEY QStringLiteral("search_reglament")

#define STOP_SEARCH() \
    initiator -> cancel(); \
    initiator -> waitForFinished();

namespace Models {
    class SearchModel : public LevelTreeModel {
        Q_OBJECT

        void searchRoutine(QFutureWatcher<void> * watcher);
        void start();

        SearchLimitLayers layers;
        QList<SearchLimitLayer> requests;

        QFutureWatcher<void> * initiator;
        QVariantHash search_reglament;
    public:
        inline SearchModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0)
            : LevelTreeModel(settings, hash, parent), initiator(0) {
            if (hash) load(*hash);
        }

        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline Core::DataSubType playlistType() const { return Core::dt_search; }

        void initiate(const SearchLimitLayers & params);
        void decline();
        void save(QJsonObject & obj);
        void load(const QJsonObject & obj);

        inline bool inProccess() { return initiator && initiator -> isRunning(); }
        inline QJsonObject toJson() {
            QJsonObject res = LevelTreeModel::toJson();
            save(res);
            return res;
        }
    protected slots:
        int proceedTabs(SearchLimit & limit, Playlist * parent);
        int proceedMyComputer(SearchLimit & limit, Playlist * parent);
        void searchFinished();
    };
}

#endif // SEARCH_MODEL_H
