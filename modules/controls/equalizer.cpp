#include "equalizer.h"
#include "media/player.h"

using namespace Playo3;

Equalizer::Equalizer(QWidget * parent) : QWidget(parent) {
    setObjectName("tool_equalizer");

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedHeight(140);
    setMinimumWidth(200);

    QGridLayout * l = new QGridLayout(this);
    QStringList hz;
    hz << "20" << "32" << "64" << "90" << "125" << "160" << "200"
       << "250" << "375" << "500" << "750" << "1k" << "1.5k"
       << "2k" << "3k" << "4k" << "8k" << "16k";

    for(int loop1 = 0; loop1 < 18; loop1++) {
        ClickableSlider * slider = new ClickableSlider(this);
        slider -> setOrientation(Qt::Vertical);
        slider -> setMinimum(-120); slider -> setMaximum(120);
        slider -> setProperty("num", loop1);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(eqValueChanged(int)));
        l -> addWidget(slider, 0, loop1);
        QLabel * label = new QLabel(hz[loop1]);
        label -> setAlignment(Qt::AlignCenter);
        l -> addWidget(label, 1, loop1);
    }

    //TODO: add enable button
    //TODO: add reset button
}

Equalizer::~Equalizer() {}

void Equalizer::eqValueChanged(int val) {
    QSlider * slider = (QSlider *)sender();
    Player::instance() -> setEQBand(slider -> property("num").toInt(), val / 12.0);
}
