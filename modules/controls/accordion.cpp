#include "accordion.h"
#include <qdebug.h>

Accordion::Accordion(QWidget * parent) : QFrame(parent), currentCell(0) {
    QVBoxLayout * base_layout = new QVBoxLayout;
    base_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    setLayout(base_layout);
    base_layout -> setContentsMargins(0,0,0,0);

    topButton = new AccordionButton("", this);
    base_layout -> addWidget(topButton);
    topButton -> hide();

    scrollArea = new QScrollArea(this);
    scrollArea -> setWidgetResizable(true);

    QWidget * container = new QWidget(this);
    new_layout = new QVBoxLayout;
    new_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    container -> setLayout(new_layout);
    scrollArea -> setWidget(container);

    base_layout -> addWidget(scrollArea);

    connect((QObject *)scrollArea -> verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));
}

void Accordion::addItem(QString name, QWidget * item, bool expanded) {
    Q_ASSERT(!item);
    AccordionCell * cell;
    cells << (cell = new AccordionCell(name, item, this));
    new_layout -> addWidget(cell);
    cell -> setCollapse(!expanded);
}
QWidget * Accordion::addItem(QString name, bool expanded) {
    QWidget * panel = new QWidget(this);
    addItem(name, panel, expanded);
    return panel;
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

    scrollArea -> ensureWidgetVisible(cells[index] -> title);
}

void Accordion::activate(int index) {
    if (index < 0 || index >= cells.size()) return;

    scrollArea -> ensureWidgetVisible(cells[index] -> title);
    cells[index] -> setCollapse(false);
}

int Accordion::indexOf(QWidget * obj) {
    QList<AccordionCell *>::Iterator cell = cells.begin();
    for(int i = 0; cell != cells.end(); cell++, i++)
        if ((*cell) -> item == obj) return i;

    return -1;
}

void Accordion::scrollValueChanged(int value) {
    if (value == 0) return;

    QWidget * w = qobject_cast<QWidget *>(childAt(width() / 2, scrollArea -> pos().y() + 1));

    if (!w) return;

    if (dynamic_cast<AccordionButton *>(w)) {
        topButton -> hide();
        return;
    }

    while(w -> parent() && w -> parentWidget() != scrollArea -> widget())
        w = w -> parentWidget();

    AccordionCell * cell = dynamic_cast<AccordionCell *>(w);

    if (cell) {
        if (cell -> isCollapsed()) return;

        if (currentCell != cell) {
            if (currentCell) disconnect(currentCell, SLOT(toogleCollapse()));
            currentCell = cell;

            topButton -> setText(currentCell -> title -> text());
            connect(topButton, SIGNAL(clicked()), currentCell, SLOT(toogleCollapse()));
        }
        topButton -> show();
    }
    else topButton -> hide();
}
