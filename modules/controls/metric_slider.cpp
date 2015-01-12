#include "metric_slider.h"
#include "media/player.h"
#include "media/duration.h"
#include <qdebug.h>

MetricSlider::MetricSlider(QWidget * parent, bool showPosition) : ClickableSlider(parent)
  , show_position(showPosition)
  , fillColor(QColor::fromRgb(0,0,0))
  , margin(16), spacing(30)
  , padding(7) {
    setMouseTracking(show_position);

    pen.setCosmetic(true);
    pen.setWidth(6);
}

void MetricSlider::resizeEvent(QResizeEvent *) {
    rRect = rect();

    if (orientation() == Qt::Vertical) {
        hVal = height() - margin;
        rRect.moveTop(rRect.top() + margin + 2); // +2 border
        rRect.setHeight(rRect.height() - margin * 2 - 4); // -4 border
    } else {
        hVal = width() - margin;
        rRect.moveLeft(rRect.left() + margin + 2); // +2 border
        rRect.setWidth(rRect.width() - margin * 2 - 4); // -4 border
    }

    fVal = (float)(hVal - margin);
}

//TODO: draw text by QStaticText
void MetricSlider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (!Settings::instance() -> isMetricShow() || minimum() == maximum()) return;

    QPainter p(this);
    p.save();
    p.setPen(pen);

    QString strNum;

    int multiplyer = 0;
    float temp = 0, step = ((float)maximum()) / tickInterval();

    if (orientation() == Qt::Horizontal) {
        while(temp < spacing)
            temp = ((float)(rRect.width())) / (step / ++multiplyer);

        step = temp;

        int bottom = rRect.bottom() - 6, h = pen.width() / 2;

        for(double pos = step + rRect.left(), val = multiplyer; pos <= rRect.right() + 0.5; pos += step, val += multiplyer) {
            p.drawPoint(pos + h, bottom);
            if (show_position) {
                strNum = QString::number(val);
                p.drawText(pos - (padding * strNum.length()) , bottom - h * 2, strNum);
            }
        }

        if (show_position) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rRect.left(), rRect.y(), rRect.width() - 1, 3);
                p.fillRect(rRect.left(), rRect.y(), (rRect.width() - 1) * pos, 3, fillColor);
            }
        }
    } else {
        while(temp < spacing)
            temp = ((float)(rRect.height())) / (step / ++multiplyer);

        step = temp;

        int temp, left = rRect.left() + pen.width() + 2, w = pen.width() / 2;

        for(double pos = step, val = multiplyer; ; pos += step, val += multiplyer) {
            temp = rRect.bottom() - pos;
            if (temp < rRect.top() - 0.5)
                break;

            p.drawPoint(left, temp);
            if (show_position) {
                strNum = QString::number(val);
                p.drawText(left + w, temp, strNum);
            }
        }

        if (show_position) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rRect.x(), rRect.bottom(), 3, -(rRect.height() - 1));
                p.fillRect(rRect.x(), rRect.bottom(), 3, -((rRect.height() - 1) * pos), fillColor);
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

    }

    QSlider::mouseMoveEvent(ev);
}
