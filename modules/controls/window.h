#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <qmainwindow.h>
#include <qpainter.h>

#include "modules/controls/window_title.h"

namespace Controls {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget * parent = 0);
        ~MainWindow();

        void locationCorrection();

        inline void addOuterChild(QWidget * w) {
            if (!outerChilds.contains(w))
                outerChilds.append(w);
        }
        inline void removeOuterChild(QWidget * w) {
            outerChilds.removeOne(w);
        }

        inline void setTabPosition(QTabWidget::TabPosition tabPosition) {
            QMainWindow::setTabPosition(Qt::AllDockWidgetAreas, tabPosition);
        }

    public slots:
        void toggleWindowMostTop();
        inline void setWindowTitle(const QString & newTitle) { titleWidget -> setText(newTitle); }

    protected:
        bool event(QEvent * event);
        bool eventFilter(QObject *, QEvent *);

        void resizeEvent(QResizeEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent *);
        void paintEvent(QPaintEvent *);
    protected:
        WindowTitle * titleWidget;

    private:
        QList<QWidget *> outerChilds;

        QRect & stickCorrection(QRect & r);
        bool isResizeable();
        inline void dropFlags() { resizeFlagX = resizeFlagY = moveFlag = false; }

        uint titleHeight;
        int doubleBorderWidth, halfBorderWidth;
        QPixmap * background;
        bool resizeFlagX, resizeFlagY, moveFlag, inAction;
        bool childInAction, skipChildAction;
        bool atBottom, atLeft, atRight, atTop;
        QPoint dragPos;

        QLinearGradient brush;
        QRect backRect, borderRect, geom;
        QList<QRect> screenRects;
    };
}

#endif // MAIN_WINDOW
