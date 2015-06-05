#include "equalizer.h"

using namespace Playo3;

Equalizer::Equalizer(QWidget * parent) : QWidget(parent) {
    setObjectName("tool_equalizer");

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedHeight(140);
    setMinimumWidth(240);

    QGridLayout * l = new QGridLayout(this);

    QMap<int, QString> bands = Player::instance() -> bands();

    QMap<int, QString>::Iterator band = bands.begin();
    for(int num = 0; band != bands.end(); band++, num++) {
        ClickableSlider * slider = new ClickableSlider(this);
        slider -> setOrientation(Qt::Vertical);
        slider -> setMinimum(-150); slider -> setMaximum(150);
        slider -> setProperty("num", num);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(eqValueChanged(int)));
        l -> addWidget(slider, 1, num, Qt::AlignCenter);
        QLabel * label = new QLabel(band.value());
        label -> setAlignment(Qt::AlignCenter);
        label -> setWordWrap(true);
        l -> addWidget(label, 2, num, Qt::AlignCenter);

        l -> setColumnStretch(num, 1);
    }

    enabled = new QCheckBox(this);
    connect(enabled, SIGNAL(toggled(bool)), Player::instance(), SLOT(registerEQ(bool)));
    l -> addWidget(enabled, 0, 0, Qt::AlignCenter);


    QPushButton * reset = new QPushButton("reset", this);
    connect(reset, SIGNAL(clicked()), this, SLOT(reset()));
    l -> addWidget(reset, 0, 1, 1, 3, Qt::AlignCenter);
}

Equalizer::~Equalizer() {}

QJsonObject Equalizer::settings() {
    QMap<int, int> gains = Player::instance() -> eqGains();
    QMap<int, int>::Iterator gain = gains.begin();

    QJsonObject res;
    for(; gain != gains.end(); gain++)
        res.insert(QString::number(gain.key()), gain.value());

    res.insert("enabled", enabled -> isChecked());

    return res;
}
void Equalizer::setSettings(QJsonObject settings) {
    QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
    QList<ClickableSlider *>::Iterator slider = sliders.begin();

    for(; slider != sliders.end(); slider++)
        (*slider) -> setValue(settings.value(QString::number((*slider) -> property("num").toInt())).toInt() * 15);

    enabled -> setChecked(settings.value("enabled").toBool());
}

void Equalizer::eqValueChanged(int val) {
    QSlider * slider = (QSlider *)sender();
    Player::instance() -> setEQBand(slider -> property("num").toInt(), val / 15.0);
}

void Equalizer::reset() {
    QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
    QList<ClickableSlider *>::Iterator slider = sliders.begin();

    for(; slider != sliders.end(); slider++)
        (*slider) -> setValue(0);
}
