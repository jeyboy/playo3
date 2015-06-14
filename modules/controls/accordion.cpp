#include "accordion.h"

Accordion::Accordion(QWidget * parent) : QScrollArea(parent) {
//    setWidgetResizable(true);
//    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    sizePolicy.setHorizontalStretch(1);
//    sizePolicy.setVerticalStretch(1);
//    setSizePolicy(sizePolicy);

    new_layout = new QVBoxLayout;
    new_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    setLayout(new_layout);
}

void Accordion::addItem(QString name, QWidget * item) {
    item -> setStyleSheet("background-color: red;");
    AccordionCell * cell;
    cells << (cell = new AccordionCell(name, item, this));
    new_layout -> addWidget(cell);
}
void Accordion::removeItem(int index) {
    if (index < 0 || index >= cells.size()) return;
    delete cells.takeAt(index);
}
