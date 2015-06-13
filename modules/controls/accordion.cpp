#include "accordion.h"

Accordion::Accordion(QWidget * parent) : QFrame(parent) {
    new_layout = new QVBoxLayout(this);
}

void Accordion::addItem(QString name, QWidget * item) {
    AccordionCell * cell;
    cells << (cell = new AccordionCell(name, item, this));
    new_layout -> addWidget(cell, 1);
}
void Accordion::removeItem(int index) {
    if (index < 0 || index >= cells.size()) return;
    delete cells.takeAt(index);
}
