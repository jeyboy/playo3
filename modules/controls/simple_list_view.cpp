#include "simple_list_view.h"

using namespace Controls;

SimpleListView::SimpleListView(QWidget * parent) : QListView(parent) {}

void SimpleListView::removeSelected() {
    for(QModelIndexList::Iterator it = selectedIndexes().begin(); it != selectedIndexes().end(); it++)
        model() -> removeRow((*it).row(), (*it).parent());
}

void SimpleListView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete)
        removeSelected();

    QListView::keyPressEvent(event);
}
