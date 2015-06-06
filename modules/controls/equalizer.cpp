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

    enabled = new QCheckBox("On", this);
    connect(enabled, SIGNAL(toggled(bool)), Player::instance(), SLOT(registerEQ(bool)));
    l -> addWidget(enabled, 0, 0, 1, 3, Qt::AlignCenter);


    QPushButton * reset = new QPushButton("reset", this);
    connect(reset, SIGNAL(clicked()), this, SLOT(reset()));
    l -> addWidget(reset, 0, 3, 1, 3, Qt::AlignCenter);

    presetsList = new QComboBox(this);
    connect(presetsList, SIGNAL(currentTextChanged(QString)), this, SLOT(presetChanged(QString)));
    l -> addWidget(presetsList, 0, 6, 1, 7, Qt::AlignCenter);

    QPushButton * save = new QPushButton("save", this);
    connect(save, SIGNAL(clicked()), this, SLOT(createPreset()));
    l -> addWidget(save, 0, 13, 1, 3, Qt::AlignCenter);

    QPushButton * remove = new QPushButton("remove", this);
    connect(remove, SIGNAL(clicked()), this, SLOT(removePreset()));
    l -> addWidget(remove, 0, 16, 1, 3, Qt::AlignCenter);
}

Equalizer::~Equalizer() {}

QJsonObject Equalizer::settings() {
    QJsonObject res;
    QJsonObject presetsNode;

    QMap<QString, QMap<int, int> >::Iterator preset = presets.begin();

    for(; preset != presets.end(); preset++) {
        QJsonObject curr_preset;

        for(QMap<int, int>::Iterator gain = preset.value().begin(); gain != preset.value().end(); gain++)
            curr_preset.insert(QString::number(gain.key()), gain.value());

        presetsNode.insert(preset.key(), curr_preset);
    }

    res.insert("presets", presetsNode);
    res.insert("active", presetsList -> currentText());
    res.insert("enabled", enabled -> isChecked());

    return res;
}
void Equalizer::setSettings(QJsonObject settings) {
    QJsonObject presetsNode = settings.value("presets").toObject();

    foreach(QString key, presetsNode.keys()) {
        QJsonObject presetVals = presetsNode.value(key).toObject();
        QMap<int, int> gains;

        foreach(QString gKey, presetVals.keys())
            gains.insert(gKey.toInt(), presetVals.value(gKey).toInt());

        presets.insert(key, gains);
    }

    if (presets.isEmpty())
        presets.insert("Manual", QMap<int, int>());

    presetsList -> insertItems(0, presets.keys());

    presetsList -> setCurrentText(settings.value("active").toString());
    enabled -> setChecked(settings.value("enabled").toBool());
}

void Equalizer::createPreset() {
    ToolbarDialog dialog("New preset name", this);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();

        if (name.isEmpty()) return; // TODO: output error
        bool isNew = !presets.contains(name);

        presets.insert(name, Player::instance() -> eqGains());

        if (isNew)
            presetsList -> insertItem(0, name);
        presetsList -> setCurrentText(name);
    }
}

void Equalizer::removePreset() {
    if (presetsList -> currentText() != "Manual")
        presetsList -> removeItem(presetsList -> currentIndex());
}

void Equalizer::presetChanged(QString name) {
    QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
    QList<ClickableSlider *>::Iterator slider = sliders.begin();
    QMap<int, int> gains = presets.value(name);

    for(; slider != sliders.end(); slider++)
        (*slider) -> setValue(gains.value((*slider) -> property("num").toInt()));
}

void Equalizer::eqValueChanged(int val) {
    QSlider * slider = (QSlider *)sender();
    int pos = slider -> property("num").toInt();
    Player::instance() -> setEQBand(pos, val / 15.0);
    presets[presetsList -> currentText()].insert(pos, val);
}

void Equalizer::reset() {
    QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
    QList<ClickableSlider *>::Iterator slider = sliders.begin();

    for(; slider != sliders.end(); slider++)
        (*slider) -> setValue(0);
}
