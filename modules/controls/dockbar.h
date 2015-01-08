#ifndef DOCKBAR
#define DOCKBAR

#include <QDockWidget>
#include "modules/controls/window_title.h"
#include "misc/stylesheets.h"
#include <qdebug.h>

namespace Playo3 {
    class DockBar : public QDockWidget {
        Q_OBJECT
    public:
        DockBar(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
        inline void setWindowTitle(const QString & newTitle) { titleWidget -> setText(newTitle); }        
    signals:
        void activating();
        void closing();
    public slots:
        inline void toggleFloating() { setFloating(!isFloating()); }
        inline void floatingChanged(bool floating) {
            setAttribute(Qt::WA_OpaquePaintEvent, floating);
            setAttribute(Qt::WA_NoSystemBackground, floating);
            setAttribute(Qt::WA_TranslucentBackground, floating);
        }

    protected:
        inline void closeEvent(QCloseEvent * e) {
            emit closing();
            QDockWidget::closeEvent(e);
        }

        inline void focusInEvent(QFocusEvent * e) {
            emit activating();
            QDockWidget::focusInEvent(e);
        }
//        void focusOutEvent(QFocusEvent *);
        void paintEvent(QPaintEvent *);

    private:
        WindowTitle * titleWidget;
    };
}

#endif // DOCK_BAR
