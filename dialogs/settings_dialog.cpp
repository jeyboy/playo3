#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include <QFontDatabase>
#include "media/player.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog), iconSizeChanged(false) {
  ui -> setupUi(this);

  setWindowTitle("Playo settings");
  setFixedWidth(359);
  setFixedHeight(312);
  setSizeGripEnabled(false);

  initGlobalSettings();
  initItemsSettings();
  initViewSettings();
  initHotkeysSettings();
  initSpectrumSettings();
  initLibrarySettings();
  initExtensions();
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::instance() -> hotKeys();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++) {
        switch((*it) -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(nextExecTriggering()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(nextExecWithDelTriggering())); /*onNextItemRequiring*/
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(prevExecTriggering()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(stop()));
            break;}
            case HOTKEY_OPEN_SETTINGS: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(editActiveBar()));
            break;}


            case HOTKEY_POS_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(slidePosForward()));
            break;}

            case HOTKEY_POS_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(slidePosBackward()));
            break;}

            case HOTKEY_VOL_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(slideVolForward()));
            break;}

            case HOTKEY_VOL_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), Player::instance(), SLOT(slideVolBackward()));
            break;}
        }
    }
}

void SettingsDialog::on_cancelButton_clicked() {
    reject();
}

void SettingsDialog::on_resetButton_clicked() {
    switch(ui -> settingsTabs -> currentIndex()) {
        case 0: {
            Settings::instance() -> resetGlobalSettings();
            initGlobalSettings();
        break;}

        case 1: {
            Settings::instance() -> resetTabSettings();
            initViewSettings();
        break;}

        case 2: {
            Settings::instance() -> resetHotkeySettings();
            initHotkeysSettings();
        break;}

        case 3: {
            Settings::instance() -> resetItemSettings();
            initItemsSettings();
        break;}

        case 4: {
            Settings::instance() -> resetSpectrumSettings();
            initSpectrumSettings();
        break;}

        case 5: {
            Settings::instance() -> resetLibrarySettings();
            initLibrarySettings();
        break;}
    }
}

void SettingsDialog::on_acceptButton_clicked() {
    saveGlobalSettings();
    saveItemsSettings();
    saveViewSettings();
    saveHotkeysSettings();
    saveSpectrumSettings();
    saveLibrarySettings();

    accept();
}

void SettingsDialog::on_browseButton_clicked() {
    QString path = QFileDialog::getExistingDirectory(this, "Please choose new default download path");
    if (!path.isEmpty()) {
        if (!path.endsWith('/'))
            path += "/";
        ui -> downloadPath -> setText(path);
    }
}

bool SettingsDialog::isIconSizeChanged() const {
    return iconSizeChanged;
}

void SettingsDialog::on_defaultColorButton_clicked() {
    if (execColorDialog(defaultColor))
        ui -> defaultColorButton -> setStyleSheet("background-color: " + defaultColor.name() + ";");
}

void SettingsDialog::on_listenedColorButton_clicked() {
    if (execColorDialog(listenedColor))
        ui -> listenedColorButton -> setStyleSheet("background-color: " + listenedColor.name() + ";");
}

void SettingsDialog::on_likedColorButton_clicked() {
    if (execColorDialog(likedColor))
        ui -> likedColorButton -> setStyleSheet("background-color: " + likedColor.name() + ";");
}

void SettingsDialog::on_playedColorButton_clicked() {
    if (execColorDialog(playedColor))
        ui -> playedColorButton -> setStyleSheet("background-color: " + playedColor.name() + ";");
}

void SettingsDialog::on_folderColorButton_clicked() {
    if (execColorDialog(folderColor))
        ui -> folderColorButton -> setStyleSheet("background-color: " + folderColor.name() + ";");
}

void SettingsDialog::on_defaultItemTextColorButton_clicked() {
    if (execColorDialog(itemTextColor))
        ui -> defaultItemTextColorButton -> setStyleSheet("background-color: " + itemTextColor.name() + ";");
}

void SettingsDialog::on_selectedItemTextColorButton_clicked() {
    if (execColorDialog(selectedItemTextColor))
        ui -> selectedItemTextColorButton -> setStyleSheet("background-color: " + selectedItemTextColor.name() + ";");
}

void SettingsDialog::on_defaultItemInfoTextColorButton_clicked() {
    if (execColorDialog(itemInfoTextColor))
        ui -> defaultItemInfoTextColorButton -> setStyleSheet("background-color: " + itemInfoTextColor.name() + ";");
}

