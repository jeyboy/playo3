#include "accordion.h"

using namespace Controls;

Accordion::Accordion(QWidget * parent) : QScrollArea(parent), exclusive(false), toggleable(true), currentCell(0), tabs(new QButtonGroup(this)) {
    topButton = new QPushButton(QStringLiteral("*"), this);
    topButton -> setFixedSize(16, 16);
    topButton -> hide();
    connect(topButton, SIGNAL(clicked()), this, SLOT(collapseRequired()));

    QWidget * container = new QWidget(this);
    new_layout = new QVBoxLayout;
    new_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    new_layout -> setContentsMargins(2, 2, 2, 2);
    container -> setLayout(new_layout);

    setWidget(container);
    setWidgetResizable(true);

    connect((QObject *)verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));
    connect(tabs, SIGNAL(buttonClicked(int)), this, SLOT(tabActivated(int)));
}

void Accordion::addItem(const QString & name, QWidget * item, bool expanded) {
    AccordionCell * cell;
    cells << (cell = new AccordionCell(name, item, this));
    new_layout -> addWidget(cell);
    tabs -> addButton(cell -> title, (int)cell);
    cell -> title -> setCheckable(exclusive);
    cell -> setCollapse(true);
    if (expanded)
        cell -> title -> animateClick();
}
QWidget * Accordion::addItem(const QString & name, bool expanded) {
    QWidget * panel = new QWidget(this);
    addItem(name, panel, expanded);
    return panel;
}

void Accordion::removeItem(int index) {
    if (index < 0 || index >= cells.size()) return;
    tabs -> removeButton(cells[index] -> title);
    delete cells.takeAt(index);
}
void Accordion::setItemCollapsed(int index, bool collapsed) {
    if (index < 0 || index >= cells.size()) return;

    if (collapsed != cells[index] -> isCollapsed())
        cells[index] -> title -> animateClick();
}

void Accordion::ensureVisible(int index) {
    if (index < 0 || index >= cells.size()) return;
    ensureWidgetVisible(cells[index] -> title);
}

void Accordion::activate(int index) {
    if (index < 0 || index >= cells.size()) return;

    tabs -> blockSignals(true);
    cells[index] -> title -> animateClick();
    cells[index] -> setCollapse(false);
    tabs -> blockSignals(false);
}

int Accordion::indexOf(QWidget * obj) {
    QList<AccordionCell *>::Iterator cell = cells.begin();
    for(int i = 0; cell != cells.end(); cell++, i++)
        if ((*cell) -> item == obj) return i;

    return -1;
}

void Accordion::setExclusive(bool is_exclusive) {
    tabs -> setExclusive(is_exclusive);
    if ((exclusive = is_exclusive)) {
        int i = 0;
        for(QList<AccordionCell *>::Iterator cell = cells.begin(); cell != cells.end(); cell++) {
            (*cell) -> title -> setCheckable(true);
            if (!(*cell) -> isCollapsed())
                (*cell) -> setCollapse(++i > 1);
        }
    }
}

void Accordion::collapseRequired() {
    if (currentCell) {
        currentCell -> toogleCollapse();
        topButton -> hide();
        new_layout -> setContentsMargins(1, 1, 1, 1);
        ensureWidgetVisible(currentCell -> title, 10, 10);
    }
}

void Accordion::tabActivated(int obj, TabState act) {
    AccordionCell * cell = (AccordionCell *)obj;
    if (!cell) return;

    if (cell != currentCell)
        emit currentChanged((int)cell -> item);

    if (!toggleable && act == toggle) act = expand;

    switch(act) {
        case expand: {
            if (exclusive && currentCell) currentCell -> setCollapse(true);
            cell -> setCollapse(false);
            ensureWidgetVisible(cell);
        break;}

        case collapse: {
            cell -> setCollapse(false);
            ensureWidgetVisible(cell -> title);
        break; }

        default: {
            if (exclusive && currentCell && cell -> isCollapsed() && cell != currentCell) currentCell -> setCollapse(true);
            cell -> toogleCollapse();
            ensureWidgetVisible(cell -> title);
        }
    }

    currentCell = cell;
}

void Accordion::scrollValueChanged(int value) {
    if (value == 0) return;

    QWidget * w = qobject_cast<QWidget *>(childAt(width() / 2, 1));

    if (!w) return;

    if (!dynamic_cast<AccordionCell *>(w)) {
        while(w -> parent() && w -> parentWidget() != widget())
            w = w -> parentWidget();

        AccordionCell * cell = dynamic_cast<AccordionCell *>(w);

        if (cell) {
            if (cell -> isCollapsed()) return;

            if (currentCell != cell) {
                currentCell = cell;
    //            topButton -> setText(currentCell -> title -> text());
            }

            new_layout -> setContentsMargins(17, 1, 1, 1);
            topButton -> show();
            return;
        }
    }

    topButton -> hide();
    new_layout -> setContentsMargins(1, 1, 1, 1);
}
