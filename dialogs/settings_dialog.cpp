#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include <QFontDatabase>

using namespace Dialogs;

SettingsDialog::SettingsDialog(QWidget * parent) :
  QDialog(parent), ui(new Ui::SettingsDialog), iconSizeChanged(false) {
  ui -> setupUi(this);

  setWindowTitle(QStringLiteral("Playo settings"));
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
    QList<HotkeyModelItem *> list = *Settings::obj().hotKeys();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++) {
        switch((*it) -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playNext()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playNextWithDel()));
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playPrev()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(stop()));
            break;}
            case HOTKEY_OPEN_SETTINGS: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(editActiveBar()));
            break;}

            case HOTKEY_POS_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slidePosForward()));
            break;}

            case HOTKEY_POS_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slidePosBackward()));
            break;}

            case HOTKEY_VOL_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slideVolForward()));
            break;}

            case HOTKEY_VOL_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slideVolBackward()));
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
            Settings::obj().resetGlobalSettings();
            initGlobalSettings();
        break;}

        case 1: {
            Settings::obj().resetTabSettings();
            initViewSettings();
        break;}

        case 2: {
            Settings::obj().resetHotkeySettings();
            initHotkeysSettings();
        break;}

        case 3: {
            Settings::obj().resetItemSettings();
            initItemsSettings();
        break;}

        case 4: {
            Settings::obj().resetSpectrumSettings();
            initSpectrumSettings();
        break;}

        case 5: {
            Settings::obj().resetLibrarySettings();
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
    QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("Please choose new default download path"));
    if (!path.isEmpty()) {
        if (!path.endsWith('/'))
            path += QStringLiteral("/");
        ui -> downloadPath -> setText(path);
    }
}

bool SettingsDialog::isIconSizeChanged() const {
    return iconSizeChanged;
}

