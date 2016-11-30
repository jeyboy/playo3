#include "window.h"
#include "dockbar.h"

#include <qdebug.h>

using namespace Controls;

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent),
    titleHeight(30), doubleBorderWidth(Settings::currentStyle -> borderWidth * 2),
    halfBorderWidth(Settings::currentStyle -> borderWidth / 2),
     resizeFlagX(false), resizeFlagY(false), moveFlag(false), inAction(false),
     childInAction(false), skipChildAction(false)
{    
    setContentsMargins(doubleBorderWidth, doubleBorderWidth + titleHeight, doubleBorderWidth, doubleBorderWidth);
    setMouseTracking(true);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    if (Settings::currentStyle -> isTransparent())
        setAttribute(Qt::WA_TranslucentBackground, true);

    setAttribute(Qt::WA_AlwaysShowToolTips, true);

    titleWidget = new WindowTitle(
        false,
        this,
        titleHeight + 6,
        QMargins(doubleBorderWidth, doubleBorderWidth, doubleBorderWidth, 0),
        QMargins(0, 0, 0, 0),
        Settings::currentStyle -> borderWidth * 2,
        Settings::currentStyle -> borderWidth * 2,
        false, false, false
    );
    titleWidget -> addCustomButton(QStringLiteral("Most top"), QPixmap(QStringLiteral(":/controls/top_off_button")), QPixmap(QStringLiteral(":/controls/top_on_button")), this, SLOT(toggleWindowMostTop()));
    titleWidget -> addMiniButton();
    titleWidget -> addMaxiButton();
    titleWidget -> addCloseButton();

    if (parent == 0) {
        QString tooltip(
            "<p><b>" % QApplication::applicationName() % "<b></p>"
            "<p><b>Version:<b> " % QApplication::applicationVersion() % "</p>"
            "<p><b>Site:<b> https://github.com/jeyboy/playo3</p>"
            "<p><b>All content is provided solely for information !!!<b></p>"
        );

        titleWidget -> setTitleToolTip(tooltip);
    }

//    setDocumentMode(true);
}

MainWindow::~MainWindow() {}

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

    Settings::currentStyle -> calcBorderRect(rect(), borderRect);
    titleWidget -> resize(event -> size().width(), titleWidget -> height());

    int minSide = qMin(rect().width(), (int)(rect().height() - titleHeight)) / 2, minSideHalf = minSide / 2;
    backRect.setRect(rect().width() / 2 - minSideHalf, (rect().height() + titleHeight) / 2 - minSideHalf, minSide, minSide);
}

void MainWindow::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton) {
//        qDebug() << QApplication::widgetAt(QCursor::pos());
        moveFlag = !isResizeable() && !isMaximized();
        if (moveFlag || resizeFlagX || resizeFlagY) {
            screenRects = Screen::screenRects(this);
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

                if (change |= qAbs(parentRect.right() - currRect.left()) < Settings::currentStyle -> stickDistance)
                    currRect.moveLeft(parentRect.right());

                if (!change && (change |= qAbs(parentRect.bottom() - currRect.top()) < Settings::currentStyle -> stickDistance))
                    currRect.moveTop(parentRect.bottom());

                if (!change && (change |= qAbs(parentRect.left() - currRect.right()) < Settings::currentStyle -> stickDistance))
                    currRect.moveRight(parentRect.left());

                if (!change && (change |= qAbs(parentRect.top() - currRect.bottom()) < Settings::currentStyle -> stickDistance))
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

            QList<QWidget *>::Iterator it = outerChilds.begin();

            for(; it != outerChilds.end(); it++)
                (*it) -> move((*it) -> geometry().topLeft() - parentOffset);

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

            QList<QWidget *>::Iterator it = outerChilds.begin();

            for(; it != outerChilds.end(); it++)
                (*it) -> move((*it) -> geometry().topLeft() - parentOffset);

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
    switch(Settings::currentStyle -> styleType()) {
        case IStylesheets::light: {
            bool isResizing = (resizeFlagX || resizeFlagY);
            QPainter painter(this);

            Settings::currentStyle -> mainBrush.setStart(rect().topLeft());
            Settings::currentStyle -> mainBrush.setFinalStop(rect().bottomRight());
            painter.setBrush(Settings::currentStyle -> mainBrush);
            painter.drawImage(backRect, Settings::currentStyle -> background());

            painter.setPen(isResizing ? Settings::currentStyle -> resizePen : Settings::currentStyle -> pen);

            if (Settings::currentStyle -> isTransparent())
                painter.drawRoundedRect(borderRect, Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);
            else
                painter.drawRect(rect());

        break; }

        case IStylesheets::dark: {
            QPainter painter(this);

            Settings::currentStyle -> initMainBrush(rect());
            painter.setBrush(Settings::currentStyle -> mainBrush);
            painter.setPen(Qt::NoPen);

            if (Settings::currentStyle -> isTransparent())
                painter.drawRoundedRect(borderRect, Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);
            else
                painter.drawRect(rect());

            painter.drawImage(backRect, Settings::currentStyle -> background());
        break; }

        default: { QMainWindow::paintEvent(event); }
    }
}

QRect & MainWindow::stickCorrection(QRect & rect) {
    for(QList<QRect>::Iterator it = screenRects.begin(); it != screenRects.end(); it++) {
        if (qAbs((*it).right() - rect.right()) < Settings::currentStyle -> stickDistance)
            rect.moveRight((*it).right());

        if (qAbs((*it).bottom() - rect.bottom()) < Settings::currentStyle -> stickDistance)
            rect.moveBottom((*it).bottom());

        if (qAbs((*it).left() - rect.left()) < Settings::currentStyle -> stickDistance)
            rect.moveLeft((*it).left());

        if (qAbs((*it).top() - rect.top()) < Settings::currentStyle -> stickDistance)
            rect.moveTop((*it).top());
    }

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
