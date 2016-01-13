#include "clickable_slider.h"

using namespace Controls;

ClickableSlider::ClickableSlider(QWidget * parent) : QSlider(parent) {
    setStyle(slider_style = new SliderStyle());
}
