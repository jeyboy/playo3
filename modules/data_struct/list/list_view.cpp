#include "list_view.h"

using namespace Playo3;

ListView::ListView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new ListModel(hash)), parent, settings) {
    setIndentation(-2);
}

ListView::~ListView() {}
