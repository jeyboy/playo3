#ifndef ACCORDION
#define ACCORDION

#include <qscrollarea.h>
#include <qpushbutton.h>
#include <qboxlayout.h>

struct AccordionButton : public QPushButton {
    AccordionButton(QString name, QWidget * parent = 0) : QPushButton(name, parent) {
        setCheckable(true);
    }
};

struct AccordionCell : public QWidget {
    Q_OBJECT
public:
    AccordionCell(QWidget * parent = 0) : QWidget(parent), item(0), title(0) {}

    AccordionCell(QString name, QWidget * container, QWidget * parent = 0) :
        QWidget(parent), item(container), title(new AccordionButton(name, this))
    {
        connect(title, SIGNAL(clicked()), this, SLOT(toogleCollapse()));

        container -> setParent(this);
        QVBoxLayout * l = new QVBoxLayout;
        l -> setSizeConstraint(QLayout::SetMinAndMaxSize);

        l -> addWidget(title);
        l -> addWidget(item);
        l -> setMargin(0);
        l -> setSpacing(0);

        setLayout(l);
    }

    ~AccordionCell() {}

    QWidget * item;
    QPushButton * title;


    inline bool isCollapsed() { return item -> isHidden(); }
public slots:
    inline void setCollapse(bool collapse) {
        item -> setHidden(collapse);
        title -> setChecked(!collapse);
    }
    inline void toogleCollapse() { setCollapse(!item -> isHidden()); }
};

class Accordion : public QScrollArea {
    Q_OBJECT
public:
    Accordion(QWidget * parent = 0);

    void addItem(QString name, QWidget * item, bool expanded = false);
    QWidget * addItem(QString name, bool expanded = false);
    void removeItem(int index);
    void setItemCollapsed(int index, bool collapsed = true);
    void ensureVisible(int index);
    void activate(int index);
    inline void clear() { qDeleteAll(cells); }
    int indexOf(QWidget * obj);
protected slots:
    void scrollValueChanged(int value);
    void collapseRequired();

private:
    AccordionCell * currentCell;
    AccordionButton * topButton;

    QVBoxLayout * new_layout;
    QList<AccordionCell *> cells;
};

#endif // ACCORDION
