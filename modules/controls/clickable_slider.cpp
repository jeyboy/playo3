#include "clickable_slider.h"
#include "settings.h"

using namespace Controls;

ClickableSlider::ClickableSlider(int default_value, QWidget * parent, const QString & css_rule) : QSlider(parent), default_value(default_value), rule(css_rule) {
    setStyle(slider_style = new SliderStyle());
    if (!rule.isEmpty())
        connect(this, SIGNAL(valueChanged(int)), this, SLOT(valieChanged(int)));

    if (default_value != -1)
        setContextMenuPolicy(Qt::DefaultContextMenu);
}

void ClickableSlider::contextMenuEvent(QContextMenuEvent * event) {
    QMenu menu(this);

    menu.addAction(QStringLiteral("Default"), this, SLOT(toDefault()));

    menu.exec(event -> globalPos());
    event -> accept();
}

void ClickableSlider::valieChanged(int newVal) {
    int percent = (newVal / (float)maximum() * 100);
    Settings::currentStyle -> applyProperty(this, rule.toUtf8(), percent > 75 ? "full" : percent > 25 ? "half" : "semi");
}

void ClickableSlider::toDefault() { setValue(default_value > maximum() ? maximum() : default_value < minimum() ? minimum() : default_value); }
