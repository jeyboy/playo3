#ifndef METRIC_SLIDER_H
#define METRIC_SLIDER_H

#include "clickable_slider.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QToolTip>

#include "misc/settings.h"

class MetricSlider : public ClickableSlider {
    Q_OBJECT
public:
    MetricSlider(QWidget * parent = 0, bool showPosition = true);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    QPen pen;
    bool show_position;
    QColor fillColor;
    int margin, spacing, padding;
};

#endif // METRIC_SLIDER_H
