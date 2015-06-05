#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <qwidget.h>
#include <qgridlayout.h>
#include "clickable_slider.h"

namespace Playo3 {
    class Equalizer : public QWidget {
        Q_OBJECT

    public:
        Equalizer(QWidget * parent = 0);
        ~Equalizer();

    protected slots:
        void eqValueChanged(int);
    };
}

#endif // EQUALIZER_H
