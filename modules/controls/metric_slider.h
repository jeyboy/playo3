#ifndef METRIC_SLIDER_H
#define METRIC_SLIDER_H

#include "clickable_slider.h"
#include <QPainter>
#include <QToolTip>
#include <QLinearGradient>
#include <QStyleOptionSlider>

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
            if (max <= 60000)
                setTickInterval(10000);
            else
                setTickInterval(60000);

            calcGrid();
            repaint();
        }
        inline void updateMetric() { calcGrid(); }

    protected:
        int posToVal(int pos) const;
        double valToPos(int pos) const;
        int ticks() const;

        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);

        void calcGrid();
    private:
        QStyleOptionSlider opt;

        bool show_position, show_mini_progress;
        int spacing, point_radius;
        int sliderMin, sliderMax;
        double halfHandle;
        QRect bodyRect;
        QPainterPath path;
    };
}

#endif // METRIC_SLIDER_H
