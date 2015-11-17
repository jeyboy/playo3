#include "equalizer_view.h"
#include "player/player_index.h"

using namespace Controls;

void EqualizerView::initTopLayout(QHBoxLayout * layout) {
    enabled = new QCheckBox(QStringLiteral("On"), this);  
    PlayerFactory::obj().registerCallback(in, enabled, SIGNAL(toggled(bool)), SLOT(activateEQ(bool)));
    layout -> addWidget(enabled, 0, Qt::AlignCenter);

    QPushButton * reset = new QPushButton(QStringLiteral("reset"), this);
    connect(reset, SIGNAL(clicked()), this, SLOT(reset()));
    layout -> addWidget(reset, 1, Qt::AlignCenter);


    presetTypesList = new QComboBox(this);
    presetTypesList -> insertItems(0, PlayerFactory::obj().currPlayer() -> presetTypesList());
    presetTypesList -> setCurrentText(PlayerFactory::obj().currPlayer() -> currentPresetType());
    PlayerFactory::obj().registerCallback(in, presetTypesList, SIGNAL(currentTextChanged(QString)), SLOT(changePresetType(QString)));
    layout -> addWidget(presetTypesList, 3, Qt::AlignCenter);
    PlayerFactory::obj().registerCallback(out, this, SIGNAL(presetTypeChanged()), SLOT(presetTypeChanged()));


    presetsList = new QComboBox(this);
    connect(presetsList, SIGNAL(currentTextChanged(QString)), this, SLOT(presetChanged(QString)));
    layout -> addWidget(presetsList, 3, Qt::AlignCenter);

    QPushButton * save = new QPushButton(QStringLiteral("save"), this);
    connect(save, SIGNAL(clicked()), this, SLOT(createPreset()));
    layout -> addWidget(save, 1, Qt::AlignCenter);

    QPushButton * remove = new QPushButton(QStringLiteral("remove"), this);
    connect(remove, SIGNAL(clicked()), this, SLOT(removePreset()));
    layout -> addWidget(remove, 1, Qt::AlignCenter);
}
void EqualizerView::initBottomLayout(QGridLayout * layout) {
    QMap<float, QString> bands = PlayerFactory::obj().currPlayer() -> bands();
    dbOutput.clear();

    QMap<float, QString>::Iterator band = bands.begin();
    for(int num = 0; band != bands.end(); band++, num++) {
        QLabel * dbLabel = new QLabel("0");
        dbLabel -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(dbLabel, 0, num, Qt::AlignCenter);
        dbOutput << dbLabel;

        ClickableSlider * slider = new ClickableSlider(this);
        slider -> setOrientation(Qt::Vertical);
        slider -> setMinimum(EQ_MIN_INTERVAL); slider -> setMaximum(EQ_MAX_INTERVAL);
        slider -> setProperty("num", num);
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(eqValueChanged(int)));
        layout -> addWidget(slider, 1, num, Qt::AlignCenter);


        QLabel * label = new QLabel(band.value());
        label -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(label, 2, num, Qt::AlignCenter);

        layout -> setColumnStretch(num, 1);
    }
}

EqualizerView::EqualizerView(QWidget * parent) : QWidget(parent), presetChanging(false) {
    setObjectName(QStringLiteral("tool_equalizer"));

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedHeight(200);
    setMinimumWidth(50);

    QVBoxLayout * vLayout = new QVBoxLayout(this);
    QHBoxLayout * topLayout = new QHBoxLayout();
    initTopLayout(topLayout);

    scrollArea = new QScrollArea(this);
    scrollArea -> setWidgetResizable(true);
    scrollArea -> setStyleSheet("background-color: transparent; border: none;");

    presetTypeChanged();

    vLayout -> addLayout(topLayout);
    vLayout -> addWidget(scrollArea);
}

EqualizerView::~EqualizerView() {}