void SettingsDialog::on_selectedItemInfoTextColorButton_clicked() {
    if (execColorDialog(selectedItemInfoTextColor))
        ui -> selectedItemInfoTextColorButton -> setStyleSheet("background-color: " + selectedItemInfoTextColor.name() + ";");
}

void SettingsDialog::on_spectrumColor_clicked() {
    if (execColorDialog(spectrumColor))
        ui -> spectrumColor -> setStyleSheet("background-color: " + spectrumColor.name() + ";");
}

void SettingsDialog::on_spectrumColor2_clicked() {
    if (execColorDialog(spectrumColor2))
        ui -> spectrumColor2 -> setStyleSheet("background-color: " + spectrumColor2.name() + ";");
}

void SettingsDialog::on_spectrumColor3_clicked() {
    if (execColorDialog(spectrumColor3))
        ui -> spectrumColor3 -> setStyleSheet("background-color: " + spectrumColor3.name() + ";");
}


void SettingsDialog::initGlobalSettings() {
    ui -> drawMetrics -> setChecked(Settings::instance() -> isMetricShow());
    ui -> drawMetricsNumero -> setChecked(Settings::instance() -> isMetricNumero());

    ui -> downloadPath -> setText(Settings::instance() -> defaultDownloadPath());

    if (!QDir().mkdir(Settings::instance() -> defaultDownloadPath()))
        ui -> downloadPath -> setStyleSheet("background-color: red; color: white;");


    QStringList positions;
    positions.append("Above");
    positions.append("Below");
    positions.append("Left");
    positions.append("Right");

    ui -> tabPositionSelect -> insertItems(0, positions);
    ui -> tabPositionSelect -> setCurrentIndex(Settings::instance() -> tabPosition());


    ui -> saveCommonTab -> setChecked(Settings::instance() -> isSaveCommonTab());
    ui -> openDropPointInTab -> setChecked(Settings::instance() -> isOpenDropPointInTab());
    if (!Settings::instance() -> isOpenDropPointInTab())
        on_openDropPointInTab_toggled(false);

    QStringList tab_types;
    tab_types.append("List");
    tab_types.append("Level Tree");
    tab_types.append("Tree");

    ui -> dropPointTabTypeSelect -> insertItems(0, tab_types);
    ui -> dropPointTabTypeSelect -> setCurrentIndex(Settings::instance() -> openDropPointInTabType() - 1);
}

