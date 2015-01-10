#include "dockbar.h"
#include <qdebug.h>

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags), sticked(false) {
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("tool_" + title);
    setTitleBarWidget((titleWidget = new WindowTitle(this, 30, QMargins(10, 10, 10, 0), 4, false, false, false)));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Stylesheets::initBrush(brush);
}

void DockBar::resizeEvent(QResizeEvent * event) {
    titleWidget -> resize(event -> size().width(), titleWidget -> height());

    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());

    Stylesheets::calcBorderRect(rect(), borderRect);

    QDockWidget::resizeEvent(event);
}

void DockBar::moveEvent(QMoveEvent * e) {
    if (titleWidget -> inAction()) {
        QRect parentRect = parentWidget() -> geometry();
        QRect currRect = geometry();
        bool change = false;

        if (change |= qAbs(parentRect.right() - currRect.left()) < Stylesheets::stickDistance)
            currRect.moveLeft(parentRect.right());

        if (!change && (change |= qAbs(parentRect.bottom() - currRect.top()) < Stylesheets::stickDistance))
            currRect.moveTop(parentRect.bottom());

        if (!change && (change |= qAbs(parentRect.left() - currRect.right()) < Stylesheets::stickDistance))
            currRect.moveRight(parentRect.left());

        if (!change && (change |= qAbs(parentRect.top() - currRect.bottom()) < Stylesheets::stickDistance))
            currRect.moveBottom(parentRect.top());

        if (change) {
            setGeometry(currRect);
            ((MainWindow *)parentWidget()) -> addOuterChild(this);
        }
        else if (sticked != change)
            ((MainWindow *)parentWidget()) -> removeOuterChild(this);

        sticked = change;
    }

    QDockWidget::moveEvent(e);
}

void DockBar::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.save();

    if (isFloating()) {
        painter.setBrush(brush);
        painter.setPen(Stylesheets::pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
        painter.setPen(Stylesheets::bevelPen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }
    else { //QDockWidget::paintEvent(event);
        painter.setPen(QPen(Qt::white));
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.restore();
    event -> accept();
}
