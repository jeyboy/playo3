#ifndef CLICKABLE_SLIDER_H
#define CLICKABLE_SLIDER_H

#include <qslider.h>

#include "modules/controls/slider_style.h"

namespace Controls {
    class ClickableSlider : public QSlider {
        Q_OBJECT

        SliderStyle * slider_style;
        QString rule;
    public:
        ClickableSlider(QWidget * parent = 0, QString css_rule = QString());
        ~ClickableSlider() { delete slider_style; }
    public slots:
        void setValueSilently(int val) {
            blockSignals(true);
            setValue(val);
            blockSignals(false);
        }
    protected slots:
        void valieChanged(int newVal);
    };
}

#endif // CLICKABLE_SLIDER_H
