#include "main_window.h"

using namespace Playo3;

#include <QGridLayout>
//#include <qbitmap.h>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    borderWidth(6), radius(12), background(new QPixmap(":main")),
    resizeFlagX(false), resizeFlagY(false),
    moveFlag(false), inAction(false), brush(0) {

    setContentsMargins(borderWidth * 2, borderWidth * 2 + 20, borderWidth * 2, borderWidth * 2);
    setMouseTracking(true);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_PaintOnScreen);

    QVector<qreal> penPattern;
    penPattern.append(1); penPattern.append(3);

    pen.setColor(QColor::fromRgb(255, 255, 255));
    pen.setWidth(borderWidth / 2);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);


    bevelPen.setColor(QColor::fromRgb(23, 23, 23));
    bevelPen.setWidth(borderWidth / 2);
    bevelPen.setCosmetic(true);
    bevelPen.setStyle(Qt::DashLine);
    bevelPen.setJoinStyle(Qt::RoundJoin);
    bevelPen.setDashPattern(penPattern);

    menuWidget = new QWidget(this);
    menuWidget -> setContentsMargins(0, 5, 0, 0);
    menuWidget -> setMouseTracking(true);
    menuWidget -> setStyleSheet("border-bottom: 1px solid white; margin: 0 " + QString::number(borderWidth * 2) + "px 0 " + QString::number(borderWidth * 2) + "px;");
    (new QGridLayout(menuWidget)) -> setContentsMargins(0, 0, 0, 0);
}

MainWindow::~MainWindow() {
    delete brush;
    delete background;
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    menuWidget -> resize(event -> size().width(), menuWidget -> height());

    delete brush;
    brush = new QLinearGradient(0, 0, rect().width(), rect().height());
    brush -> setColorAt(0, QColor::fromRgb(181, 189, 200, 212));
    brush -> setColorAt(.36, QColor::fromRgb(130, 140, 149, 212));
    brush -> setColorAt(1, QColor::fromRgb(40, 52, 59, 212));

    int minSide = qMin(rect().width(), centralWidget() -> rect().height()) / 2, minSideHalf = minSide / 2;
    backRect.setRect(rect().width() / 2 - minSideHalf, rect().height() / 2 - minSideHalf, minSide, minSide);

    int offset = borderWidth / 2;
    borderRect.setRect(
                rect().x() + offset,
                rect().y() + offset,
                rect().width() - offset * 2,
                rect().height() - offset * 2
    );

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
    } else
        QMainWindow::mouseMoveEvent(event);
}

void MainWindow::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.save();
    painter.drawPixmap(backRect, *background);
    painter.setPen(pen);
    painter.setBrush(*brush);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.setPen(bevelPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.restore();

    QMainWindow::paintEvent(event);
}

bool MainWindow::isResizeable() {
    QPoint pos = mapFromGlobal(QCursor::pos());

    atLeft = pos.x() >= 0 && pos.x() <= borderWidth * 2;
    atRight = pos.x() >= width() - borderWidth * 2 && pos.x() <= width();
    atTop = pos.y() >= 0 && pos.y() <= borderWidth * 2;
    atBottom = pos.y() >= height() - borderWidth * 2 && pos.y() <= height();

    resizeFlagX = atLeft || atRight;
    resizeFlagY = atTop || atBottom;

    return resizeFlagX || resizeFlagY;
}

