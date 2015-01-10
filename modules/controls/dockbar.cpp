#include "dockbar.h"

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget * parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags) {

    installEventFilter(parent);

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
    else {
        QPen pen(Qt::white);
        pen.setCosmetic(true);
        painter.setPen(pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.restore();
    event -> accept();
}
