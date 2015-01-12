#include "dockbar.h"
#include <qdatetime.h>

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget * parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags) {

    installEventFilter(parent);

    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName(title + QString::number(QDateTime::currentMSecsSinceEpoch()));
    setTitleBarWidget((titleWidget = new WindowTitle(this, 30, QMargins(10, 10, 10, 0), 4, false, false, false)));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Stylesheets::initBrush(brush);

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChanged(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
}

void DockBar::resizeEvent(QResizeEvent * event) {
    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());

    QDockWidget::resizeEvent(event);
    Stylesheets::calcBorderRect(rect(), borderRect);
    titleWidget -> resize(width(), titleWidget -> height());
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
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.restore();
    event -> accept();
}
