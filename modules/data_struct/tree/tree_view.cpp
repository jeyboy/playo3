#include "tree_view.h"

using namespace Playo3;

TreeView::TreeView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : ViewInterface(dynamic_cast<ModelInterface *>(new TreeModel(hash)), parent, settings) {
}

TreeView::TreeView(ModelInterface * newModel, QWidget * parent, ViewSettings settings) : ViewInterface(newModel, parent, settings) {

}

TreeView::~TreeView() {

}
