#include "clickable_slider.h"
#include "settings.h"
#include "toolbars.h"
#include <qstylepainter.h>

using namespace Controls;

ClickableSlider::ClickableSlider(int default_value, QWidget * parent, const QString & css_rule) : QSlider(parent), default_value(default_value) {
    setStyle(slider_style = new SliderStyle());
    if (!css_rule.isEmpty()) {
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
        QStringList parts = css_rule.split('|', QString::SkipEmptyParts);

        rule = parts.first();
        low_level = parts[1].toInt();
        up_level = parts.last().toInt();
    }

    if (default_value != -1)
        setContextMenuPolicy(Qt::DefaultContextMenu);
}

void ClickableSlider::paintEvent(QPaintEvent * ev) {
    TickPosition tpos = tickPosition();

    if (tpos == NoTicks) {
        QSlider::paintEvent(ev);
        return;
    }

    //TODO: improve me
    QStylePainter p(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect handle = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect main_rect = rect();

    int min = minimum();
    int max = maximum();

    double r = 3;
    double range = (double)(max - min);
    double work_width = (double)(width() - handle.width());
    double hanle_half = (double)(handle.width() / 2.0);

    QPointF tcenter(0, main_rect.top() + r / 2);
    QPointF bcenter(0, main_rect.bottom() - r / 2);

    // draw tick marks // do this manually because they are very badly behaved with style sheets
    int interval = tickInterval();
    if (interval == 0)
        interval = pageStep();

    QColor color = Settings::currentStyle -> pen.color();
    p.setPen(QPen(color, r));
    QPolygonF points;

    int intervals_amount = (range - 1) / interval;
    double interval_step = ((double)interval / range) * work_width + hanle_half;

    for (int i = 0; i < intervals_amount; ++i) {
        if (tpos == TicksBothSides || tpos == TicksAbove)
            points << (tcenter += QPointF(interval_step, 0));

        if (tpos == TicksBothSides || tpos == TicksBelow)
             points << (bcenter += QPointF(interval_step, 0));
    }

    p.drawPoints(points);

    // draw the slider (this is basically copy/pasted from QSlider::paintEvent)
    opt.subControls = QStyle::SC_SliderGroove;
    p.drawComplexControl(QStyle::CC_Slider, opt);

    // draw the slider handle
    opt.subControls = QStyle::SC_SliderHandle;
    p.drawComplexControl(QStyle::CC_Slider, opt);
}

void ClickableSlider::contextMenuEvent(QContextMenuEvent * event) {
    QMenu menu(this);

    menu.addAction(QStringLiteral("Set to default"), this, SLOT(toDefault()));
    menu.addSeparator();

    Presentation::ToolBars::obj().createPopupMenu(&menu);

    menu.exec(event -> globalPos());
    event -> accept();
}

void ClickableSlider::valueChanged(int new_val) {
    Settings::currentStyle -> applyProperty(
        this,
        rule.toUtf8(),
        new_val < low_level ? "semi" : new_val > up_level ? "full" : "half"
    );
}

void ClickableSlider::toDefault() { setValue(default_value > maximum() ? maximum() : default_value < minimum() ? minimum() : default_value); }
