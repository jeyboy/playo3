#ifndef CLICKABLE_SLIDER_H
#define CLICKABLE_SLIDER_H

#include <qslider.h>
#include <qmenu.h>
#include <qevent.h>

#include "modules/controls/slider_style.h"

namespace Controls {
    class ClickableSlider : public QSlider {
        Q_OBJECT

        SliderStyle * slider_style;
        QString rule;

        int default_value;
    protected:
        void contextMenuEvent(QContextMenuEvent * event);
    public:
        ClickableSlider(int default_value = -1, QWidget * parent = 0, const QString & css_rule = QString());
        ~ClickableSlider() { delete slider_style; }
    public slots:
        void setValueSilently(int val) {
            blockSignals(true);
            setValue(val);
            blockSignals(false);
        }
    protected slots:
        void valieChanged(int newVal);
        void toDefault();
    };
}

#endif // CLICKABLE_SLIDER_H
