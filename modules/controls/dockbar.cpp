#include "dockbar.h"

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags) {
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("tool_" + title);
    setTitleBarWidget((titleWidget = new WindowTitle(this, 30, QMargins(10, 10, 10, 0), 0, false, false, false)));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

//    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Stylesheets::initBrush(brush);
}

void DockBar::resizeEvent(QResizeEvent * event) {
    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());

    Stylesheets::calcBorderRect(rect(), borderRect);

    QDockWidget::resizeEvent(event);
}

void DockBar::paintEvent(QPaintEvent * event) {
    if (isFloating()) {
        QPainter painter(this);
//        painter.save();
        painter.setBrush(brush);
        painter.setPen(Stylesheets::pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
        painter.setPen(Stylesheets::bevelPen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);

        event -> accept();
//        painter.restore();
    }
    else QDockWidget::paintEvent(event);
}
