#include "search_view.h"

using namespace Playo3;

SearchView::SearchView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new SearchModel(hash)), parent, settings) {}

SearchView::~SearchView() {}
