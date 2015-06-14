#ifndef ACCORDION
#define ACCORDION

#include <qscrollarea.h>
#include <qpushbutton.h>
#include <QVBoxLayout>

struct AccordionCell : public QWidget {
public:
    AccordionCell(QWidget * parent = 0) : QWidget(parent), item(0), title(0) {}

    AccordionCell(QString name, QWidget * container, QWidget * parent = 0) :
        QWidget(parent), item(container), title(new QPushButton(name, this))
    {
        container -> setParent(this);
        QVBoxLayout * l = new QVBoxLayout;

        l -> addWidget(title);
        l -> addWidget(item);
        l -> setMargin(0);
        l -> setSpacing(0);

        setLayout(l);

//        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        sizePolicy.setHorizontalStretch(1);
//        sizePolicy.setVerticalStretch(1);
//        setSizePolicy(sizePolicy);

    }

    ~AccordionCell() {}

    QWidget * item;
    QPushButton * title;
};

class Accordion : public QScrollArea {
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
