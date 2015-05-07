#ifndef HOVERABLE_MENU
#define HOVERABLE_MENU

#include <qmenu.h>

class HoverableMenu : public QMenu {
    Q_OBJECT
public:
    inline HoverableMenu(QWidget * parent = 0) : QMenu(parent) {}
public slots:
    inline void close() { hide(); }

protected:
    inline void leaveEvent(QEvent *) {
        if (!rect().contains(mapFromGlobal(QCursor::pos()), true))
            close();
    }
};

#endif // HOVERABLE_MENU

