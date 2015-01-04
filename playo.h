#ifndef PLAYO_H
#define PLAYO_H

#include <qdebug.h>
#include <QMainWindow>
#include <QPainter>
#include <qevent.h>

namespace Ui {
    class Playo;
}

class Playo : public QMainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget *parent = 0);
    ~Playo();

protected:
    void init();
    void initMenuWidget();

    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);
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
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
private:
    Ui::Playo *ui;

    bool isResizeable();
    inline void dropFlags() { resizeFlagX = resizeFlagY = moveFlag = atLeft = atBottom = false; }

    int borderWidth, radius;
    bool resizeFlagX, resizeFlagY, moveFlag, inAction;
    bool atBottom, atLeft, atRight, atTop;
    QPoint dragPos;
    QRect geom;

    QPen pen;
    QLinearGradient * brush;
    QRect * backRect;
};

#endif // PLAYO_H
