#include "metric_slider.h"
#include "media/player.h"
#include "media/duration.h"
#include <qdebug.h>

//TODO: recalc did not called on changeTicks

MetricSlider::MetricSlider(QWidget * parent, bool showPosition) : ClickableSlider(parent)
  , show_position(showPosition), show_mini_progress(false)
  , margin(16), spacing(30), point_radius(2) {

    setMouseTracking(show_position);

    brush.setColorAt(0, QColor::fromRgb(0,0,0));
    brush.setColorAt(1, QColor::fromRgb(0,0,0));
}

void MetricSlider::resizeEvent(QResizeEvent *) {
    if (orientation() == Qt::Vertical)
        hVal = height() - margin;
    else
        hVal = width() - margin;

    fVal = (float)(hVal - margin);

    calcGrid();
}

void MetricSlider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (!Settings::instance() -> isMetricShow() || minimum() == maximum()) return;

    QPainter p(this);
    p.save();
    p.setBrush(QBrush(Qt::black)); // temp

    if (orientation() == Qt::Horizontal) {
        p.drawPath(path);

        if (show_mini_progress) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                brush.setFinalStop(rRect.height(), (rRect.width() - 1) * pos);
                p.drawRoundedRect(rRect.left(), rRect.y(), rRect.width() - 1, 3, 1, 1);
//                p.fillRect(rRect.left(), rRect.y(), (rRect.width() - 1) * pos, 3, fillColor);
            }
        }
    } else {
        p.drawPath(path);

        if (show_mini_progress) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                brush.setFinalStop(rRect.width(), -((rRect.height() - 1) * pos)); //?
                p.drawRoundedRect(rRect.x(), rRect.bottom(), 3, -(rRect.height() - 1), 1, 1);
//                p.fillRect(rRect.x(), rRect.bottom(), 3, -((rRect.height() - 1) * pos), fillColor);
            }
        }
    }

    p.restore();
}

void MetricSlider::mouseMoveEvent(QMouseEvent * ev) {
    int max = maximum();
    if (hasMouseTracking() && minimum() != max) {
        bool show = false;
        int pos;

        float dur;
        if (orientation() == Qt::Vertical) {
            pos = ev -> localPos().y();
            if ((show = (pos > margin && pos < hVal)))
                dur = (hVal - pos) / fVal;
        } else {
            pos = ev -> localPos().x();
            if ((show = (pos > margin && pos < hVal)))
                dur = (pos - margin) / fVal;
        }

        if (show)
            QToolTip::showText(ev -> globalPos(), Duration::fromMillis(max * dur));

//        QTime(0, 0, 0).addMSecs(mpPlayer->duration()).toString("HH:mm:ss")

    }

    QSlider::mouseMoveEvent(ev);
}

void MetricSlider::calcGrid() {
    path = QPainterPath();

//    QString strNum;

    int multiplyer = 0;
    float temp = 0, step = ((float)maximum()) / tickInterval();
    rRect = rect();
//    QFont strFont = font();
//    strFont.setPixelSize(10);
//    QFontMetrics metrics(strFont);

    if (orientation() == Qt::Horizontal) {
        rRect.moveLeft(rRect.left() + margin + 2); // +2 border
        rRect.setWidth(rRect.width() - margin * 2 - 4); // -4 border

        if (rRect.width() < spacing) return;

        while(temp < spacing)
            temp = ((float)(rRect.width())) / (step / ++multiplyer);

        step = temp;

        int bottom = ((rRect.bottom() - rRect.top()) / 2) + point_radius/*, top = rRect.top() + 5 + point_radius*/;

        for(double pos = step + rRect.left(), val = multiplyer; pos <= rRect.right() + 0.5; pos += step, val += multiplyer) {
//            path.addEllipse(QPoint(pos + point_radius, top), point_radius, point_radius);
            path.addEllipse(QPoint(pos + point_radius, bottom), point_radius, point_radius);

//            strNum = QString::number(val);
//            path.addText(pos - metrics.width(strNum) / 2 , bottom - point_radius * 2, strFont, strNum);
        }
    } else {
        rRect.moveTop(rRect.top() + margin + 2); // +2 border
        rRect.setHeight(rRect.height() - margin * 2 - 4); // -4 border

        if (rRect.height() < spacing) return;

        while(temp < spacing)
            temp = ((float)(rRect.height())) / (step / ++multiplyer);

        step = temp;

        int left = ((rRect.right() - rRect.left()) / 2) + point_radius - 1/*, right = rRect.right() - 4 - point_radius*/;

        for(double pos = step, val = multiplyer; ; pos += step, val += multiplyer) {
            temp = rRect.bottom() - pos;
            if (temp < rRect.top() - 0.5)
                break;

            path.addEllipse(QPoint(left, temp), point_radius, point_radius);
//            path.addEllipse(QPoint(right, temp), point_radius, point_radius);

//            strNum = QString::number(val);
//            path.addText(left + point_radius + 2, temp + metrics.height() / 3, strFont, strNum);
        }
    }

    brush.setStart(rRect.topLeft());
    brush.setFinalStop(rRect.topLeft());
}
