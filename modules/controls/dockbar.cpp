#include "dockbar.h"

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags) {
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("tool_" + title);
    setTitleBarWidget((titleWidget = new WindowTitle(this, 30, QMargins(10, 10, 10, 0), 0, false, false, false)));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);
//    setStyleSheet(
//        "QDockWidget {"
//              "background: rgba(0,0,0,50%);"
////            "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 " + Stylesheets::color1().name(QColor::HexArgb) + ", stop: .36 " + Stylesheets::color2().name(QColor::HexArgb) + ", stop: 1 " + Stylesheets::color3().name(QColor::HexArgb) + ");"
//        "}"
//    );

//    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void DockBar::paintEvent(QPaintEvent * event) {
    if (isFloating()) {
        QPainter painter(this);
//        painter.save();
//        painter.setPen(pen);

        QLinearGradient brush(0, 0, rect().width(), rect().height());
        brush.setColorAt(0, Stylesheets::color1());
        brush.setColorAt(.36, Stylesheets::color2());
        brush.setColorAt(1, Stylesheets::color3());

        painter.setBrush(brush);
        painter.drawRoundedRect(rect(), 12, 12, Qt::AbsoluteSize);
        event -> accept();
//        painter.setPen(bevelPen);
//        painter.restore();
    }
    else QDockWidget::paintEvent(event);
}
