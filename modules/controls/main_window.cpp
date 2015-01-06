#include "main_window.h"
#include <QGridLayout>
//#include <QApplication>

using namespace Playo3;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    borderWidth(6), doubleBorderWidth(borderWidth * 2), halfBorderWidth(borderWidth / 2),
    radius(12), titleHeight(30), background(new QPixmap(":main")),
    resizeFlagX(false), resizeFlagY(false),
    moveFlag(false), inAction(false), brush(0) {

    setContentsMargins(doubleBorderWidth, doubleBorderWidth + titleHeight, doubleBorderWidth, doubleBorderWidth);
    setMouseTracking(true);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_PaintOnScreen);

    QVector<qreal> penPattern;
    penPattern.append(1); penPattern.append(halfBorderWidth);

    pen.setColor(QColor::fromRgb(255, 255, 255));
    pen.setWidth(halfBorderWidth);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);

    bevelPen.setColor(QColor::fromRgb(23, 23, 23));
    bevelPen.setWidth(halfBorderWidth);
    bevelPen.setCosmetic(true);
    bevelPen.setStyle(Qt::DashLine);
    bevelPen.setJoinStyle(Qt::RoundJoin);
    bevelPen.setDashPattern(penPattern);

    initMenuWidget();
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
        inAction = true;
        moveFlag = !isResizeable();
        dragPos = event -> globalPos();
        geom = geometry();
        event -> accept();
    } else QMainWindow::mousePressEvent(event);
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
            event -> accept();
        }
        else if(moveFlag) {
            move(event -> globalPos() - (dragPos - geom.topLeft()));
            event -> accept();
        } else
            QMainWindow::mouseMoveEvent(event);
    } else
        QMainWindow::mouseMoveEvent(event);
}

void MainWindow::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.save();
    painter.setPen(pen);
    painter.setBrush(*brush);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.setPen(bevelPen);
    painter.drawPixmap(backRect, *background);
    painter.drawRoundedRect(borderRect, radius, radius, Qt::AbsoluteSize);
    painter.restore();

    QMainWindow::paintEvent(event);
}

bool MainWindow::isResizeable() {
    QPoint pos = mapFromGlobal(QCursor::pos());

    atLeft = pos.x() >= 0 && pos.x() <= doubleBorderWidth;
    atRight = pos.x() >= width() - doubleBorderWidth && pos.x() <= width();
    atTop = pos.y() >= 0 && pos.y() <= doubleBorderWidth;
    atBottom = pos.y() >= height() - doubleBorderWidth && pos.y() <= height();

    resizeFlagX = atLeft || atRight;
    resizeFlagY = atTop || atBottom;

    return resizeFlagX || resizeFlagY;
}

void MainWindow::initMenuWidget() {
    menuWidget = new QWidget(this);
    menuWidget -> setObjectName("TitleBar");
    menuWidget -> setContentsMargins(doubleBorderWidth, borderWidth, doubleBorderWidth, 0);
    menuWidget -> setMouseTracking(true);
    menuWidget -> setStyleSheet("#TitleBar { border-bottom: 1px solid white; margin: 0 " + QString::number(doubleBorderWidth) + "px 0 " + QString::number(doubleBorderWidth) + "px; }");
    QGridLayout * l = new QGridLayout(menuWidget);
    l -> setContentsMargins(0, 0, 0, 0);
//    l -> setMargin(0);
//    l -> setSpacing(0);

    titleLabel = new QLabel("The coolest text in the world", menuWidget);
    l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    l -> addWidget(
                new ClickableLabel(QPixmap(":mini_button").scaled(titleHeight * 2, titleHeight - borderWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation), this, SLOT(showMinimized()), menuWidget),
                0, 1, Qt::AlignRight | Qt::AlignVCenter);
    l -> addWidget(
                new ClickableLabel(QPixmap(":maxi_button").scaled(titleHeight * 2, titleHeight - borderWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation), this, SLOT(showMaximized()), menuWidget),
                0, 2, Qt::AlignRight | Qt::AlignVCenter);
    l -> addWidget(
                new ClickableLabel(QPixmap(":close_button").scaled(titleHeight * 2, titleHeight - borderWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation), this, SLOT(close()), menuWidget),
                0, 3, Qt::AlignRight | Qt::AlignVCenter);

    l -> setColumnStretch(0, 10);
    l -> setColumnStretch(1, 0);
    l -> setColumnStretch(2, 0);
    l -> setColumnStretch(3, 0);

//    setMenuWidget(menuWidget);
}
