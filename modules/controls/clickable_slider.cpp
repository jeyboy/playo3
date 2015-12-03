#include "clickable_slider.h"
#include "modules/controls/slider_style.h"

using namespace Controls;

ClickableSlider::ClickableSlider(QWidget * parent) : QSlider(parent) {
    setStyle(new SliderStyle());
}
