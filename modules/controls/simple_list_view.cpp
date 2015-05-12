#include "simple_list_view.h"

SimpleListView::SimpleListView(QWidget * parent) : QListView(parent) {}

void SimpleListView::removeSelected() {
    QModelIndexList::Iterator it = selectedIndexes().begin();

    for(; it != selectedIndexes().end(); it++)
        model() -> removeRow((*it).row(), (*it).parent());
}

void SimpleListView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete)
        removeSelected();

    QListView::keyPressEvent(event);
}
