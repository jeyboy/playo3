#include "equalizer.h"
#include "media/player.h"

using namespace Playo3;

Equalizer::Equalizer(QWidget * parent) : QWidget(parent) {
    setObjectName("tool_equalizer");

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedHeight(140);
    setMinimumWidth(200);

    QHBoxLayout * l = new QHBoxLayout(this);

    for(int loop1 = 0; loop1 < 18; loop1++) {
        ClickableSlider * slider = new ClickableSlider(this);
        slider -> setOrientation(Qt::Vertical);
        slider -> setMinimum(-100); slider -> setMaximum(100);
        slider -> setProperty("num", loop1);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(eqValueChanged(int)));
        l -> addWidget(slider);
    }
}

Equalizer::~Equalizer() {}

void Equalizer::eqValueChanged(int val) {
    QSlider * slider = (QSlider *)sender();
    Player::instance() -> setEQBand(slider -> property("num").toInt(), val / 10.0);
}
