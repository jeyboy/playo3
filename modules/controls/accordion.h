#ifndef ACCORDION
#define ACCORDION

#include <qscrollarea.h>
#include <qpushbutton.h>
#include <qboxlayout.h>

struct AccordionButton : public QPushButton {
    AccordionButton(QString name, QWidget * parent = 0) : QPushButton(name, parent) {}
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
    inline void setCollapse(bool collapse) { item -> setHidden(collapse); }
    inline void toogleCollapse() { item -> setHidden(!item -> isHidden()); }
};

class Accordion : public QFrame {
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




//    const QRect microFocus = childWidget->inputMethodQuery(Qt::ImCursorRectangle).toRect();
//    const QRect defaultMicroFocus =
//        childWidget->QWidget::inputMethodQuery(Qt::ImCursorRectangle).toRect();
//    QRect focusRect = (microFocus != defaultMicroFocus)
//        ? QRect(childWidget->mapTo(d->widget, microFocus.topLeft()), microFocus.size())
//        : QRect(childWidget->mapTo(d->widget, QPoint(0,0)), childWidget->size());
//    const QRect visibleRect(-d->widget->pos(), d->viewport->size());

//    if (visibleRect.contains(focusRect))
//        return;

//    focusRect.adjust(-xmargin, -ymargin, xmargin, ymargin);

//    if (focusRect.width() > visibleRect.width())
//        d->hbar->setValue(focusRect.center().x() - d->viewport->width() / 2);
//    else if (focusRect.right() > visibleRect.right())
//        d->hbar->setValue(focusRect.right() - d->viewport->width());
//    else if (focusRect.left() < visibleRect.left())
//        d->hbar->setValue(focusRect.left());

//    if (focusRect.height() > visibleRect.height())
//        d->vbar->setValue(focusRect.center().y() - d->viewport->height() / 2);
//    else if (focusRect.bottom() > visibleRect.bottom())
//        d->vbar->setValue(focusRect.bottom() - d->viewport->height());
//    else if (focusRect.top() < visibleRect.top())
//        d->vbar->setValue(focusRect.top());
protected slots:
    void scrollValueChanged(int value);

private:
    AccordionCell * currentCell;
    AccordionButton * topButton;

    QScrollArea * scrollArea;
    QVBoxLayout * new_layout;
    QList<AccordionCell *> cells;
};

#endif // ACCORDION
