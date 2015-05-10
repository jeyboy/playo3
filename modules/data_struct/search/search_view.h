#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H

#include "../view_interface.h"
#include "search_model.h"

namespace Playo3 {
    class SearchView : public IView {
      Q_OBJECT
    public:
        SearchView(QWidget * parent, ViewSettings settins, QJsonObject * attrs = 0);
        ~SearchView();

        inline void search(SearchSettings params) { ((SearchModel *)mdl) -> initiateSearch(params); }
    };
}
#endif // SEARCH_VIEW_H
