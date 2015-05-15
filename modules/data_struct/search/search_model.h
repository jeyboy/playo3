#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "../level_tree/level_tree_model.h"
#include "modules/web/socials_api/vk_api.h"
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
        void proceedVk(QJsonObject & collection);
        void proceedTabs(SearchSettings params);
        void proceedMyComputer(SearchSettings params);
    private:
        SearchSettings request;
    };
}

#endif // SEARCH_MODEL_H
