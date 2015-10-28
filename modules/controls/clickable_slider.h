#ifndef CLICKABLE_SLIDER_H
#define CLICKABLE_SLIDER_H

#include <qslider.h>

namespace Controls {
    class ClickableSlider : public QSlider {
        Q_OBJECT
    public:
        ClickableSlider(QWidget * parent = 0);
    public slots:
        void setValueSilently(int val) {
            blockSignals(true);
            setValue(val);
            blockSignals(false);
        }

        void setMax(int max) {
            setMaximum(max);
        }
    };
}

#endif // CLICKABLE_SLIDER_H