void SettingsDialog::on_defaultColorButton_clicked() {
    if (execColorDialog(defaultColor))
        ui -> defaultColorButton -> setStyleSheet(QStringLiteral("background-color: ") % defaultColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_listenedColorButton_clicked() {
    if (execColorDialog(listenedColor))
        ui -> listenedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % listenedColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_likedColorButton_clicked() {
    if (execColorDialog(likedColor))
        ui -> likedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % likedColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_playedColorButton_clicked() {
    if (execColorDialog(playedColor))
        ui -> playedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % playedColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_folderColorButton_clicked() {
    if (execColorDialog(folderColor))
        ui -> folderColorButton -> setStyleSheet(QStringLiteral("background-color: ") % folderColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_itemColorButton_clicked() {
    if (execColorDialog(itemColor))
        ui -> itemColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_defaultItemTextColorButton_clicked() {
    if (execColorDialog(itemTextColor))
        ui -> defaultItemTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemTextColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_selectedItemTextColorButton_clicked() {
    if (execColorDialog(selectedItemTextColor))
        ui -> selectedItemTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % selectedItemTextColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_defaultItemInfoTextColorButton_clicked() {
    if (execColorDialog(itemInfoTextColor))
        ui -> defaultItemInfoTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemInfoTextColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_selectedItemInfoTextColorButton_clicked() {
    if (execColorDialog(selectedItemInfoTextColor))
        ui -> selectedItemInfoTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % selectedItemInfoTextColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_spectrumColor_clicked() {
    if (execColorDialog(spectrumColor))
        ui -> spectrumColor -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor.name() % QStringLiteral(";"));
}

void SettingsDialog::on_spectrumColor2_clicked() {
    if (execColorDialog(spectrumColor2))
        ui -> spectrumColor2 -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor2.name() % QStringLiteral(";"));
}

void SettingsDialog::on_spectrumColor3_clicked() {
    if (execColorDialog(spectrumColor3))
        ui -> spectrumColor3 -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor3.name() % QStringLiteral(";"));
}


void SettingsDialog::initGlobalSettings() {
    ui -> autorunned -> blockSignals(true);
    ui -> autorunned -> setChecked(Settings::obj().isAutorunned());
    ui -> autorunned -> blockSignals(false);

    ui -> drawMetrics -> setChecked(Settings::obj().isMetricShow());
    ui -> drawMetricsNumero -> setChecked(Settings::obj().isMetricNumero());

    ui -> downloadPath -> setText(Settings::obj().defaultDownloadPath());

    if (!QDir().mkdir(Settings::obj().defaultDownloadPath()))
        ui -> downloadPath -> setStyleSheet(QStringLiteral("background-color: red; color: white;"));


    QStringList positions;
    positions.append(QStringLiteral("Above"));
    positions.append(QStringLiteral("Below"));
    positions.append(QStringLiteral("Left"));
    positions.append(QStringLiteral("Right"));

    ui -> tabPositionSelect -> insertItems(0, positions);
    ui -> tabPositionSelect -> setCurrentIndex(Settings::obj().tabPosition());


    ui -> saveCommonTab -> setChecked(Settings::obj().isSaveCommonTab());
    ui -> openDropPointInTab -> setChecked(Settings::obj().isOpenDropPointInTab());
    if (!Settings::obj().isOpenDropPointInTab())
        on_openDropPointInTab_toggled(false);

    QStringList tab_types;
    tab_types.append(QStringLiteral("List"));
    tab_types.append(QStringLiteral("Level Tree"));
    tab_types.append(QStringLiteral("Tree"));

    ui -> dropPointTabTypeSelect -> insertItems(0, tab_types);
    ui -> dropPointTabTypeSelect -> setCurrentIndex(Settings::obj().openDropPointInTabType() - 1);

    ui -> openTimeOut -> setValue(Settings::obj().openTimeOut());
}

void SettingsDialog::initItemsSettings() {
    ui -> useGradientCheck -> setChecked(Settings::obj().isUseGradient());

    ui -> itemFontSize -> setValue(Settings::obj().itemFontSize());
    ui -> itemInfoFontSize -> setValue(Settings::obj().itemInfoFontSize());

    QFontDatabase fontDatabase;

    ui -> itemFontSelect -> insertItems(0, fontDatabase.families());
    ui -> itemFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::obj().itemFontName())));
    ui -> itemInfoFontSelect -> insertItems(0, fontDatabase.families());
    ui -> itemInfoFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::obj().itemInfoFontName())));

    defaultColor = Settings::obj().defaultItemColor();
    ui -> defaultColorButton -> setStyleSheet(QStringLiteral("background-color: ") % defaultColor.name() % QStringLiteral(";"));

    listenedColor = Settings::obj().listenedItemColor();
    ui -> listenedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % listenedColor.name() % QStringLiteral(";"));

    likedColor = Settings::obj().likedItemColor();
    ui -> likedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % likedColor.name() % QStringLiteral(";"));

    playedColor = Settings::obj().playedItemColor();
    ui -> playedColorButton -> setStyleSheet(QStringLiteral("background-color: ") % playedColor.name() % QStringLiteral(";"));

    folderColor = Settings::obj().folderItemColor();
    ui -> folderColorButton -> setStyleSheet(QStringLiteral("background-color: ") % folderColor.name() % QStringLiteral(";"));

    itemColor = Settings::obj().itemColor();
    ui -> itemColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemColor.name() % QStringLiteral(";"));


    itemTextColor = Settings::obj().itemTextColor();
    ui -> defaultItemTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemTextColor.name() % QStringLiteral(";"));

    selectedItemTextColor = Settings::obj().selectedItemTextColor();
    ui -> selectedItemTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % selectedItemTextColor.name() % QStringLiteral(";"));

    itemInfoTextColor = Settings::obj().itemInfoTextColor();
    ui -> defaultItemInfoTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % itemInfoTextColor.name() % QStringLiteral(";"));

    selectedItemInfoTextColor = Settings::obj().selectedItemInfoTextColor();
    ui -> selectedItemInfoTextColorButton -> setStyleSheet(QStringLiteral("background-color: ") % selectedItemInfoTextColor.name() % QStringLiteral(";"));

    ui -> itemHeightSize -> setValue(Settings::obj().itemHeight());
}
void SettingsDialog::initViewSettings() {
    ui -> showCheckboxes -> setChecked(Settings::obj().isCheckboxShow());

    ui -> alertOnDeletion -> setChecked(Settings::obj().isAlertOnFolderDeletion());
    ui -> spoilOnActivate -> setChecked(Settings::obj().isSpoilOnActivation());

    ui -> unifyHeight -> setChecked(Settings::obj().isHeightUnificate());

    ui -> useSystemIconsCheck -> setChecked(Settings::obj().isShowSystemIcons());
    ui -> indentationStep -> setValue(Settings::obj().treeIndentation());

    ui -> findValid -> setChecked(Settings::obj().isFindValid());

    if (Settings::obj().itemPresentType() == 1)
        ui -> fullyFilledItem -> setChecked(true);
    else
        ui -> partialyFilledItem -> setChecked(true);
}
void SettingsDialog::initHotkeysSettings() {
    ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
    ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
    ui -> treeView -> setModel(new HotkeyModel(Settings::obj().hotKeys(), this));
    ui -> treeView -> hideColumn(2);
    ui -> treeView -> setColumnWidth(0, 230);
}
void SettingsDialog::initSpectrumSettings() {
    ui -> autoBarsAmount -> setChecked(Settings::obj().isAutoBarsAmount());
    ui -> autoBarWidth -> setValue(Settings::obj().autoBarWidth());

    spectrumColor = Settings::obj().spectrumColor();
    ui -> spectrumColor -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor.name() % QStringLiteral(";"));

    spectrumColor2 = Settings::obj().spectrumColor2();
    ui -> spectrumColor2 -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor2.name() % QStringLiteral(";"));

    spectrumColor3 = Settings::obj().spectrumColor3();
    ui -> spectrumColor3 -> setStyleSheet(QStringLiteral("background-color: ") % spectrumColor3.name() % QStringLiteral(";"));

    ui -> spectrumCustomColorUse -> setChecked(Settings::obj().isCustomColorSpectrum());

    ui -> spectrumBarsCount -> setValue(Settings::obj().spectrumBarsCount());
    ui -> spectrumUpdateFrequecy -> setValue(Settings::obj().spectrumFreqRate());

    ui -> spectrumHeight -> setValue(Settings::obj().spectrumHeight());

    ui -> spectrumMultiplier -> setValue(Settings::obj().spectrumMultiplier());

    QStringList spectrumTypes;
    spectrumTypes.append(QStringLiteral("Combined"));
    spectrumTypes.append(QStringLiteral("By Channels"));
    spectrumTypes.append(QStringLiteral("Curved"));

    ui -> spectrumTypeSelect -> insertItems(0, spectrumTypes);
    ui -> spectrumTypeSelect -> setCurrentIndex((int)Settings::obj().spectrumType());
}

