#ifndef HOVERABLE_MENU
#define HOVERABLE_MENU

#include <qmenu.h>

class HoverableMenu : public QMenu {
protected:
    inline void leaveEvent(QEvent *) { hide(); }
};

#endif // HOVERABLE_MENU

