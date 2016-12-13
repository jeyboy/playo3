#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "clickable_slider.h"

namespace Controls {
    class ControlPanel : public QFrame {
        Q_OBJECT

        QLabel * timer;
        ClickableSlider * slider;
        QRect region;

        QPushButton * play, * pause, * stop;
    public:
        ControlPanel(QWidget * parent = 0);
        QRect getRegion() const;
        void setRegion(const QRect & rect);
    protected:
        void paintEvent(QPaintEvent *);
    };
}

#endif // CONTROL_PANEL_H
