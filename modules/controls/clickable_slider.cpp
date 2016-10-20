#include "clickable_slider.h"
#include "settings.h"

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

void ClickableSlider::contextMenuEvent(QContextMenuEvent * event) {
    QMenu menu(this);

    menu.addAction(QStringLiteral("Default"), this, SLOT(toDefault()));

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
