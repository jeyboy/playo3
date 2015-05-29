#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "../level_tree/level_tree_model.h"
#include "modules/web/socials_api/vk_api.h"
#include "modules/web/socials_api/soundcloud_api.h"
#include "search_settings.h"

namespace Playo3 {
    class SearchModel : public LevelTreeModel {
        Q_OBJECT

    public:
        SearchModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline ContainerType containerType() const { return search; }

        void initiateSearch(SearchSettings & params);
    protected slots:
        int proceedTabs(SearchRequest & params, FolderItem * parent);
        int proceedMyComputer(SearchRequest & params, FolderItem * parent);
        void searchFinished();
    private:
        QList<FolderItem *> searchRoutine(QFutureWatcher<QList<FolderItem *> > * watcher);

        SearchSettings request;
        QFutureWatcher<QList<FolderItem *> > * initiator;
    };
}

#endif // SEARCH_MODEL_H
