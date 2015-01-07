#ifndef DOCKBAR
#define DOCKBAR

#include <QDockWidget>
#include "modules/controls/window_title.h"

namespace Playo3 {
    class DockBar : public QDockWidget {
        Q_OBJECT
    public:
        DockBar(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    signals:
        void activated();

    protected:
        inline void focusInEvent(QFocusEvent * e) {
            emit activated();
            QDockWidget::focusInEvent(e);
        }
//        void focusOutEvent(QFocusEvent *);
    };
}

#endif // DOCK_BAR
