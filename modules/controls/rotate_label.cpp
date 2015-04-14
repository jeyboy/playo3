#include "rotate_label.h"
#include <qdebug.h>

using namespace Playo3;

void RotateLabel::paintEvent(QPaintEvent * event) {
    if (is_vertical) {
        QPainter painter(this);
        painter.save();
        painter.translate(sizeHint().width(), sizeHint().height() - 1);
        painter.rotate(270);
        painter.drawText(0, 0, text());
        painter.restore();
    }
    else QLabel::paintEvent(event);
}

QSize RotateLabel::minimumSizeHint() const {
    if (is_vertical) {
        QSize s = QLabel::minimumSizeHint();
        return QSize(s.height(), s.width());
    }
    else return QLabel::minimumSizeHint();
}

QSize RotateLabel::sizeHint() const {
    if (is_vertical) {
        QSize s = QLabel::sizeHint();
        return QSize(s.height(), s.width());
    }
    else return QLabel::sizeHint();
}
