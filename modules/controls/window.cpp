#include "window.h"
#include "misc/stylesheets.h"
#include "modules/controls/dockbar.h"

using namespace Playo3;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    titleHeight(30), doubleBorderWidth(Stylesheets::borderWidth * 2),
    halfBorderWidth(Stylesheets::borderWidth / 2),
    background(new QPixmap(":main")), resizeFlagX(false),
    resizeFlagY(false), moveFlag(false), inAction(false) {

    setContentsMargins(doubleBorderWidth, doubleBorderWidth + titleHeight, doubleBorderWidth, doubleBorderWidth);
    setMouseTracking(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setStyleSheet(Stylesheets::mainWindowTabsStyle());
    titleWidget = new WindowTitle(this, titleHeight + 6, QMargins(doubleBorderWidth, doubleBorderWidth, doubleBorderWidth, 0), Stylesheets::borderWidth);

    Stylesheets::initBrush(brush);

//    setTabPosition(Qt::DockWidgetAreas areas, QTabWidget::TabPosition tabPosition);
    setDockNestingEnabled(true);
    setDockOptions(AnimatedDocks | AllowNestedDocks | VerticalTabs);
    setDocumentMode(true);
}

MainWindow::~MainWindow() {
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

bool MainWindow::eventFilter(QObject * target, QEvent * event) {
    if (event -> type() == QEvent::Move) {
        DockBar * bar = (DockBar *)target;

        if (bar) {
            QMoveEvent * e = (QMoveEvent *) event;
            qDebug() << "KL " << (e -> oldPos() - e -> pos());
            bar -> move(bar -> geometry().topLeft() + (e -> oldPos() - e -> pos()));
        }
    }

    return QMainWindow::eventFilter(target, event);
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    titleWidget -> resize(event -> size().width(), titleWidget -> height());

    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());

    int minSide = qMin(rect().width(), (int)(rect().height() - titleHeight)) / 2, minSideHalf = minSide / 2;
    backRect.setRect(rect().width() / 2 - minSideHalf, (rect().height() + titleHeight) / 2 - minSideHalf, minSide, minSide);

    Stylesheets::calcBorderRect(rect(), borderRect);
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
    painter.setBrush(brush);
    painter.setPen(Stylesheets::pen);
    painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    painter.setPen(Stylesheets::bevelPen);
    painter.drawPixmap(backRect, *background);
    painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    painter.restore();
//    event -> accept();

    QMainWindow::paintEvent(event);
}

QRect & MainWindow::stickCorrection(QRect & rect) {
    if (qAbs(screenRect.right() - rect.right()) < Stylesheets::stickDistance)
        rect.moveRight(screenRect.right());

    if (qAbs(screenRect.bottom() - rect.bottom()) < Stylesheets::stickDistance)
        rect.moveBottom(screenRect.bottom());

    if (qAbs(screenRect.left() - rect.left()) < Stylesheets::stickDistance)
        rect.moveLeft(screenRect.left());

    if (qAbs(screenRect.top() - rect.top()) < Stylesheets::stickDistance)
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
