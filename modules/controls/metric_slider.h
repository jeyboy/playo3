#ifndef METRIC_SLIDER_H
#define METRIC_SLIDER_H

#include "clickable_slider.h"
#include <QPainter>
#include <QToolTip>

#include "misc/settings.h"

class MetricSlider : public ClickableSlider {
    Q_OBJECT
public:
    MetricSlider(QWidget * parent = 0, bool showPosition = true);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    QPen pen;
    bool show_position;
    QColor fillColor;
    int margin, spacing, padding;
    float hVal, fVal;
    QRect rRect;
};

#endif // METRIC_SLIDER_H
