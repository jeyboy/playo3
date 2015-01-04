#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QMainWindow>
#include <QPainter>
#include <qevent.h>
//#include <qbitmap.h>

namespace Playo3 {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget * parent = 0);
        ~MainWindow();

    protected:
        void resizeEvent(QResizeEvent *);
        void mousePressEvent(QMouseEvent *);
        inline void mouseReleaseEvent(QMouseEvent * event) {
            if (event -> button() == Qt::LeftButton) {
                dropFlags();
                inAction = false;
            }
            QMainWindow::mouseReleaseEvent(event);
        }
        inline bool event(QEvent * event) {
            if (event -> type() == QEvent::HoverMove) {
                if ((resizeFlagX || resizeFlagY) && !inAction)
                    dropFlags();
                else
                    setCursor(Qt::ArrowCursor);
            }

            return QMainWindow::event(event);
        }
        void mouseMoveEvent(QMouseEvent *);
        void paintEvent(QPaintEvent *);
    private:
        bool isResizeable();
        inline void dropFlags() { resizeFlagX = resizeFlagY = moveFlag = false; }

        int borderWidth, radius;
        QPixmap * background;
        bool resizeFlagX, resizeFlagY, moveFlag, inAction;
        bool atBottom, atLeft, atRight, atTop;
        QPoint dragPos;
        QRect geom;

        QPen pen, bevelPen;
        QLinearGradient * brush;
        QRect backRect, borderRect, bevelRect;
    };
}

#endif // MAIN_WINDOW

