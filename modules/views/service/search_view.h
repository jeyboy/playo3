#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/service/search_model.h"

namespace View {
    class SearchView : public IView {
      Q_OBJECT
    public:
        inline SearchView(QWidget * parent, View::Params settings, QJsonObject * hash = 0)
            : IView(dynamic_cast<IModel *>(new SearchModel(hash)), parent, settings) {}

        inline void search(SearchSettings & params) { ((SearchModel *)mdl) -> initiateSearch(params); }
        inline void search(QStringList & predicates) { ((SearchModel *)mdl) -> initiateSearch(predicates); }
    };
}
#endif // SEARCH_VIEW_H
