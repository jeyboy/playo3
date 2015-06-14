#include "accordion.h"

Accordion::Accordion(QWidget * parent) : QScrollArea(parent) {
    setWidgetResizable(true);
//    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    sizePolicy.setHorizontalStretch(1);
//    sizePolicy.setVerticalStretch(1);
//    setSizePolicy(sizePolicy);

    QWidget * container = new QWidget(this);
    new_layout = new QVBoxLayout;
    new_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    container -> setLayout(new_layout);
    setWidget(container);
}

void Accordion::addItem(QString name, QWidget * item, bool expanded) {
    Q_ASSERT(!item);
    AccordionCell * cell;
    cells << (cell = new AccordionCell(name, item, this));
    new_layout -> addWidget(cell);
    cell -> setCollapse(!expanded);
}
void Accordion::removeItem(int index) {
    if (index < 0 || index >= cells.size()) return;
    delete cells.takeAt(index);
}
void Accordion::setItemCollapsed(int index, bool collapsed) {
    if (index < 0 || index >= cells.size()) return;
    cells[index] -> setCollapse(collapsed);
}

void Accordion::ensureVisible(int index) {
    if (index < 0 || index >= cells.size()) return;

    ensureWidgetVisible(cells[index] -> title);
}

void Accordion::activate(int index) {
    if (index < 0 || index >= cells.size()) return;

    ensureWidgetVisible(cells[index] -> title);
    cells[index] -> setCollapse(false);
}

int Accordion::indexOf(QWidget * obj) {
    QList<AccordionCell *>::Iterator cell = cells.begin();
    for(int i = 0; cell != cells.end(); cell++, i++)
        if ((*cell) -> item == obj) return i;

    return -1;
}
