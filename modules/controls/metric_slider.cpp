#include "metric_slider.h"
#include "media/player.h"
#include "media/duration.h"
#include <qdebug.h>

//TODO: recalc did not called on changeTicks

using namespace Playo3;

MetricSlider::MetricSlider(QWidget * parent, bool showPosition) : ClickableSlider(parent)
  , show_position(showPosition), show_mini_progress(true)
  , spacing(30), point_radius(2) {

    setMouseTracking(show_position);
    connect(Player::instance(), SIGNAL(downloadEnded()), this, SLOT(repaint()));
}

void MetricSlider::resizeEvent(QResizeEvent *) {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    halfHandle = (0.5 * sr.width());
    bodyRect = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    bodyRect.moveTopLeft(bodyRect.topLeft() + QPoint(5, 5));
    bodyRect.setSize(bodyRect.size() - QSize(10, 10));

    calcGrid();
}

void MetricSlider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (!Settings::instance() -> isMetricShow() || minimum() == maximum()) return;

    QPainter p(this);

    p.save();
    QColor c = QColor::fromRgb(0, 0, 0, 92);

    p.setPen(c);
    p.setBrush(QBrush(c)); // temp

    if (orientation() == Qt::Horizontal) {
        p.drawPath(path);

        if (show_mini_progress) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1)
                p.drawRoundedRect(rect().left() + 6, rect().y(), (rect().width() - 8) * pos, 3, 1, 1);
        }
    } else {
        p.drawPath(path);

        if (show_mini_progress) {
            float pos = Player::instance() -> getRemoteFileDownloadPosition();
            if (Player::instance() -> getSize() > 0 && pos < 1) {
                p.drawRoundedRect(rect().x(), rect().bottom(), 3, -(rect().height() - 1) * pos, 1, 1);
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
        int val;
        double pos;

        if (orientation() == Qt::Vertical) {
            pos = ev -> localPos().y();
            if ((show = (pos >= bodyRect.top() && pos <= bodyRect.bottom() + 1))) {
                val = valueConversion(ev -> pos().y()- (halfHandle - 1), true);
                qDebug() << "SV" << val;
            }
        } else {
            pos = ev -> localPos().x();
            if ((show = (pos >= bodyRect.left() && pos <= bodyRect.right() + 1))) {
                val = valueConversion(ev -> pos().x()- (halfHandle - 1), true);
                qDebug() << "SH" << val;
            }
        }

        if (show)
            QToolTip::showText(ev -> globalPos(), Duration::fromMillis(val));

//        QTime(0, 0, 0).addMSecs(mpPlayer->duration()).toString("HH:mm:ss")
    }

    QSlider::mouseMoveEvent(ev);
}

int MetricSlider::valueConversion(int pos, bool toVal) const {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }

    if (toVal)
        return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin, sliderMax - sliderMin, opt.upsideDown);
    else
        return QStyle::sliderPositionFromValue(minimum(), maximum(), pos, sliderMax - sliderMin, opt.upsideDown) + bodyRect.left();
}

void MetricSlider::calcGrid() {
    path = QPainterPath();

    int multiplyer = 1, step_val = tickInterval(), start_point = valueConversion(0, false);
    int step = valueConversion(step_val, false) - start_point;

    QString strNum;
    QFont strFont = font();
    strFont.setPixelSize(10);
    QFontMetrics metrics(strFont);

    if (orientation() == Qt::Horizontal) {
        if (bodyRect.width() < spacing) return;

        while(step < spacing)
            step = valueConversion(step_val * ++multiplyer, false) - start_point;

        int center = rect().center().y() + point_radius / 2;

        for(int pos = start_point + step, val = 1; pos <= bodyRect.width(); pos += step, val++) {
            if (!Settings::instance() -> isMetricNumero()) {
                path.addEllipse(QPoint(pos, center), point_radius, point_radius);
            } else {
                strNum = QString::number(val * multiplyer - 1);
                path.addText(pos - metrics.width(strNum) / 2 , bodyRect.center().y() + metrics.height() / 3, strFont, strNum);
            }
        }
    } else {
        if (bodyRect.height() < spacing) return;

//        while(temp < spacing)
//            temp = ((float)(bodyRect.height())) / (step / ++multiplyer);

//        step = temp;

//        int left = rect().center().x() + point_radius / 2/*, right = rRect.right() - 4 - point_radius*/;

//        for(double pos = step, val = multiplyer; ; pos += step, val += multiplyer) {
//            temp = bodyRect.bottom() - pos;
//            if (temp < bodyRect.top() - 0.5)
//                break;

//            path.addEllipse(QPoint(left, temp), point_radius, point_radius);
////            path.addEllipse(QPoint(right, temp), point_radius, point_radius);

////            strNum = QString::number(val);
////            path.addText(left + point_radius + 2, temp + metrics.height() / 3, strFont, strNum);
//        }
    }
}
