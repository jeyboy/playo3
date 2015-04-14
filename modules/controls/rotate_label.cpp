#include "rotate_label.h"

using namespace Playo3;

void RotateLabel::paintEvent(QPaintEvent * event) {
    if (isVertical) {
        QPainter painter(this);
        painter.save();
        painter.setPen(Qt::black);
        painter.setBrush(Qt::Dense1Pattern);

        painter.rotate(-90);
        painter.drawText(0, 0, text());
        painter.restore();
    }
    else QLabel::paintEvent(event);
}

QSize RotateLabel::minimumSizeHint() const {
    if (isVertical) {
        QSize s = QLabel::minimumSizeHint();
        return QSize(s.height(), s.width());
    }
    else return QLabel::minimumSizeHint();
}

QSize RotateLabel::sizeHint() const {
    if (isVertical) {
        QSize s = QLabel::sizeHint();
        return QSize(s.height(), s.width());
    }
    else return QLabel::sizeHint();
}
