#include "slider.h"
#include "media/player.h"
#include "media/duration.h"
#include "misc/stylesheets.h"
#include "modules/controls/slider_style.h"

Slider::Slider(QWidget * parent, bool isPositionSlider) : QSlider(parent) {
    setStyle(new SliderStyle());
    setMouseTracking(isPositionSlider);
    position_slider = isPositionSlider;
    fillColor = QColor::fromRgb(0,0,0);
//    setToolTipDuration(1000);

    setStyleSheet(Stylesheets::sliderStyles());
    margin = 4;
}

//TODO: draw text by QStaticText
void Slider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (!Settings::instance() -> isMetricShow())
        return;

    QPainter p(this);
    p.save();

    p.setPen(QColor::fromRgb(0, 0, 0));
    QRect rect = this -> rect();

    QString strNum;

    double temp = 0, step = ((double)maximum()) / tickInterval();
    int multiplyer = 0;

    if (orientation() == Qt::Horizontal) {
        rect.moveLeft(rect.left() + margin + 1); // +1 border
        rect.setWidth(rect.width() - margin * 2 - 2); // -2 border

        while(temp < 24) {
            temp = ((float)(rect.width())) / (step / ++multiplyer);
        }

        step = temp;
        int bottom = rect.bottom() - 6, h = (rect.height() / 3) - 3;
        double val = multiplyer;

        for(double pos = step + rect.left(); pos <= rect.right() + 0.5; pos += step, val += multiplyer) {
            strNum = QString::number(val);
            p.drawLine(QLineF(pos, bottom - h, pos, bottom));
            if (position_slider)
                p.drawText(pos - 7 * strNum.length() , bottom, strNum);
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rect.left(), rect.y(), rect.width() - 1, 3);
                p.fillRect(rect.left(), rect.y(), (rect.width() - 1) * pos, 3, fillColor);
            }
        }
    } else {
        rect.moveTop(rect.top() + margin + 1); // +1 border
        rect.setHeight(rect.height() - margin * 2 - 2); // -2 border

        while(temp < 24) {
            temp = ((float)(rect.height())) / (step / ++multiplyer);
        }

        step = temp;
        int temp, left = rect.left() + 6, w = (rect.width() / 3) - 3;
        double val = multiplyer;

        for(double pos = step; ; pos += step, val += multiplyer) {
            strNum = QString::number(val);
            temp = rect.bottom() - pos;
            if (temp < rect.top() - 0.5)
                break;

            p.drawLine(QLineF(left, temp, left + w, temp));
            if (position_slider)
                p.drawText(left, temp + 10, strNum);
        }

        if (position_slider) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRect(rect.x(), rect.bottom(), 3, -(rect.height() - 1));
                p.fillRect(rect.x(), rect.bottom(), 3, -((rect.height() - 1) * pos), fillColor);
            }
        }
    }

    p.restore();
}

void Slider::mouseMoveEvent(QMouseEvent * ev) {
    if (hasMouseTracking()) {
        QPointF p = ev -> localPos();
        bool show = false;

        int dur;
        if (orientation() == Qt::Vertical) {
            if ((show = (p.y() > margin && p.y() < height() - margin)))
                dur = maximum() *((height() - margin - p.y()) / (height() - 2 * margin));
        } else {
            if ((show = (p.x() > margin && p.x() < width() - margin)))
                dur = maximum() * ((p.x() - margin) / (width() - 2 * margin));
        }

        if (show)
            QToolTip::showText(ev -> globalPos(), Duration::fromMillis(dur));

    }

    QSlider::mouseMoveEvent(ev);
}
