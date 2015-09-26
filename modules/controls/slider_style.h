#ifndef SLIDER_STYLE_H
#define SLIDER_STYLE_H

#include <qproxystyle.h>

namespace Controls {
    class SliderStyle: public QProxyStyle {
    public:
        virtual int styleHint(StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0) const {
            if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
                return Qt::LeftButton;
            else
                return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    };
}

#endif // SLIDER_STYLE_H
