#include "window.h"
#include "misc/stylesheets.h"

using namespace Playo3;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    titleHeight(30), stickDist(12), borderWidth(6),
    doubleBorderWidth(borderWidth * 2), halfBorderWidth(borderWidth / 2),
    radius(12), background(new QPixmap(":main")), resizeFlagX(false),
    resizeFlagY(false), moveFlag(false), inAction(false), brush(0) {

    setContentsMargins(doubleBorderWidth, doubleBorderWidth + titleHeight, doubleBorderWidth, doubleBorderWidth);
    setMouseTracking(true);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_PaintOnScreen);

    setStyleSheet(Stylesheets::mainWindowTabsStyle());
    titleWidget = new WindowTitle(this, titleHeight + 6, QMargins(doubleBorderWidth, doubleBorderWidth, doubleBorderWidth, 0), borderWidth);
}

MainWindow::~MainWindow() {
    delete brush;
    delete background;
}

void MainWindow::locationCorrection() {
    int width, height;
    Screen::screenSize(width, height);
    int left = x(), top = y();

    if (left >= width)
        left = width - 50;

    if (top >= height)
        top = height - 50;

    move(left, top);
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    titleWidget -> resize(event -> size().width(), titleWidget -> height());

    delete brush;
    brush = new QLinearGradient(0, 0, rect().width(), rect().height());
    brush -> setColorAt(0, Stylesheets::color1());
    brush -> setColorAt(.36, Stylesheets::color2());
    brush -> setColorAt(1, Stylesheets::color3());

    int minSide = qMin(rect().width(), (int)(rect().height() - titleHeight)) / 2, minSideHalf = minSide / 2;
    backRect.setRect(rect().width() / 2 - minSideHalf, (rect().height() + titleHeight) / 2 - minSideHalf, minSide, minSide);

    borderRect.setRect(
                rect().x() + halfBorderWidth,
                rect().y() + halfBorderWidth,
                rect().width() - borderWidth,
                rect().height() - borderWidth
    );

    QMainWindow::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton) {
//        qDebug() << QApplication::widgetAt(QCursor::pos());
        moveFlag = !isResizeable() && !isMaximized();
        if (moveFlag || resizeFlagX || resizeFlagY) {
            screenRect = Screen::screenRect(this);
            inAction = true;
            dragPos = event -> globalPos();
            geom = geometry();
            event -> accept();
            return;
        }
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
            setGeometry(stickCorrection(nr));
            event -> accept();
        }
        else if(moveFlag) {
            QRect newRect(geom); newRect.moveTopLeft(event -> globalPos() - (dragPos - geom.topLeft()));
            move(stickCorrection(newRect).topLeft());
            event -> accept();
        } else
            QMainWindow::mouseMoveEvent(event);
    } else
        QMainWindow::mouseMoveEvent(event);
}

void MainWindow::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.save();
    painter.setPen(Stylesheets::pen);
    painter.setBrush(*brush);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.setPen(Stylesheets::bevelPen);
    painter.drawPixmap(backRect, *background);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.restore();
    event -> accept();

//    QMainWindow::paintEvent(event);
}

QRect & MainWindow::stickCorrection(QRect & rect) {
    if (qAbs(screenRect.right() - rect.right()) < (int)stickDist)
        rect.moveRight(screenRect.right());

    if (qAbs(screenRect.bottom() - rect.bottom()) < (int)stickDist)
        rect.moveBottom(screenRect.bottom());

    if (qAbs(screenRect.left() - rect.left()) < (int)stickDist)
        rect.moveLeft(screenRect.left());

    if (qAbs(screenRect.top() - rect.top()) < (int)stickDist)
        rect.moveTop(screenRect.top());

    return rect;
}

bool MainWindow::isResizeable() {
    if (isMaximized()) {
        return (atLeft = atRight = atTop = atBottom = resizeFlagX = resizeFlagY = false);
    } else {
        QPoint pos = mapFromGlobal(QCursor::pos());

        atLeft = pos.x() >= 0 && pos.x() <= doubleBorderWidth;
        atRight = pos.x() >= width() - doubleBorderWidth && pos.x() <= width();
        atTop = pos.y() >= 0 && pos.y() <= doubleBorderWidth;
        atBottom = pos.y() >= height() - doubleBorderWidth && pos.y() <= height();

        resizeFlagX = atLeft || atRight;
        resizeFlagY = atTop || atBottom;

        return resizeFlagX || resizeFlagY;
    }
}
