#ifndef HOVERABLE_MENU
#define HOVERABLE_MENU

#include <qmenu.h>

class HoverableMenu : public QMenu {
public:
    inline HoverableMenu(QWidget * parent = 0) : QMenu(parent) {}
protected:
    inline void leaveEvent(QEvent *) {
        if (!rect().contains(mapFromGlobal(QCursor::pos()), true))
            hide();
    }
};

#endif // HOVERABLE_MENU

