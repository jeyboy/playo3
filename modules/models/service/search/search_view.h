#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H

#include "../view_interface.h"
#include "search_model.h"

namespace Playo3 {
    class SearchView : public IView {
      Q_OBJECT
    public:
        inline SearchView(QWidget * parent, ViewSettings settings, QJsonObject * hash = 0)
            : IView(dynamic_cast<IModel *>(new SearchModel(hash)), parent, settings) {}

        inline void search(SearchSettings & params) { ((SearchModel *)mdl) -> initiateSearch(params); }
        inline void search(QStringList & predicates) { ((SearchModel *)mdl) -> initiateSearch(predicates); }
    };
}
#endif // SEARCH_VIEW_H
