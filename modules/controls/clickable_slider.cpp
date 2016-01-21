#include "clickable_slider.h"
#include "settings.h"

using namespace Controls;

ClickableSlider::ClickableSlider(QWidget * parent, QString css_rule) : QSlider(parent), rule(css_rule) {
    setStyle(slider_style = new SliderStyle());
    if (!rule.isEmpty())
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(valieChanged(int)));
}

void ClickableSlider::valieChanged(int newVal) {
    int percent = (newVal / (float)maximum() * 100);
    Settings::currentStyle -> applyProperty(this, rule.toUtf8(), percent > 75 ? "full" : percent > 25 ? "half" : "semi");
}
