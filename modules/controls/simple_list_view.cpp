#include "simple_list_view.h"

SimpleListView::SimpleListView(QWidget *parent) : QListView(parent) {}

void SimpleListView::removeSelected() {
    foreach(QModelIndex index, selectedIndexes()) {
        model() -> removeRow(index.row(), index.parent());
    }
}

void SimpleListView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete) {
        removeSelected();
    }

    QListView::keyPressEvent(event);
}