void SettingsDialog::initLibrarySettings() {
    ui -> libSaveFreq -> setValue(Settings::obj().saveLibDelay() / 1000.0);
    ui -> remoteItemProcDelay -> setValue(Settings::obj().remoteItemsProcDelay() / 1000.0);
    ui -> interactiveStats -> setChecked(Settings::obj().isInteractiveProc());
    ui -> showInfo -> setChecked(Settings::obj().isShowInfo());
}

void SettingsDialog::initExtensions() {
    extDialog = new ExtensionDialog(this);
    extDialog -> setWindowFlags(Qt::Widget);
    ui -> settingsTabs -> addTab(extDialog, "Extension filters");
}

void SettingsDialog::saveGlobalSettings() {
    Settings::obj().setAutorun(ui -> autorunned -> isChecked());

    Settings::obj().setDefaultDownloadPath(ui -> downloadPath -> text());
    Settings::obj().setMetricShow(ui -> drawMetrics -> isChecked());
    Settings::obj().setMetricNumeroShow(ui -> drawMetricsNumero -> isChecked());
    Settings::obj().setTabPosition(ui -> tabPositionSelect -> currentIndex());

    Settings::obj().setSaveCommonTab(ui -> saveCommonTab -> isChecked());
    Settings::obj().setOpenDropPointInTab(ui -> openDropPointInTab -> isChecked());
    Settings::obj().setOpenDropPointInTabType((Data::Type)(ui -> dropPointTabTypeSelect -> currentIndex() + 1));

    Settings::obj().setOpenTimeOut(ui -> openTimeOut -> value());
}

