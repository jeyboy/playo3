#include "metric_slider.h"
#include "player/player_index.h"

//TODO: recalc did not called on changeTicks

using namespace Controls;

MetricSlider::MetricSlider(QWidget * parent, bool showPosition, int default_value) : ClickableSlider(default_value, parent)
  , show_position(showPosition), show_mini_progress(true)
  , spacing(30), point_radius(2) {

    setMouseTracking(show_position);

    PlayerFactory::obj().registerCallback(answer_from_player, this, SIGNAL(downloadingLevelChanged(float)), SLOT(downloadingLevelChanged(float)));
}

void MetricSlider::resizeEvent(QResizeEvent *) {
    initStyleOption(&opt);
    QRect sr = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    halfHandle = (0.5 * sr.width());
    bodyRect = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    int sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = bodyRect.x();
        sliderMax = bodyRect.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = bodyRect.y();
        sliderMax = bodyRect.bottom() - sliderLength + 1;
    }

    bodyRect.moveTopLeft(bodyRect.topLeft() + QPoint(5, 5));
    bodyRect.setSize(bodyRect.size() - QSize(10, 10));

    calcGrid();
}

void MetricSlider::paintEvent(QPaintEvent * event) {
    QSlider::paintEvent(event);

    if (minimum() == maximum()) return;

    QPainter p(this);
    p.save();
    QColor c = QColor::fromRgb(0, 0, 0, 92);

    p.setPen(c);
    p.setBrush(QBrush(c)); // temp

    if (Settings::obj().isMetricShow())
        p.drawPath(path);

    if (show_mini_progress && !progressRect.isEmpty()) {
        p.setPen(QColor::fromRgb(0, 255, 0, 128));
        p.drawRoundedRect(progressRect, 1, 1);
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
            if ((show = (pos >= bodyRect.top() && pos <= bodyRect.bottom() + 1)))
                val = posToVal(ev -> pos().y());
        } else {
            pos = ev -> localPos().x();
            if ((show = (pos >= bodyRect.left() && pos <= bodyRect.right() + 1)))
                val = posToVal(ev -> pos().x());
        }

        if (show)
            QToolTip::showText(ev -> globalPos(), Duration::fromMillis(val));

//        QTime(0, 0, 0).addMSecs(mpPlayer->duration()).toString("HH:mm:ss")
    }

    QSlider::mouseMoveEvent(ev);
}

void MetricSlider::downloadingLevelChanged(float level) {
    if ((progressVal = level) == 1)
        progressRect = QRect();
    else {
        if (orientation() == Qt::Horizontal)
            progressRect = QRect(rect().left() + 6, rect().y(), (rect().width() - 8) * progressVal, 3);
        else
            progressRect = QRect(rect().x(), rect().bottom() - 6, 3, -(rect().height() - 1) * progressVal);
    }

    if (show_mini_progress)
        update(/*progressRect*/);
}

int MetricSlider::posToVal(int pos) const {
    pos -= halfHandle - 1;

    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin, sliderMax - sliderMin, opt.upsideDown);
}

double MetricSlider::valToPos(int pos) const {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;
    int available;

    if (orientation() == Qt::Horizontal)
        available = bodyRect.width() - style() -> pixelMetric(QStyle::PM_SliderLength, &opt, this);
    else
        available = bodyRect.height() - style() -> pixelMetric(QStyle::PM_SliderLength, &opt, this);

    double percent = pos == 0 ? 0 : ((double)pos) / (maximum() - minimum());

    if (opt.upsideDown) {
        return (available * (1.0 - percent));
    } else
        return (available * percent);
}

void MetricSlider::calcGrid() {
    path = QPainterPath();

    int multiplyer = 1, step_val = tickInterval(), vall_offset = tickInterval() < 60000 ? 10 : 1;
    double start_point = valToPos(0), step;

    QString strNum;
    QFont strFont = font();
    strFont.setPixelSize(10);
    QFontMetrics metrics(strFont);

    if (orientation() == Qt::Horizontal) {
        step = valToPos(step_val) - start_point;
        if (bodyRect.width() <= spacing || step == 0) return;

        while(step < spacing)
            step = valToPos(step_val * ++multiplyer) - start_point;

        int center = rect().center().y() + point_radius / 2;
        for(double pos = bodyRect.x() + halfHandle / 2 + step, val = vall_offset; pos - halfHandle / 2 <= bodyRect.width(); pos += step, val += vall_offset) {
            if (!Settings::obj().isMetricNumero()) {
                path.addEllipse(QPoint(pos + point_radius / 2, center), point_radius, point_radius);
            } else {
                strNum = QString::number(val * multiplyer);
                path.addText(pos - metrics.width(strNum) / 2 , bodyRect.center().y() + metrics.height() / 3, strFont, strNum);
            }
        }
    } else {
        step = start_point - valToPos(step_val);
        if (bodyRect.height() <= spacing || step == 0) return;

        while(step < spacing)
            step = start_point - valToPos(step_val * ++multiplyer);

        int center = rect().center().x() + point_radius / 2;

        for(double pos = bodyRect.bottom() - halfHandle / 2 - step, val = vall_offset; pos > bodyRect.y(); pos -= step, val += vall_offset) {
            if (!Settings::obj().isMetricNumero()) {
                path.addEllipse(QPoint(center, pos), point_radius, point_radius);
            } else {
                strNum = QString::number(val * multiplyer);
                path.addText(bodyRect.center().x() - metrics.width(strNum) / 2, pos + metrics.height() / 3, strFont, strNum);
            }
        }
    }
}
