#ifndef CLICKABLE_SLIDER_H
#define CLICKABLE_SLIDER_H

#include <QSlider>

class ClickableSlider : public QSlider {
    Q_OBJECT
public:
    ClickableSlider(QWidget * parent = 0);
};

#endif // CLICKABLE_SLIDER_H
