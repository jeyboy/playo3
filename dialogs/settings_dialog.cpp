#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include <QFontDatabase>
#include "media/player.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog) {
  ui -> setupUi(this);

  setWindowTitle("Playo settings");
  setFixedWidth(356);
  setFixedHeight(305);
  setSizeGripEnabled(false);

  ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
  ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
  ui -> treeView -> setModel(new HotkeyModel(Settings::instance() -> getHotKeys(), this));
  ui -> treeView -> hideColumn(2);
  ui -> treeView -> setColumnWidth(0, 230);

  ui -> downloadPath -> setText(Settings::instance() -> getDownloadPath());

  ui -> showCheckboxes -> setChecked(Settings::instance() -> isCheckboxShow());
  ui -> drawMetrics -> setChecked(Settings::instance() -> isMetricShow());
  ui -> spoilOnActivate -> setChecked(Settings::instance() -> isSpoilOnActivation());
  ui -> showInfo -> setChecked(Settings::instance() -> isShowInfo());

  ui -> useSystemIconsCheck -> setChecked(Settings::instance() -> isShowSystemIcons());

  ui -> useGradientCheck -> setChecked(Settings::instance() -> isUseGradient());

  ui -> itemFontSize -> setValue(Settings::instance() -> getItemFontSize());
  ui -> itemInfoFontSize -> setValue(Settings::instance() -> getItemInfoFontSize());

  QStringList positions;
  positions.append("Above");
  positions.append("Below");
  positions.append("Left");
  positions.append("Right");

  ui -> tabPositionSelect -> insertItems(0, positions);
  ui -> tabPositionSelect -> setCurrentIndex(Settings::instance() -> getTabPosition());

  QFontDatabase fontDatabase;

  ui -> itemFontSelect -> insertItems(0, fontDatabase.families());
  ui -> itemFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::instance() -> getItemFontName())));
  ui -> itemInfoFontSelect -> insertItems(0, fontDatabase.families());
  ui -> itemInfoFontSelect -> setCurrentIndex(fontDatabase.families().indexOf(QRegExp(Settings::instance() -> getItemInfoFontName())));

  defaultColor = Settings::instance() -> getDefaultColor();
  ui -> defaultColorButton -> setStyleSheet("background-color: " + defaultColor.name() + ";");

  listenedColor = Settings::instance() -> getListenedColor();
  ui -> listenedColorButton -> setStyleSheet("background-color: " + listenedColor.name() + ";");

  likedColor = Settings::instance() -> getLikedColor();
  ui -> likedColorButton -> setStyleSheet("background-color: " + likedColor.name() + ";");

  playedColor = Settings::instance() -> getPlayedColor();
  ui -> playedColorButton -> setStyleSheet("background-color: " + playedColor.name() + ";");

  folderColor = Settings::instance() -> getFolderColor();
  ui -> folderColorButton -> setStyleSheet("background-color: " + folderColor.name() + ";");
  
  
  itemTextColor = Settings::instance() -> getItemTextColor();
  ui -> defaultItemTextColorButton -> setStyleSheet("background-color: " + itemTextColor.name() + ";");

  selectedItemTextColor = Settings::instance() -> getSelectedItemTextColor();
  ui -> selectedItemTextColorButton -> setStyleSheet("background-color: " + selectedItemTextColor.name() + ";");

  itemInfoTextColor = Settings::instance() -> getItemInfoTextColor();
  ui -> defaultItemInfoTextColorButton -> setStyleSheet("background-color: " + itemInfoTextColor.name() + ";");

  selectedItemInfoTextColor = Settings::instance() -> getSelectedItemInfoTextColor();
  ui -> selectedItemInfoTextColorButton -> setStyleSheet("background-color: " + selectedItemInfoTextColor.name() + ";");

  ui -> itemHeightSize -> setValue(Settings::instance() -> getItemHeight());
  ui -> indentationStep -> setValue(Settings::instance() -> getTreeIndentation());

  spectrumColor = Settings::instance() -> getSpectrumColor();
  ui -> spectrumColor -> setStyleSheet("background-color: " + spectrumColor.name() + ";");

  spectrumColor2 = Settings::instance() -> getSpectrumColor2();
  ui -> spectrumColor2 -> setStyleSheet("background-color: " + spectrumColor2.name() + ";");

  spectrumColor3 = Settings::instance() -> getSpectrumColor3();
  ui -> spectrumColor3 -> setStyleSheet("background-color: " + spectrumColor3.name() + ";");

  ui -> spectrumCustomColorUse -> setChecked(Settings::instance() -> isCustomColorSpectrum());

  ui -> spectrumBarsCount -> setValue(Settings::instance() -> getSpectrumBarsCount());
  ui -> spectrumUpdateFrequecy -> setValue(Settings::instance() -> getSpectrumFreqRate());

  ui -> spectrumHeight -> setValue(Settings::instance() -> getSpectrumHeight());

  ui -> spectrumMultiplier -> setValue(Settings::instance() -> getSpectrumMultiplier());
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::instance() -> getHotKeys();

    foreach(HotkeyModelItem * key, list) {
        switch(key -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextExecTriggering()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextExecWithDelTriggering())); /*onNextItemRequiring*/
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(prevExecTriggering()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(stop()));
            break;}
            case HOTKEY_SETTINGS: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(editActiveBar()));
            break;}


            case HOTKEY_POS_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slidePosForward()));
            break;}

            case HOTKEY_POS_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slidePosBackward()));
            break;}

            case HOTKEY_VOL_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slideVolForward()));
            break;}

            case HOTKEY_VOL_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slideVolBackward()));
            break;}
        }
    }
}

