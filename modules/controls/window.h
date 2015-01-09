#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QMainWindow>
#include <qpainter.h>
#include "modules/controls/window_title.h"
#include "misc/screen.h"

namespace Playo3 {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget * parent = 0);
        ~MainWindow();

        void locationCorrection();

    public slots:
        inline void setWindowTitle(const QString & newTitle) { titleWidget -> setText(newTitle); }

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
                if (!inAction) {
                    if (isResizeable()) {
                        if (atBottom || atTop) {
                            if (atLeft)
                                setCursor(atBottom ? Qt::SizeBDiagCursor : Qt::SizeFDiagCursor);
                            else if (atRight)
                                setCursor(atBottom ? Qt::SizeFDiagCursor : Qt::SizeBDiagCursor);
                            else
                                setCursor(Qt::SizeVerCursor);
                        } else setCursor(Qt::SizeHorCursor);

                        dropFlags();
                    } else
                        setCursor(Qt::ArrowCursor);
                }
            }

            return QMainWindow::event(event);
        }
        void mouseMoveEvent(QMouseEvent *);
        void paintEvent(QPaintEvent *);
    protected:
        WindowTitle * titleWidget;

    private:
        QRect & stickCorrection(QRect & r);
        bool isResizeable();
        inline void dropFlags() { resizeFlagX = resizeFlagY = moveFlag = false; }

        uint titleHeight;
        int doubleBorderWidth, halfBorderWidth;
        QPixmap * background;
        bool resizeFlagX, resizeFlagY, moveFlag, inAction, recursion;
        bool atBottom, atLeft, atRight, atTop;
        QPoint dragPos;
        QRect geom;

        QLinearGradient brush;
        QRect backRect, borderRect, screenRect;
    };
}

#endif // MAIN_WINDOW