void SettingsDialog::initItemsSettings() {
    ui -> useGradientCheck -> setChecked(Settings::instance() -> isUseGradient());

    ui -> itemFontSize -> setValue(Settings::instance() -> itemFontSize());
    ui -> itemInfoFontSize -> setValue(Settings::instance() -> itemInfoFontSize());

    QFontDatabase fontDatabase;

    ui -> itemFontSelect -> insertItems(0, fontDatabase.families());
    ui -> itemFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::instance() -> itemFontName())));
    ui -> itemInfoFontSelect -> insertItems(0, fontDatabase.families());
    ui -> itemInfoFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::instance() -> itemInfoFontName())));

    defaultColor = Settings::instance() -> defaultItemColor();
    ui -> defaultColorButton -> setStyleSheet("background-color: " + defaultColor.name() + ";");

    listenedColor = Settings::instance() -> listenedItemColor();
    ui -> listenedColorButton -> setStyleSheet("background-color: " + listenedColor.name() + ";");

    likedColor = Settings::instance() -> likedItemColor();
    ui -> likedColorButton -> setStyleSheet("background-color: " + likedColor.name() + ";");

    playedColor = Settings::instance() -> playedItemColor();
    ui -> playedColorButton -> setStyleSheet("background-color: " + playedColor.name() + ";");

    folderColor = Settings::instance() -> folderItemColor();
    ui -> folderColorButton -> setStyleSheet("background-color: " + folderColor.name() + ";");


    itemTextColor = Settings::instance() -> itemTextColor();
    ui -> defaultItemTextColorButton -> setStyleSheet("background-color: " + itemTextColor.name() + ";");

    selectedItemTextColor = Settings::instance() -> selectedItemTextColor();
    ui -> selectedItemTextColorButton -> setStyleSheet("background-color: " + selectedItemTextColor.name() + ";");

    itemInfoTextColor = Settings::instance() -> itemInfoTextColor();
    ui -> defaultItemInfoTextColorButton -> setStyleSheet("background-color: " + itemInfoTextColor.name() + ";");

    selectedItemInfoTextColor = Settings::instance() -> selectedItemInfoTextColor();
    ui -> selectedItemInfoTextColorButton -> setStyleSheet("background-color: " + selectedItemInfoTextColor.name() + ";");

    ui -> itemHeightSize -> setValue(Settings::instance() -> itemHeight());
}
void SettingsDialog::initViewSettings() {
    ui -> showCheckboxes -> setChecked(Settings::instance() -> isCheckboxShow());

    ui -> alertOnDeletion -> setChecked(Settings::instance() -> isAlertOnFolderDeletion());
    ui -> spoilOnActivate -> setChecked(Settings::instance() -> isSpoilOnActivation());

    ui -> useSystemIconsCheck -> setChecked(Settings::instance() -> isShowSystemIcons());
    ui -> indentationStep -> setValue(Settings::instance() -> treeIndentation());
}
void SettingsDialog::initHotkeysSettings() {
    ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
    ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
    ui -> treeView -> setModel(new HotkeyModel(Settings::instance() -> hotKeys(), this));
    ui -> treeView -> hideColumn(2);
    ui -> treeView -> setColumnWidth(0, 230);
}
void SettingsDialog::initSpectrumSettings() {
    ui -> autoBarsAmount -> setChecked(Settings::instance() -> isAutoBarsAmount());

    spectrumColor = Settings::instance() -> spectrumColor();
    ui -> spectrumColor -> setStyleSheet("background-color: " + spectrumColor.name() + ";");

    spectrumColor2 = Settings::instance() -> spectrumColor2();
    ui -> spectrumColor2 -> setStyleSheet("background-color: " + spectrumColor2.name() + ";");

    spectrumColor3 = Settings::instance() -> spectrumColor3();
    ui -> spectrumColor3 -> setStyleSheet("background-color: " + spectrumColor3.name() + ";");

    ui -> spectrumCustomColorUse -> setChecked(Settings::instance() -> isCustomColorSpectrum());

    ui -> spectrumBarsCount -> setValue(Settings::instance() -> spectrumBarsCount());
    ui -> spectrumUpdateFrequecy -> setValue(Settings::instance() -> spectrumFreqRate());

    ui -> spectrumHeight -> setValue(Settings::instance() -> spectrumHeight());

    ui -> spectrumMultiplier -> setValue(Settings::instance() -> spectrumMultiplier());

    QStringList spectrumTypes;
    spectrumTypes.append("Combined");
    spectrumTypes.append("By Channels");
    spectrumTypes.append("Curved");

    ui -> spectrumTypeSelect -> insertItems(0, spectrumTypes);
    ui -> spectrumTypeSelect -> setCurrentIndex((int)Settings::instance() -> spectrumType());
}

void SettingsDialog::initLibrarySettings() {
    ui -> libSaveFreq -> setValue(Settings::instance() -> saveLibDelay() / 1000.0);
    ui -> remoteItemProcDelay -> setValue(Settings::instance() -> remoteItemsProcDelay() / 1000.0);
    ui -> interactiveStats -> setChecked(Settings::instance() -> isInteractiveProc());
    ui -> showInfo -> setChecked(Settings::instance() -> isShowInfo());
}

void SettingsDialog::initExtensions() {
    extDialog = new Playo3::ExtensionDialog(this);
    extDialog -> setWindowFlags(Qt::Widget);
    ui -> settingsTabs -> addTab(extDialog, "Extension filters");
}

void SettingsDialog::saveGlobalSettings() {
    Settings::instance() -> setDefaultDownloadPath(ui -> downloadPath -> text());
    Settings::instance() -> setMetricShow(ui -> drawMetrics -> isChecked());
    Settings::instance() -> setMetricNumeroShow(ui -> drawMetricsNumero -> isChecked());
    Settings::instance() -> setTabPosition(ui -> tabPositionSelect -> currentIndex());

    Settings::instance() -> setSaveCommonTab(ui -> saveCommonTab -> isChecked());
    Settings::instance() -> setOpenDropPointInTab(ui -> openDropPointInTab -> isChecked());
    Settings::instance() -> setOpenDropPointInTabType((Playo3::ContainerType)(ui -> dropPointTabTypeSelect -> currentIndex() + 1));
}

