#include "aspect_ratio_label.h"
#include <qpainter.h>
#include <qdebug.h>

AspectRatioLabel::AspectRatioLabel(QWidget * parent) : QLabel(parent) {
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumHeight(256);
}

void AspectRatioLabel::setPixmap(const QPixmap & p) {
    QLabel::setPixmap(p);
    updateGeometry();
}

QSize AspectRatioLabel::sizeHint() const {
    qDebug() << "SIZE" << QLabel::sizeHint();
    return QLabel::sizeHint();
}

void AspectRatioLabel::paintEvent(QPaintEvent * /*e*/) {
//    QLabel::paintEvent(e);

    const QPixmap * pix = pixmap();

    if (pix) {
        QPainter paint(this);
        QPixmap new_p = pix -> scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int x = (width() - new_p.width()) / 2;
        int y = (height() - new_p.height()) / 2;

        paint.drawPixmap(x, y, new_p);
    }
}
