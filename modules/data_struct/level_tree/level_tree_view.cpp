#include "level_tree_view.h"

LevelTreeView::LevelTreeView(QWidget * parent, CBHash settingsSet, QJsonObject * hash)
    : View(dynamic_cast<ModelInterface *>(new LevelTreeModel(hash)), parent, settingsSet) {
}

LevelTreeView::~LevelTreeView() {

}