void SettingsDialog::saveItemsSettings() {
    Settings::instance() -> setItemHeight(ui -> itemHeightSize -> value());

    Settings::instance() -> setUseGradient(ui -> useGradientCheck -> isChecked());

    Settings::instance() -> setDefaultItemColor(defaultColor);
    Settings::instance() -> setListenedItemColor(listenedColor);
    Settings::instance() -> setLikedItemColor(likedColor);
    Settings::instance() -> setPlayedItemColor(playedColor);
    Settings::instance() -> setFolderItemColor(folderColor);

    Settings::instance() -> setItemFontName(ui -> itemFontSelect -> currentText());
    Settings::instance() -> setItemFontSize(ui -> itemFontSize -> value());

    Settings::instance() -> setItemInfoFontName(ui -> itemInfoFontSelect -> currentText());
    Settings::instance() -> setItemInfoFontSize(ui -> itemInfoFontSize -> value());

    Settings::instance() -> setItemTextColor(itemTextColor);
    Settings::instance() -> setItemInfoTextColor(itemInfoTextColor);

    Settings::instance() -> setSelectedItemTextColor(selectedItemTextColor);
    Settings::instance() -> setSelectedItemInfoTextColor(selectedItemInfoTextColor);
}

void SettingsDialog::saveViewSettings() {
    Settings::instance() -> setAlertOnFolderDeletion(ui -> alertOnDeletion -> isChecked());
    Settings::instance() -> setSpoilOnActivation(ui -> spoilOnActivate -> isChecked());
    Settings::instance() -> setCheckboxShow(ui -> showCheckboxes -> isChecked());

    iconSizeChanged |= Settings::instance() -> itemHeight() != ui -> itemHeightSize -> value();
    Settings::instance() -> setShowInfo(ui -> showInfo -> isChecked());

    Settings::instance() -> setShowSystemIcons(ui -> useSystemIconsCheck -> isChecked());

    Settings::instance() -> setTreeIndentation(ui -> indentationStep -> value());
}

void SettingsDialog::saveHotkeysSettings() {
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> treeView -> model());
    QList<HotkeyModelItem *> list = model -> toplevelItems();
    Settings::instance() -> setHotKeys(list);

    HotkeyManager::instance() -> clear();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++)
        HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString());
}

void SettingsDialog::saveSpectrumSettings() {
    Settings::instance() -> setAutoBarsAmount(ui -> autoBarsAmount -> isChecked());

    Settings::instance() -> setSpectrumColor(spectrumColor);
    Settings::instance() -> setSpectrumColor2(spectrumColor2);
    Settings::instance() -> setSpectrumColor3(spectrumColor3);

    Settings::instance() -> setCustomColorSpectrum(ui -> spectrumCustomColorUse -> isChecked());

    Settings::instance() -> setSpectrumBarsCount(ui -> spectrumBarsCount -> value());
    Settings::instance() -> setSpectrumFreqRate(ui -> spectrumUpdateFrequecy -> value());

    Settings::instance() -> setSpectrumHeight(ui -> spectrumHeight -> value());
    Settings::instance() -> setSpectrumMultiplier(ui -> spectrumMultiplier -> value());
    Settings::instance() -> setSpectrumType((Playo3::SpectrumType)ui -> spectrumTypeSelect -> currentIndex());
}

void SettingsDialog::saveLibrarySettings() {
    Settings::instance() -> setSaveLibDelay(ui -> libSaveFreq -> value() * 1000);
    Settings::instance() -> setRemoteItemsProcDelay(ui -> remoteItemProcDelay -> value() * 1000);
    Settings::instance() -> setInteractiveProc(ui -> interactiveStats -> isChecked());

    iconSizeChanged |= Settings::instance() -> isShowInfo() != ui -> showInfo -> isChecked();
    Settings::instance() -> setShowInfo(ui -> showInfo -> isChecked());
}

bool SettingsDialog::execColorDialog(QColor & color) {
    QColorDialog col(this);
    col.setCurrentColor(color);
    if (col.exec() == QColorDialog::Accepted) {
        color = col.selectedColor();
        return true;
    }

    return false;
}

void SettingsDialog::on_openDropPointInTab_toggled(bool checked) {
    ui -> label_dropPointTabType -> setEnabled(checked);
    ui -> dropPointTabTypeSelect -> setEnabled(checked);
}

void SettingsDialog::on_drawMetrics_clicked(bool checked) {
    ui -> drawMetricsNumero -> setEnabled(checked);
}
