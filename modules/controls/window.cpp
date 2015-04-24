#include "window.h"
#include "misc/stylesheets.h"
#include "dockbar.h"

using namespace Playo3;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    titleHeight(30), doubleBorderWidth(Stylesheets::borderWidth * 2),
    halfBorderWidth(Stylesheets::borderWidth / 2), background(new QPixmap(":main")),
     resizeFlagX(false), resizeFlagY(false), moveFlag(false), inAction(false),
     childInAction(false), skipChildAction(false)
{

    setContentsMargins(doubleBorderWidth, doubleBorderWidth + titleHeight, doubleBorderWidth, doubleBorderWidth);
    setMouseTracking(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setStyleSheet(Stylesheets::mainWindowTabsStyle());

    titleWidget = new WindowTitle(
        false,
        this,
        titleHeight + 6,
        QMargins(doubleBorderWidth, doubleBorderWidth, doubleBorderWidth, 0),
        QMargins(0, 0, 0, 0),
        Stylesheets::borderWidth,
        Stylesheets::borderWidth,
        false, false, false
    );
    titleWidget -> addCustomButton("Most top", QPixmap(":top_off_button"), QPixmap(":top_on_button"), this, SLOT(toggleWindowMostTop()));
    titleWidget -> addMiniButton();
    titleWidget -> addMaxiButton();
    titleWidget -> addCloseButton();

    if (parent == 0) {
        QString tooltip(
            "<p><b>" + QApplication::applicationName() + "<b></p>"
            "<p><b>Version:<b> " + QApplication::applicationVersion() + "</p>"
            "<p><b>Developer:<b> Jenua Boiko</p>"
            "<p><b>Site:<b> https://github.com/jeyboy/playo3</p>"
        );

        titleWidget -> setTitleToolTip(tooltip);
    }

    Stylesheets::initBrush(brush);

//    setDocumentMode(true);
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

void MainWindow::toggleWindowMostTop() {
    HoverableLabel * button = (HoverableLabel *)sender();

    if (windowFlags() & Qt::WindowStaysOnTopHint) {
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowStaysOnTopHint;
        setWindowFlags(flags);
        button -> setOff();
    }
    else {
        button -> setOn();
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    }

    show();
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);

    Stylesheets::calcBorderRect(rect(), borderRect);
    titleWidget -> resize(event -> size().width(), titleWidget -> height());

    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());

    int minSide = qMin(rect().width(), (int)(rect().height() - titleHeight)) / 2, minSideHalf = minSide / 2;
    backRect.setRect(rect().width() / 2 - minSideHalf, (rect().height() + titleHeight) / 2 - minSideHalf, minSide, minSide);
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

void MainWindow::mouseReleaseEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton) {
        dropFlags();
        inAction = false;
        repaint(); // refresh border
    }
    QMainWindow::mouseReleaseEvent(event);
}

//TODO: move all events funcs to this
bool MainWindow::event(QEvent * event) {
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

bool MainWindow::eventFilter(QObject * o, QEvent * e) {
    switch(e -> type()) {
        case QEvent::MouseButtonPress: {
            childInAction = true;
            break;}

        case QEvent::MouseButtonRelease: {
            childInAction = false;
            DockBar * bar = qobject_cast<DockBar *>(o);

            if (bar -> isSticked())
                addOuterChild(bar);
            else
                removeOuterChild(bar);
            break;}

//        case QEvent::Resize:
        case QEvent::Move: {
            if (/*e -> type() == QEvent::Resize || */(childInAction && !skipChildAction)) {
                DockBar * bar = qobject_cast<DockBar *>(o);

                QRect parentRect = geometry();
                QRect currRect = bar -> geometry();
                bool change = false;

                if (change |= qAbs(parentRect.right() - currRect.left()) < Stylesheets::stickDistance)
                    currRect.moveLeft(parentRect.right());

                if (!change && (change |= qAbs(parentRect.bottom() - currRect.top()) < Stylesheets::stickDistance))
                    currRect.moveTop(parentRect.bottom());

                if (!change && (change |= qAbs(parentRect.left() - currRect.right()) < Stylesheets::stickDistance))
                    currRect.moveRight(parentRect.left());

                if (!change && (change |= qAbs(parentRect.top() - currRect.bottom()) < Stylesheets::stickDistance))
                    currRect.moveBottom(parentRect.top());

                bar -> setStickedFlag(change);

                if (change) {
                    skipChildAction = true;
                    bar -> setGeometry(currRect);
                    skipChildAction = false;
                }
            }
            break;}
        default: {}
    }

    return QMainWindow::eventFilter(o, e);
}

void MainWindow::mouseMoveEvent(QMouseEvent * event) {
    if (event -> buttons() & Qt::LeftButton) {
        if (resizeFlagX || resizeFlagY) {
            QSize ns(geom.width(), geom.height());

            if (resizeFlagY) {
                if (atBottom)
                    ns.rheight() = geom.height() + (event -> globalPos().y() - dragPos.y());
                else
                    ns.rheight() = geom.height() - (event -> globalPos().y() - dragPos.y());
            }

            if (resizeFlagX) {
                if (atLeft)
                    ns.rwidth() = geom.width() - (event -> globalPos().x() - dragPos.x());
                else
                    ns.rwidth() = geom.width() + (event -> globalPos().x() - dragPos.x());
            }

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

            stickCorrection(nr);

            ///////////////////// sticked child moving /////////////////////////////
            skipChildAction = true;
            QPoint parentOffset = (geometry().topLeft() - nr.topLeft()) + (geometry().bottomRight() - nr.bottomRight());

            foreach(QWidget * w, outerChilds)
                w -> move(w -> geometry().topLeft() - parentOffset);

            skipChildAction = false;
            //////////////////////////////////////////////////

            setGeometry(nr);
            event -> accept();
        }
        else if (moveFlag) {
            QRect newRect(geom); newRect.moveTopLeft(event -> globalPos() - (dragPos - geom.topLeft()));
            stickCorrection(newRect);

            ///////////////////// sticked child moving /////////////////////////////
            skipChildAction = true;
            QPoint parentOffset = (geometry().topLeft() - newRect.topLeft());

            foreach(QWidget * w, outerChilds)
                w -> move(w -> geometry().topLeft() - parentOffset);

            skipChildAction = false;
            //////////////////////////////////////////////////

            move(newRect.topLeft());
            event -> accept();
        } else
            QMainWindow::mouseMoveEvent(event);
    } else
        QMainWindow::mouseMoveEvent(event);
}

void MainWindow::paintEvent(QPaintEvent * event) {
    bool isResizing = (resizeFlagX || resizeFlagY);
    QPainter painter(this);

    painter.setBrush(brush);
    if (!isResizing) {
        painter.setPen(Stylesheets::pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.setPen(isResizing ? Stylesheets::resizePen : Stylesheets::bevelPen);
    painter.drawPixmap(backRect, *background);
    painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);

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
