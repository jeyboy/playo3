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
        if (event -> button() == Qt::LeftButton)
            resizeFlagX = resizeFlagY = moveFlag = false;
        QMainWindow::mouseReleaseEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
private:
    Ui::Playo *ui;

    bool isResizeable();

    int borderWidth, radius;
    bool resizeFlagX, resizeFlagY, moveFlag;
    bool atBottom, atLeft;
    QPoint dragPos;
    QRect geom;
    QPen pen;
    QLinearGradient * brush;
};

#endif // PLAYO_H
