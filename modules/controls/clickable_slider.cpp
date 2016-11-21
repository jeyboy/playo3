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

    QStylePainter p(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect handle = style() -> subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    QRect main_rect = rect();

    int h = 4;
    int ty = main_rect.top();
    int by = main_rect.bottom();

    int min = minimum();
    int max = maximum();
    double range = (double)(max - min);


    // draw tick marks
    // do this manually because they are very badly behaved with style sheets
    int interval = tickInterval();
    if (interval == 0)
        interval = pageStep();

    p.setPen(QPen(QColor("#fff"), 3));
    for (int i = min; i <= max; i += interval) {
        int x = round((double)((double)((double)(i - min) / range) * (double)(width() - handle.width()) + (double)(handle.width() / 2.0))) - 1;

        if (tpos == TicksBothSides || tpos == TicksAbove)
            p.drawLine(x, ty, x, ty + h);

        if (tpos == TicksBothSides || tpos == TicksBelow)
            p.drawLine(x, by, x, by - h);
    }

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
