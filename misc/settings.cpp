#include "settings.h"

Settings *Settings::self = 0;

Settings *Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

QBrush Settings::buildGradient(QRect rect, QColor color, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(1, QColor::fromRgba(qRgba(0, 0, 0, 192)));
    else if (useGradient)
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush Settings::defaultState(QRect rect, bool dark) {
    return buildGradient(rect, defaultColor1, dark);
}
QBrush Settings::listenedState(QRect rect, bool dark) {
    return buildGradient(rect, listenedColor1, dark);
}
QBrush Settings::likedState(QRect rect, bool dark) {
    return buildGradient(rect, likedColor1, dark);
}
QBrush Settings::playedState(QRect rect, bool dark) {
    return buildGradient(rect, playedColor1, dark);
}

QBrush Settings::unprocessedState(QRect rect, bool dark) {
//    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

//    if (dark) {
//        grad.setColorAt(0, folderColor1);
//        grad.setColorAt(0.8, Qt::black);
//    } else
//        grad.setColorAt(0, Qt::white);

//    return grad;

    return buildGradient(rect, folderColor1, dark);
}


bool Settings::isUseGradient() const {
    return useGradient;
}
void Settings::setUseGradient(bool use) {
    useGradient = use;
}

QColor Settings::getDefaultColor() const {
    return defaultColor1;
}
void Settings::setDefaultColor(QColor newColor) {
    defaultColor1 = newColor;
}

QColor Settings::getListenedColor() const {
    return listenedColor1;
}
void Settings::setListenedColor(QColor newColor) {
    listenedColor1 = newColor;
}

QColor Settings::getLikedColor() const {
    return likedColor1;
}
void Settings::setLikedColor(QColor newColor) {
    likedColor1 = newColor;
}

QColor Settings::getPlayedColor() const {
    return playedColor1;
}
void Settings::setPlayedColor(QColor newColor) {
    playedColor1 = newColor;
}

QColor Settings::getFolderColor() const {
    return folderColor1;
}
void Settings::setFolderColor(QColor newColor) {
    folderColor1 = newColor;
}

bool Settings::isShowSystemIcons() const {
    return showSystemIcons;
}
void Settings::setShowSystemIcons(bool show) {
    showSystemIcons = show;
}


bool Settings::isShowInfo() const {
    return showInfo;
}
void Settings::setShowInfo(bool show) {
    showInfo = show;
}

bool Settings::isSpoilOnActivation() const {
    return spoilOnActivation;
}
void Settings::setSpoilOnActivation(bool show) {
    spoilOnActivation = show;
}

bool Settings::isMetricShow() const {
    return showMetric;
}
void Settings::setMetricShow(bool show) {
    showMetric = show;
}

bool Settings::isCheckboxShow() const {
    return showCheckbox;
}
void Settings::setCheckboxShow(bool show) {
    showCheckbox = show;
}

QString Settings::getDownloadPath() const {
    return downloadPath;
}

void Settings::setDownloadPath(QString newPath) {
    downloadPath = newPath;
}






QString Settings::getItemFontName() {
    return itemFontName;
}

void Settings::setItemFontName(QString newFontName) {
    itemFontName = newFontName;
}

int Settings::getItemFontSize() {
    return itemFontSize;
}

void Settings::setItemFontSize(int newFontSize) {
    itemFontSize = newFontSize;
}

QColor Settings::getItemTextColor() {
    return itemTextColor;
}

void Settings::setItemTextColor(QColor newColor) {
    itemTextColor = newColor;
}

QColor Settings::getSelectedItemTextColor() {
    return selectedItemTextColor;
}

void Settings::setSelectedItemTextColor(QColor newColor) {
    selectedItemTextColor = newColor;
}

QString Settings::getItemInfoFontName() {
    return itemInfoFontName;
}

void Settings::setItemInfoFontName(QString newFontName) {
    itemInfoFontName = newFontName;
}

int Settings::getItemInfoFontSize() {
    return itemInfoFontSize;
}

void Settings::setItemInfoFontSize(int newFontSize) {
    itemInfoFontSize = newFontSize;
}

QColor Settings::getItemInfoTextColor() {
    return itemInfoColor;
}

void Settings::setItemInfoTextColor(QColor newColor) {
    itemInfoColor = newColor;
}

QColor Settings::getSelectedItemInfoTextColor() {
    return selectedItemInfoColor;
}

void Settings::setSelectedItemInfoTextColor(QColor newColor) {
    selectedItemInfoColor = newColor;
}

int Settings::getTabPosition() {
    return tabPosition;
}
void Settings::setTabPosition(int newPositionsType) {
    tabPosition = newPositionsType;
}

int Settings::getItemHeight() {
    return itemHeight;
}
void Settings::setItemHeight(int newHeight) {
    itemHeight = newHeight;
}

bool Settings::getScrollButtonUsage() {
    return useScrollButtons;
}
void Settings::setScrollButtonUsage(bool use) {
    useScrollButtons = use;
}


bool Settings::isCustomColorSpectrum() {
    return customcolorSpectrum;
}
void Settings::setCustomColorSpectrum(bool use) {
    customcolorSpectrum = use;
}

QColor Settings::getSpectrumColor() {
    return spectrumColor;
}
void Settings::setSpectrumColor(QColor newColor) {
    spectrumColor = newColor;
}

QColor Settings::getSpectrumColor2() {
    return spectrumColor2;
}
void Settings::setSpectrumColor2(QColor newColor) {
    spectrumColor2 = newColor;
}

QColor Settings::getSpectrumColor3() {
    return spectrumColor3;
}
void Settings::setSpectrumColor3(QColor newColor) {
    spectrumColor3 = newColor;
}

int Settings::getSpectrumFreqRate() {
    return spectrumFreqRate;
}
void Settings::setSpectrumFreqRate(int newRate) {
    spectrumFreqRate = newRate;
}

int Settings::getSpectrumBarsCount() {
    return spectrumBarsCount;
}
void Settings::setSpectrumBarsCount(int newCount) {
    spectrumBarsCount = newCount;
}

int Settings::getSpectrumHeight() {
    return spectrumHeight;
}

void Settings::setSpectrumHeight(int newHeight) {
    spectrumHeight = newHeight;
}

bool Settings::getSpectrumCombo() {
    return comboSpectrum;
}
void Settings::setSpectrumCombo(bool newState) {
    comboSpectrum = newState;
}

int Settings::getSpectrumMultiplier() {
    return spectrumMultiplier;
}
void Settings::setSpectrumMultiplier(int newMultiplier) {
    spectrumMultiplier = newMultiplier;
}

int Settings::getTotalItemHeight() {
    return getItemHeight() + (isShowInfo() ? getItemInfoFontSize() * 2 : 0);
}

int Settings::getIconHeight() {
    return getTotalItemHeight() - 1;
}

QList<HotkeyModelItem *> * Settings::getHotKeys() const {
    QVector<QVariant> itemVals;
    QList<HotkeyModelItem *> * ret = new QList<HotkeyModelItem *>();
    int iKey;

    foreach(QString key, hotkeys.keys()) {
        iKey = key.toInt();
        itemVals = QVector<QVariant>();
        itemVals.append(humanizeHotkeyText.value(iKey));
        itemVals.append(hotkeys.value(key));
        itemVals.append(iKey);
        ret -> append(new HotkeyModelItem(itemVals));
    }

    return ret;
}

QJsonObject Settings::hotkeysJson() const {
    return hotkeys;
}

void Settings::setHotKeys(QList<HotkeyModelItem *> hotkeyList) {
    foreach(HotkeyModelItem * item, hotkeyList) {
        hotkeys.insert(QString::number(item -> data(2).toInt()), QJsonValue::fromVariant(item -> data(1)));
    }
}

void Settings::setHotKeys(QJsonObject hotkeysHash) {
    if (hotkeysHash.empty()) {
        hotkeys = QJsonObject();
        hotkeys.insert(QString::number(HOTKEY_NEXT), QJsonValue::fromVariant("Ctrl+Down"));
        hotkeys.insert(QString::number(HOTKEY_NEXT_AND_DELETE), QJsonValue::fromVariant("Ctrl+Delete"));
        hotkeys.insert(QString::number(HOTKEY_PREV), QJsonValue::fromVariant("Ctrl+Up"));
        hotkeys.insert(QString::number(HOTKEY_PLAY), QJsonValue::fromVariant("Media Play"));
        hotkeys.insert(QString::number(HOTKEY_STOP), QJsonValue::fromVariant("Media Stop"));
        hotkeys.insert(QString::number(HOTKEY_SETTINGS), QJsonValue::fromVariant("Ctrl+Q"));

        hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+Alt+Right"));
        hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+Alt+Left"));
        hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+Shift+Right"));
        hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+Shift+Left"));
    } else {
        hotkeys = hotkeysHash;
    }
}

void Settings::fromJson(QJsonObject settingsObj) {
    setHotKeys(settingsObj.value("hotkeys").toObject());
    downloadPath = settingsObj.value("download_path").toString(QCoreApplication::applicationDirPath() + "/downloads/");
    showCheckbox = settingsObj.value("show_checkboxes").toBool(true);
    showMetric = settingsObj.value("show_metric").toBool(true);
    spoilOnActivation = settingsObj.value("spoil_on_activation").toBool(true);
    showInfo = settingsObj.value("show_info").toBool(true);
    showSystemIcons = settingsObj.value("show_system_icons").toBool(false);
    useGradient = settingsObj.value("use_gradient").toBool(true);

    QVariant defaultColor1Var = settingsObj.value("default_color1").toVariant();
    defaultColor1 = defaultColor1Var.isValid() ? defaultColor1Var.value<QColor>() : QColor(98, 173, 248);

    QVariant listenedColor1Var = settingsObj.value("listened_color1").toVariant();
    listenedColor1 = listenedColor1Var.isValid() ? listenedColor1Var.value<QColor>() : QColor(240, 128, 128);

    QVariant likedColor1Var = settingsObj.value("liked_color1").toVariant();
    likedColor1 = likedColor1Var.isValid() ? likedColor1Var.value<QColor>() : QColor(232, 196, 0);

    QVariant playedColor1Var = settingsObj.value("played_color1").toVariant();
    playedColor1 = playedColor1Var.isValid() ? playedColor1Var.value<QColor>() : QColor(144, 238, 144);

    QVariant folderColor1Var = settingsObj.value("folder_color1").toVariant();
    folderColor1 = folderColor1Var.isValid() ? folderColor1Var.value<QColor>() : QColor(220, 220, 220);

    itemFontName = settingsObj.value("item_font_name").toString("Arial");
    itemFontSize = settingsObj.value("item_font_size").toInt(9);

    QVariant itemTextColorVar = settingsObj.value("item_text_color").toVariant();
    itemTextColor = itemTextColorVar.isValid() ? itemTextColorVar.value<QColor>() : QColor(0, 0, 0);

    QVariant selectedItemTextColorVar = settingsObj.value("selected_item_text_color").toVariant();
    selectedItemTextColor = selectedItemTextColorVar.isValid() ? selectedItemTextColorVar.value<QColor>() : QColor(255, 255, 255);

    itemInfoFontName = settingsObj.value("item_info_font_name").toString("Arial");
    itemInfoFontSize = settingsObj.value("item_info_font_size").toInt(7);

    itemTextColorVar = settingsObj.value("item_info_text_color").toVariant();
    itemInfoColor = itemTextColorVar.isValid() ? itemTextColorVar.value<QColor>() : QColor(0, 0, 0);

    selectedItemTextColorVar = settingsObj.value("selected_item_info_text_color").toVariant();
    selectedItemInfoColor = selectedItemTextColorVar.isValid() ? selectedItemTextColorVar.value<QColor>() : QColor(255, 255, 255);

    tabPosition = settingsObj.value("tab_position").toInt(0);

    itemHeight = settingsObj.value("item_height").toInt(18);

    useScrollButtons = settingsObj.value("use_scroll_buttons").toBool(true);

    customcolorSpectrum = settingsObj.value("customcolor_spectrum").toBool(false);

    QVariant specColorVar = settingsObj.value("spectrum_color").toVariant();
    spectrumColor = specColorVar.isValid() ? specColorVar.value<QColor>() : QColor(0, 0, 0);

    QVariant specColorVar2 = settingsObj.value("spectrum_color2").toVariant();
    spectrumColor2 = specColorVar2.isValid() ? specColorVar2.value<QColor>() : QColor(128, 128, 128);

    QVariant specColorVar3 = settingsObj.value("spectrum_color3").toVariant();
    spectrumColor3 = specColorVar3.isValid() ? specColorVar3.value<QColor>() : QColor(255, 255, 255);


    spectrumFreqRate = settingsObj.value("spectrum_rate").toInt(20);
    spectrumBarsCount = settingsObj.value("spectrum_bars_count").toInt(30);

    spectrumHeight = settingsObj.value("spectrum_height").toInt(60);
    comboSpectrum = settingsObj.value("spectrum_combo").toBool(false);

    spectrumMultiplier = settingsObj.value("spectrum_multiplier").toInt(3);
}

QJsonObject Settings::toJson() {
    QJsonObject ret = QJsonObject();

    ret.insert("hotkeys", hotkeysJson());
    ret.insert("download_path", QJsonValue::fromVariant(downloadPath));
    ret.insert("show_checkboxes", QJsonValue::fromVariant(showCheckbox));
    ret.insert("show_metric", QJsonValue::fromVariant(showMetric));
    ret.insert("spoil_on_activation", QJsonValue::fromVariant(spoilOnActivation));
    ret.insert("show_info", QJsonValue::fromVariant(showInfo));
    ret.insert("show_system_icons", QJsonValue::fromVariant(showSystemIcons));

    ret.insert("default_color1", QJsonValue::fromVariant(defaultColor1));
    ret.insert("listened_color1", QJsonValue::fromVariant(listenedColor1));
    ret.insert("liked_color1", QJsonValue::fromVariant(likedColor1));
    ret.insert("played_color1", QJsonValue::fromVariant(playedColor1));
    ret.insert("folder_color1", QJsonValue::fromVariant(folderColor1));


    ret.insert("item_font_name", QJsonValue::fromVariant(itemFontName));
    ret.insert("item_font_size", QJsonValue::fromVariant(itemFontSize));
    ret.insert("item_text_color", QJsonValue::fromVariant(itemTextColor));
    ret.insert("selected_item_text_color", QJsonValue::fromVariant(selectedItemTextColor));

    ret.insert("item_info_font_name", QJsonValue::fromVariant(itemInfoFontName));
    ret.insert("item_info_font_size", QJsonValue::fromVariant(itemInfoFontSize));
    ret.insert("item_info_text_color", QJsonValue::fromVariant(itemInfoColor));
    ret.insert("selected_item_info_text_color", QJsonValue::fromVariant(selectedItemInfoColor));

    ret.insert("tab_position", QJsonValue::fromVariant(tabPosition));
    ret.insert("item_height", QJsonValue::fromVariant(itemHeight));

    ret.insert("use_scroll_buttons", QJsonValue::fromVariant(useScrollButtons));

    ret.insert("customcolor_spectrum", QJsonValue::fromVariant(customcolorSpectrum));
    ret.insert("spectrum_color", QJsonValue::fromVariant(spectrumColor));
    ret.insert("spectrum_color2", QJsonValue::fromVariant(spectrumColor2));
    ret.insert("spectrum_color3", QJsonValue::fromVariant(spectrumColor3));

    ret.insert("spectrum_rate", QJsonValue::fromVariant(spectrumFreqRate));
    ret.insert("spectrum_bars_count", QJsonValue::fromVariant(spectrumBarsCount));
    ret.insert("spectrum_height", QJsonValue::fromVariant(spectrumHeight));
    ret.insert("spectrum_combo", QJsonValue::fromVariant(comboSpectrum));
    ret.insert("spectrum_multiplier", QJsonValue::fromVariant(spectrumMultiplier));

    return ret;
}

QFont Settings::getItemFont() {
    return QFont(getItemFontName(), getItemFontSize(), QFont::Normal);
}

QFont Settings::getItemInfoFont() {
    return QFont(getItemInfoFontName(), getItemInfoFontSize(), QFont::Bold);
}
