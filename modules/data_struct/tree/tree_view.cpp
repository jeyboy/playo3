#include "tree_view.h"

using namespace Playo3;

TreeView::TreeView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new TreeModel(hash)), parent, settings) {
}

TreeView::TreeView(IModel * newModel, QWidget * parent, ViewSettings settings) : IView(newModel, parent, settings) {

}

TreeView::~TreeView() {

}
