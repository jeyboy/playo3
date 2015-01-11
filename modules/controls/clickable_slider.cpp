#include "clickable_slider.h"
#include "misc/stylesheets.h"
#include "modules/controls/slider_style.h"

ClickableSlider::ClickableSlider(QWidget * parent)
    : QSlider(parent) {
    setStyle(new SliderStyle());
    setStyleSheet(Stylesheets::sliderStyles());
}