QJsonObject EqualizerView::settings() {
    QJsonObject res;
    QJsonObject presetsNode;

    QMap<QString, QMap<int, int> >::Iterator preset = presets.begin();

    for(; preset != presets.end(); preset++) {
        QJsonObject curr_preset;

        for(QMap<int, int>::Iterator gain = preset.value().begin(); gain != preset.value().end(); gain++)
            curr_preset.insert(QString::number(gain.key()), gain.value());

        presetsNode.insert(preset.key(), curr_preset);
    }

    res.insert(JSON_KEY, presetsNode);
    res.insert(QStringLiteral("active"), presetsList -> currentText());
    res.insert(QStringLiteral("enabled"), enabled -> isChecked());
    res.insert(QStringLiteral("active_preset"), presetTypesList -> currentText());

    return res;
}
void EqualizerView::setSettings(const QJsonObject & json) {
    presetTypesList -> setCurrentText(json.value(QStringLiteral("active_preset")).toString());

    QJsonObject presetsNode = json.value(JSON_KEY).toObject();

    foreach(QString key, presetsNode.keys()) {
        QJsonObject presetVals = presetsNode.value(key).toObject();
        QMap<int, int> gains;

        foreach(QString gKey, presetVals.keys())
            gains.insert(gKey.toInt(), presetVals.value(gKey).toInt());

        presets.insert(key, gains);
    }

    if (presets.isEmpty())
        presets.insert(DEFAULT_PRESET, QMap<int, int>());

    presetsList -> insertItems(0, presets.keys());

    presetsList -> setCurrentText(json.value(QStringLiteral("active")).toString());
    enabled -> setChecked(json.value(QStringLiteral("enabled")).toBool());
}

void EqualizerView::createPreset() {
    UserActionDialog dialog(this);
    dialog.buildPresetForm();

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getValue(dialog.name_key);

        if (name.isEmpty()) return; // TODO: output error
        bool isNew = !presets.contains(name);

        presets.insert(name, presets.value(presetsList -> currentText()));

        if (presetsList -> currentText() == DEFAULT_PRESET)
            presets.insert(DEFAULT_PRESET, QMap<int, int>());

        if (isNew)
            presetsList -> insertItem(0, name);
        presetsList -> setCurrentText(name);
    }
}

void EqualizerView::removePreset() {
    if (presetsList -> currentText() != DEFAULT_PRESET) {
        presets.remove(presetsList -> currentText());
        presetsList -> removeItem(presetsList -> currentIndex());
    }
}

void EqualizerView::presetTypeChanged() {
    QWidget * scroll_panel = new QWidget(this);
    scroll_panel -> setStyleSheet("background-color: transparent;");
    QGridLayout * bottomLayout = new QGridLayout(scroll_panel);
    initBottomLayout(bottomLayout);
    scrollArea -> setWidget(scroll_panel);
    reset();
}

void EqualizerView::presetChanged(QString name) {
    presetChanging = true;
    QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
    QList<ClickableSlider *>::Iterator slider = sliders.begin();
    QMap<int, int> gains = presets.value(name);

    for(; slider != sliders.end(); slider++)
        (*slider) -> setValue(gains.value((*slider) -> property("num").toInt()));
    presetChanging = false;
}

void EqualizerView::eqValueChanged(int val) {
    QSlider * slider = (QSlider *)sender();
    int pos = slider -> property("num").toInt();
    PlayerFactory::obj().currPlayer() -> eqBand(pos, val / EQ_BASE);

    dbOutput[pos] -> setText(QString::number(val / EQ_BASE, 'g', 2) % QStringLiteral(" dB"));

    if (presetChanging) return;

    if (presetsList -> currentText() != DEFAULT_PRESET) {
        presetsList -> blockSignals(true);
        presets.insert(DEFAULT_PRESET, presets.value(presetsList -> currentText()));
        presetsList -> setCurrentText(DEFAULT_PRESET);
        presetsList -> blockSignals(false);
    }

    presets[presetsList -> currentText()].insert(pos, val);
}

void EqualizerView::reset() {
    if (presetsList -> currentText() == DEFAULT_PRESET) {
        QList<ClickableSlider *> sliders = findChildren<ClickableSlider *>();
        QList<ClickableSlider *>::Iterator slider = sliders.begin();

        for(; slider != sliders.end(); slider++)
            (*slider) -> setValue(0);
    } else {
        presets.insert(DEFAULT_PRESET, QMap<int, int>());
        presetsList -> setCurrentText(DEFAULT_PRESET);
    }
}