void SettingsDialog::on_cancelButton_clicked() {
    reject();
}

void SettingsDialog::on_acceptButton_clicked() {
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> treeView -> model());
    QList<HotkeyModelItem *> list = model -> toplevelItems();
    Settings::instance() -> setHotKeys(list);

    HotkeyManager::instance() -> clear();

    foreach(HotkeyModelItem * key, list) {
        HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString());
    }

    Settings::instance() -> setDownloadPath(ui -> downloadPath -> text());
    Settings::instance() -> setCheckboxShow(ui -> showCheckboxes -> isChecked());
    Settings::instance() -> setMetricShow(ui -> drawMetrics -> isChecked());

    Settings::instance() -> setSpoilOnActivation(ui -> spoilOnActivate -> isChecked());

    iconSizeChanged = Settings::instance() -> isShowInfo() != ui -> showInfo -> isChecked()
            || Settings::instance() -> getItemHeight() != ui -> itemHeightSize -> value();
    Settings::instance() -> setShowInfo(ui -> showInfo -> isChecked());

    Settings::instance() -> setShowSystemIcons(ui -> useSystemIconsCheck -> isChecked());

    Settings::instance() -> setUseGradient(ui -> useGradientCheck -> isChecked());

    Settings::instance() -> setDefaultColor(defaultColor);
    Settings::instance() -> setListenedColor(listenedColor);
    Settings::instance() -> setLikedColor(likedColor);
    Settings::instance() -> setPlayedColor(playedColor);
    Settings::instance() -> setFolderColor(folderColor);

    Settings::instance() -> setItemFontName(ui -> itemFontSelect -> currentText());
    Settings::instance() -> setItemFontSize(ui -> itemFontSize -> value());

    Settings::instance() -> setItemInfoFontName(ui -> itemInfoFontSelect -> currentText());
    Settings::instance() -> setItemInfoFontSize(ui -> itemInfoFontSize -> value());

    Settings::instance() -> setItemTextColor(itemTextColor);
    Settings::instance() -> setItemInfoTextColor(itemInfoTextColor);

    Settings::instance() -> setSelectedItemTextColor(selectedItemTextColor);
    Settings::instance() -> setSelectedItemInfoTextColor(selectedItemInfoTextColor);

    Settings::instance() -> setTabPosition(ui -> tabPositionSelect -> currentIndex());

    Settings::instance() -> setItemHeight(ui -> itemHeightSize -> value());
    Settings::instance() -> setTreeIndentation(ui -> indentationStep -> value());

    Settings::instance() -> setSpectrumColor(spectrumColor);
    Settings::instance() -> setSpectrumColor2(spectrumColor2);
    Settings::instance() -> setSpectrumColor3(spectrumColor3);
    Settings::instance() -> setCustomColorSpectrum(ui -> spectrumCustomColorUse -> isChecked());

    Settings::instance() -> setSpectrumBarsCount(ui -> spectrumBarsCount -> value());
    Settings::instance() -> setSpectrumFreqRate(ui -> spectrumUpdateFrequecy -> value());

    Settings::instance() -> setSpectrumHeight(ui -> spectrumHeight -> value());
    Settings::instance() -> setSpectrumMultiplier(ui -> spectrumMultiplier -> value());

    accept();
}

void SettingsDialog::on_browseButton_clicked() {
    QString path = QFileDialog::getExistingDirectory(this, "Please choose new download path");
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

bool SettingsDialog::execColorDialog(QColor & color) {
    QColorDialog col(this);
    col.setCurrentColor(color);
    if (col.exec() == QColorDialog::Accepted) {
        color = col.selectedColor();
        return true;
    }

    return false;
}
