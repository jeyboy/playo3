#ifndef DOCKBAR
#define DOCKBAR

#include <qdockwidget.h>

#include "modules/controls/window.h"
#include "settings.h"

namespace Controls {
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
        DockBar(const QString & title, QWidget * parent = 0, bool closable = true, Qt::WindowFlags flags = 0, const QString & objName = QString());
//        ~DockBar() { Settings::unregisterTransparentWidget(this); }
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

        inline void initiateSearch() { titleWidget -> initiateSearch(mainWidget(), SLOT(startInnerSearch(QString)), SLOT(endInnerSearch()), SIGNAL(searchFinished())); }

        inline QWidget * mainWidget() { return inProcess ? mWidget : widget(); }
        void useVerticalTitles(bool vertical);
        inline bool isUsedVerticalTitles() const { return titleWidget -> isVertical(); }

    signals:
        void closing();
    public slots:
        inline void showSearch() { titleWidget -> showSearch(); }

        inline void rotate() { useVerticalTitles(!isUsedVerticalTitles()); }

        inline void onSetDefaultIco(const QImage & ico) { titleWidget -> setIco(ico); }
        inline void onMoveInBackgroundProcess() { titleWidget -> showProgress(true); }
        inline void onMoveOutBackgroundProcess() { titleWidget -> showProgress(false); }
        inline void onSetBackgroundProgress(int percent) {
            if (!titleWidget -> isShowProgress())
                onMoveInBackgroundProcess();
            titleWidget -> setProgress(percent);
        }

        void onMoveInProcess();
        void onMoveOutProcess();
        void onSetProgress(int percent);
        void onSetProgress2(int percent);

        inline void toggleFloating() { setFloating(!isFloating()); }
        inline void floatingChanged(bool floating) { if (!floating) setTabBarSettings(); }
        inline void onDockLocationChanged(Qt::DockWidgetArea area) { if (area != Qt::NoDockWidgetArea) setTabBarSettings(); }
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
        QWidget * mWidget;
        Spinner * spinner;
    };
}

#endif // DOCK_BAR
