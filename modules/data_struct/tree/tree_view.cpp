#include "tree_view.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new TreeModel(hash)), parent, settingsSet) {
}

TreeView::TreeView(Model * newModel, QWidget *parent, CBHash settingsSet) : View(newModel, parent, settingsSet) {

}

TreeView::~TreeView() {

}
