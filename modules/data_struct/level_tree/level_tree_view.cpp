#include "level_tree_view.h"

using namespace Playo3;

LevelTreeView::LevelTreeView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : ViewInterface(dynamic_cast<ModelInterface *>(new LevelTreeModel(hash)), parent, settings) {
}

LevelTreeView::~LevelTreeView() {

}
