#ifndef ACCORDION
#define ACCORDION

#include <qframe.h>
#include <qpushbutton.h>
#include <QVBoxLayout>

struct AccordionCell : public QWidget {
public:
    AccordionCell(QWidget * parent = 0) : QWidget(parent), item(0), title(0) {}

    AccordionCell(QString name, QWidget * container, QWidget * parent = 0) :
        QWidget(parent), item(container), title(new QPushButton(name, this))
    {
        container -> setParent(this);
        QVBoxLayout * l = new QVBoxLayout(this);

        l -> addWidget(title, 1);
        l -> addWidget(item, 1);
        l -> insertStretch(-1, 2);
    }

    ~AccordionCell() {}

    QWidget * item;
    QPushButton * title;
};

class Accordion : public QFrame {
    Q_OBJECT
public:
    Accordion(QWidget * parent = 0);

    void addItem(QString name, QWidget * item);
    void removeItem(int index);
private:
    QVBoxLayout * new_layout;
    QList<AccordionCell *> cells;
};

#endif // ACCORDION
