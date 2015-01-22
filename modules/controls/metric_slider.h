#ifndef METRIC_SLIDER_H
#define METRIC_SLIDER_H

#include "clickable_slider.h"
#include <QPainter>
#include <QToolTip>
#include <QLinearGradient>

#include "misc/settings.h"

namespace Playo3 {
    class MetricSlider : public ClickableSlider {
        Q_OBJECT
    public:
        MetricSlider(QWidget * parent = 0, bool showPosition = true);

        inline void setMinimum(int min) {
            ClickableSlider::setMinimum(min);
            calcGrid();
        }
        inline void setMaximum(int max) {
            ClickableSlider::setMaximum(max);
            calcGrid();
            repaint();
        }

    protected:
        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);

        void calcGrid();
    private:
        bool show_position, show_mini_progress;
        int margin, spacing, point_radius;
        float hVal, fVal;
        QRect rRect;
        QPainterPath path;
    };
}

#endif // METRIC_SLIDER_H