void SettingsDialog::saveItemsSettings() {
    Settings::obj().setItemHeight(ui -> itemHeightSize -> value());

    Settings::obj().setUseGradient(ui -> useGradientCheck -> isChecked());

    Settings::obj().setDefaultItemColor(defaultColor);
    Settings::obj().setListenedItemColor(listenedColor);
    Settings::obj().setLikedItemColor(likedColor);
    Settings::obj().setPlayedItemColor(playedColor);
    Settings::obj().setFolderItemColor(folderColor);
    Settings::obj().setItemColor(itemColor);

    Settings::obj().setItemFontName(ui -> itemFontSelect -> currentText());
    Settings::obj().setItemFontSize(ui -> itemFontSize -> value());

    Settings::obj().setItemInfoFontName(ui -> itemInfoFontSelect -> currentText());
    Settings::obj().setItemInfoFontSize(ui -> itemInfoFontSize -> value());

    Settings::obj().setItemTextColor(itemTextColor);
    Settings::obj().setItemInfoTextColor(itemInfoTextColor);

    Settings::obj().setSelectedItemTextColor(selectedItemTextColor);
    Settings::obj().setSelectedItemInfoTextColor(selectedItemInfoTextColor);
}

void SettingsDialog::saveViewSettings() {
    Settings::obj().setAlertOnFolderDeletion(ui -> alertOnDeletion -> isChecked());
    Settings::obj().setSpoilOnActivation(ui -> spoilOnActivate -> isChecked());
    Settings::obj().setCheckboxShow(ui -> showCheckboxes -> isChecked());

    iconSizeChanged |= Settings::obj().itemHeight() != ui -> itemHeightSize -> value();
    Settings::obj().setShowInfo(ui -> showInfo -> isChecked());
    iconSizeChanged |= Settings::obj().isHeightUnificate() != ui -> unifyHeight -> isChecked();
    Settings::obj().setHeightUnification(ui -> unifyHeight -> isChecked());

    Settings::obj().setShowSystemIcons(ui -> useSystemIconsCheck -> isChecked());

    Settings::obj().setTreeIndentation(ui -> indentationStep -> value());

    Settings::obj().setItemPresentType(ui -> fullyFilledItem -> isChecked() ? 1 : 2);

    Settings::obj().setFindValid(ui -> findValid -> isChecked());
}

void SettingsDialog::saveHotkeysSettings() {
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> treeView -> model());
    QList<HotkeyModelItem *> list = model -> toplevelItems();
    Settings::obj().setHotKeys(list);

    HotkeyManager::instance() -> clear();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++)
        HotkeyManager::instance() -> registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString());
}

void SettingsDialog::saveSpectrumSettings() {
    Settings::obj().setAutoBarsAmount(ui -> autoBarsAmount -> isChecked());
    Settings::obj().setAutoBarWidth(ui -> autoBarWidth -> value());

    Settings::obj().setSpectrumColor(spectrumColor);
    Settings::obj().setSpectrumColor2(spectrumColor2);
    Settings::obj().setSpectrumColor3(spectrumColor3);

    Settings::obj().setCustomColorSpectrum(ui -> spectrumCustomColorUse -> isChecked());

    Settings::obj().setSpectrumBarsCount(ui -> spectrumBarsCount -> value());
    Settings::obj().setSpectrumFreqRate(ui -> spectrumUpdateFrequecy -> value());

    Settings::obj().setSpectrumHeight(ui -> spectrumHeight -> value());
    Settings::obj().setSpectrumMultiplier(ui -> spectrumMultiplier -> value());
    Settings::obj().setSpectrumType((SpectrumType)ui -> spectrumTypeSelect -> currentIndex());
}

void SettingsDialog::saveLibrarySettings() {
    Settings::obj().setSaveLibDelay(ui -> libSaveFreq -> value() * 1000);
    Settings::obj().setRemoteItemsProcDelay(ui -> remoteItemProcDelay -> value() * 1000);
    Settings::obj().setInteractiveProc(ui -> interactiveStats -> isChecked());

    iconSizeChanged |= Settings::obj().isShowInfo() != ui -> showInfo -> isChecked();
    Settings::obj().setShowInfo(ui -> showInfo -> isChecked());
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

void SettingsDialog::on_autorunned_toggled(bool checked) {
    if (checked)
        Autorun::registerApp();
    else
        Autorun::unregisterApp();
}
