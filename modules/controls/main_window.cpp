#include "main_window.h"

using namespace Playo3;

#include "playo.h"
#include "ui_playo.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <qbitmap.h>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    borderWidth(4), radius(12),
    resizeFlagX(false), resizeFlagY(false),
    moveFlag(false), inAction(false), brush(0) {

    setContentsMargins(borderWidth * 2, borderWidth * 2, borderWidth * 2, borderWidth * 2);
    setMouseTracking(true);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_PaintOnScreen);

    QLinearGradient grad(0, 0, rect().width(), 0);
    grad.setColorAt(0, QColor::fromRgb(181,189,200));
    grad.setColorAt(0.5, QColor::fromRgb(40,52,59));
    grad.setColorAt(1, QColor::fromRgb(130,140,149));

    pen.setBrush(grad);
    pen.setWidth(borderWidth);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
}

MainWindow::~MainWindow() {
    delete brush;
    delete backRect;
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    delete brush;
    brush = new QLinearGradient(0, 0, rect().width(), rect().height());
    brush -> setColorAt(0, QColor::fromRgb(181, 189, 200, 224));
    brush -> setColorAt(.36, QColor::fromRgb(130, 140, 149, 224));
    brush -> setColorAt(1, QColor::fromRgb(40, 52, 59, 224));

    backRect = new QRect(rect().width()/2 - rect().width()/4, rect().height()/2 - rect().height()/4, rect().width()/2, rect().height()/2);

//    brush -> setColorAt(0, QColor::fromRgb(207,231,250));
////    brush -> setColorAt(0.5, QColor::fromRgb(241,241,241));
////    brush -> setColorAt(0.51, QColor::fromRgb(225,225,225));
//    brush -> setColorAt(1, QColor::fromRgb(99,147,193));

    QMainWindow::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton) {
        inAction = true;
        moveFlag = !isResizeable();
        dragPos = event -> globalPos();
        geom = geometry();
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent * event) {
    if (event -> buttons() & Qt::LeftButton) {
        if (resizeFlagX || resizeFlagY) {
            QSize ns;
            if (atBottom)
                ns.rheight() = geom.height() + (event -> globalPos().y() - dragPos.y());
            else
                ns.rheight() = geom.height() - (event -> globalPos().y() - dragPos.y());

            if (atLeft)
                ns.rwidth() = geom.width() - (event -> globalPos().x() - dragPos.x());
            else
                ns.rwidth() = geom.width() + (event -> globalPos().x() - dragPos.x());

            ns = QLayout::closestAcceptableSize(this, ns);

            QPoint p;
            QRect nr(p, ns);
            if (atBottom) {
                if (atLeft)
                    nr.moveTopRight(geom.topRight());
                else
                    nr.moveTopLeft(geom.topLeft());
            } else {
                if (atLeft)
                    nr.moveBottomRight(geom.bottomRight());
                else
                    nr.moveBottomLeft(geom.bottomLeft());
            }
            setGeometry(nr);
            QMainWindow::mouseMoveEvent(event);
        }
        else if(moveFlag)
            move(event -> globalPos() - (dragPos - geom.topLeft()));
        else
            QMainWindow::mouseMoveEvent(event);
    } else {

        ////////////// maybe this block must be in event handler////////////////
        if (isResizeable()) {
            if (atBottom || atTop) {
                if (atLeft)
                    setCursor(atBottom ? Qt::SizeBDiagCursor : Qt::SizeFDiagCursor);
                else if (atRight)
                    setCursor(atBottom ? Qt::SizeFDiagCursor : Qt::SizeBDiagCursor);
                else
                    setCursor(Qt::SizeVerCursor);
            } else setCursor(Qt::SizeHorCursor);

//            dropFlags();
        }
        //////////////////////////////

        QMainWindow::mouseMoveEvent(event);
    }
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
//    painter.save();
    painter.setPen(pen);
    painter.setBrush(*brush);
    int offset = borderWidth;
    QRect dRect(rect().x() + offset, rect().y() + offset, rect().width() - offset * 2, rect().height() - offset * 2);
    painter.drawRoundedRect(dRect, radius, radius, Qt::AbsoluteSize);
    painter.drawPixmap(*backRect, QPixmap(":main"));
}

bool MainWindow::isResizeable() {
    QPoint pos = mapFromGlobal(QCursor::pos());

    atLeft = pos.x() >= 0 && pos.x() <= borderWidth * 3;
    atRight = pos.x() >= width() - borderWidth * 3 && pos.x() <= width();
    atTop = pos.y() >= 0 && pos.y() <= borderWidth * 3;
    atBottom = pos.y() >= height() - borderWidth * 3 && pos.y() <= height();

    resizeFlagX = atLeft || atRight;
    resizeFlagY = atTop || atBottom;

    return resizeFlagX || resizeFlagY;
}

