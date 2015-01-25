#ifndef DOCKBAR
#define DOCKBAR

#include <QDockWidget>
#include "modules/controls/window.h"
#include "misc/stylesheets.h"
#include <qdebug.h>

namespace Playo3 {
    class DockBar : public QDockWidget {
        Q_OBJECT
    public:
        DockBar(const QString &title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
        inline void setWindowTitle(const QString & newTitle) {
            titleWidget -> setText(newTitle);
            QDockWidget::setWindowTitle(newTitle);
        }
        inline bool isSticked() const { return sticked; }
        inline void setSticked(const bool stick) { sticked = stick; }
        inline void markAsSticked() {
            if (parent())
                ((MainWindow *)parentWidget()) -> addOuterChild(this);
        }

    signals:
        void activating();
        void closing();
    public slots:
        inline void toggleFloating() { setFloating(!isFloating()); }
        inline void floatingChanged(bool floating) {
            if (floating)
                setTabBarSettings();
        }
        inline void onDockLocationChanged(Qt::DockWidgetArea area) {
            if (area != Qt::NoDockWidgetArea)
                setTabBarSettings();
        }

    protected:
        void resizeEvent(QResizeEvent *);
        inline void closeEvent(QCloseEvent * e) {
            emit closing();
            ((MainWindow *)parentWidget()) -> removeOuterChild(this);
            QDockWidget::closeEvent(e);
        }

        inline void focusInEvent(QFocusEvent * e) {
            qDebug() << "FOCUSED";
            emit activating();
            QDockWidget::focusInEvent(e);
        }
//        void focusOutEvent(QFocusEvent *);
        void paintEvent(QPaintEvent *);

    private:
        inline void setTabBarSettings() {
            QList<QTabBar *> tabbars = parentWidget() -> findChildren<QTabBar *>(QString(), Qt::FindDirectChildrenOnly);
            foreach(QTabBar * tab, tabbars) {
               tab -> setElideMode(Qt::ElideRight);
               tab -> setContentsMargins(3, 3, 3, 3);
            }
        }

        bool sticked;
        WindowTitle * titleWidget;
        QRect borderRect;
        QLinearGradient brush;
    };
}

#endif // DOCK_BAR
