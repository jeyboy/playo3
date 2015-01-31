#include "dockbar.h"
#include <qdatetime.h>

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget * parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags), sticked(false) {

    installEventFilter(parent);

    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName(title + QString::number(QDateTime::currentMSecsSinceEpoch()));
    setTitleBarWidget((titleWidget = new WindowTitle(this, 30, QMargins(10, 0, 10, 0), QMargins(0, 8, 0, 0), 5, 0, false, false, false)));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Stylesheets::initBrush(brush);

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChanged(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
}

//bool DockBar::event(QEvent * event) {
//    qDebug() << event -> type();

//    switch(event -> type()) {
//        case QEvent::WindowActivate:
//        case QEvent::WindowDeactivate:
//            ;
//    }

//    return QDockWidget::event(event);
//}

void DockBar::resizeEvent(QResizeEvent * event) {
    Stylesheets::calcBorderRect(rect(), borderRect);
    QDockWidget::resizeEvent(event);

    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());
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
        QPen pen(Qt::lightGray);
        pen.setCosmetic(true);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.restore();
    event -> accept();
}
