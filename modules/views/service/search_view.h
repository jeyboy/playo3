#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/service/search_model.h"

namespace Views {
    class SearchView : public IView {
      Q_OBJECT
    public:
        inline SearchView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new SearchModel(settings, hash), parent) {}

        ~SearchView() {}
        inline void search(const SearchLimitLayers & params) { ((SearchModel *)mdl) -> initiate(params); }
    };
}
#endif // SEARCH_VIEW_H
