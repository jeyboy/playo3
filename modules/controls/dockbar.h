#ifndef DOCKBAR
#define DOCKBAR

#include <qdockwidget.h>

#include "modules/controls/window.h"
#include "misc/stylesheets.h"
#include "modules/controls/spinner.h"

namespace Playo3 {
    struct TabifyParams {
        TabifyParams() : tabbar(0), index(-1) {}
        TabifyParams(QTabBar * bar, int ind) : tabbar(bar), index(ind) {}

        bool operator == (TabifyParams & oth) { return index == oth.index && tabbar == oth.tabbar; }

        QTabBar * tabbar;
        int index;
    };

    class DockBar : public QDockWidget {
        Q_OBJECT
    public:
        DockBar(const QString & title, QWidget * parent = 0, bool closable = true, Qt::WindowFlags flags = 0);
        inline void setWindowTitle(const QString & newTitle) {
            titleWidget -> setText(newTitle);
            QDockWidget::setWindowTitle(newTitle);
        }
        inline bool isSticked() const { return sticked; }
        inline void setStickedFlag(bool stick) { sticked = stick; }
        void markAsSticked();
//        inline void markAsUnsticked() {
//            sticked = false;
//            if (parent())
//                ((MainWindow *)parentWidget()) -> removeOuterChild(this);
//        }

        inline QWidget * mainWidget() { return inProcess ? mWidget : widget(); }
        void useVerticalTitles(bool vertical);
        inline bool isUsedVerticalTitles() const { return titleWidget -> isVertical(); }

    signals:
        void closing();
    public slots:
        inline void rotate() { useVerticalTitles(!isUsedVerticalTitles()); }
        void onMoveInProcess();
        void onMoveOutProcess();
        void onSetProgress(int percent);
        void onSetProgress2(int percent);

        inline void toggleFloating() { setFloating(!isFloating()); }
        inline void floatingChanged(bool floating) {
            if (floating)
                setTabBarSettings();
        }
        inline void onDockLocationChanged(Qt::DockWidgetArea area) {
            if (area != Qt::NoDockWidgetArea)
                setTabBarSettings();
        }
        TabifyParams tabIndex() const;

    protected:
//        bool event(QEvent *event);
        void resizeEvent(QResizeEvent *);
        void closeEvent(QCloseEvent * e);
        void paintEvent(QPaintEvent *);

    private:
        void setTabBarSettings();

        bool sticked, inProcess;
        WindowTitle * titleWidget;
        QRect borderRect;
        QLinearGradient brush;
        QWidget * mWidget;
        Spinner * spinner;
    };
}

#endif // DOCK_BAR
