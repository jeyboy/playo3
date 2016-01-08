#ifndef HOVERABLE_MENU
#define HOVERABLE_MENU

#include <qmenu.h>
#include <qdebug.h>

namespace Controls {
    class HoverableMenu : public QMenu {
        Q_OBJECT
    public:
        inline HoverableMenu(QWidget * parent = 0) : QMenu(parent) {}
    public slots:
        inline void close() { hide(); }

    protected:
        void mouseMoveEvent(QMouseEvent *) {
            if (!rect().contains(mapFromGlobal(QCursor::pos())))
                close();
        }

        inline void leaveEvent(QEvent *) {
            if (!rect().contains(mapFromGlobal(QCursor::pos())))
                close();
        }
    };
}

#endif // HOVERABLE_MENU

