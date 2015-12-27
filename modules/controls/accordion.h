#ifndef ACCORDION
#define ACCORDION

#include <qscrollarea.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qbuttongroup.h>

namespace Controls {
    struct AccordionCell : public QWidget {
        Q_OBJECT
    public:
        AccordionCell(QWidget * parent = 0) : QWidget(parent), item(0), title(0) {}

        AccordionCell(const QString & name, QWidget * container, QWidget * parent = 0) :
            QWidget(parent), item(container), title(new QPushButton(name, this))
        {
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
    signals:
        void collapsed(bool);
    public slots:
        inline void setCollapse(bool collapse) {
            item -> setHidden(collapse);
            title -> setChecked(!collapse);
            emit collapsed(collapse);
        }
        inline void toogleCollapse() { setCollapse(!item -> isHidden()); }
    };

    class Accordion : public QScrollArea {
        Q_OBJECT

        enum TabState {
            collapse = 1,
            expand,
            toggle
        };

        bool exclusive, toggleable;
        AccordionCell * currentCell;
        QPushButton * topButton;

        QVBoxLayout * new_layout;
        QList<AccordionCell *> cells;
        QButtonGroup * tabs;
    public:
        Accordion(QWidget * parent = 0);

        void addItem(const QString & name, QWidget * item, bool expanded = false);
        QWidget * addItem(const QString & name, bool expanded = false);
        void removeItem(int index);
        void setItemCollapsed(int index, bool collapsed = true);
        void ensureVisible(int index);
        void activate(int index);
        inline void clear() { qDeleteAll(cells); }
        int indexOf(QWidget * obj);

        inline bool isExclusive() const { return exclusive; }
        void setExclusive(bool is_exclusive);

        inline bool isToggleable() const { return toggleable; }
        void setToggleable(bool is_toggleable) { toggleable = is_toggleable; }

        void collapseAll() {
            bool excl = exclusive;
            setExclusive(false);

            for(QList<AccordionCell *>::Iterator cell = cells.begin(); cell != cells.end(); cell++)
                tabActivated((int)*cell, collapse);

            setExclusive(excl);
        }

    signals:
        void currentChanged(int uid);
    protected slots:
        void scrollValueChanged(int value);
        void collapseRequired();
        void tabActivated(int, TabState act = toggle);
    };
}

#endif // ACCORDION
