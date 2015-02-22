#include "level_tree_view.h"

using namespace Playo3;

LevelTreeView::LevelTreeView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new LevelTreeModel(hash)), parent, settings) {
}

LevelTreeView::~LevelTreeView() {

}
