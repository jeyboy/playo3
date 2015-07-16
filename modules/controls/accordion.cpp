#include "accordion.h"

Accordion::Accordion(QWidget * parent) : QScrollArea(parent), currentCell(0) {
    topButton = new AccordionButton(QStringLiteral("*"), this);
    topButton -> setFixedSize(16, 16);
    topButton -> hide();
    connect(topButton, SIGNAL(clicked()), this, SLOT(collapseRequired()));

    setWidgetResizable(true);

    QWidget * container = new QWidget(this);
    new_layout = new QVBoxLayout;
    new_layout -> setSizeConstraint(QLayout::SetMinAndMaxSize);
    new_layout -> setContentsMargins(2, 2, 2, 2);
    container -> setLayout(new_layout);
    setWidget(container);
    connect((QObject *)verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));
}

void Accordion::addItem(QString name, QWidget * item, bool expanded) {
//    Q_ASSERT(!item);
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

void Accordion::collapseRequired() {
    if (currentCell) {
        currentCell -> toogleCollapse();
        topButton -> hide();
        new_layout -> setContentsMargins(1, 1, 1, 1);
        ensureWidgetVisible(currentCell -> title, 10, 10);
    }
}

void Accordion::scrollValueChanged(int value) {
    if (value == 0) return;

    QWidget * w = qobject_cast<QWidget *>(childAt(width() / 2, 1));

    if (!w) return;

    if (!dynamic_cast<AccordionButton *>(w)) {
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
