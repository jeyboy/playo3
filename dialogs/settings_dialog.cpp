#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include <QFontDatabase>

using namespace Dialogs;

SettingsDialog::SettingsDialog(QWidget * parent) :
  BaseDialog(parent), ui(new Ui::SettingsDialog), lastTab(0), iconSizeChanged(false) {
    ui -> setupUi(this);

    setWindowTitle(QStringLiteral("Playo settings"));
    setProperty("transparent", true);

    instantiation();

//    setFixedWidth(359);
//    setFixedHeight(312);
//    setSizeGripEnabled(false);
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::instantiation() {
    initExtensions();

    instantiateLayout();

    initGlobalSettings();
    initItemsSettings();
    initViewSettings();
    initHotkeysSettings();
    initSpectrumSettings();
    initLibrarySettings();
}

void SettingsDialog::instantiateLayout() {
    QGridLayout * grid = new QGridLayout(this);

    ui -> linksArea -> setProperty("borderless", true);
    IStylesheets::applyProperty(ui -> linksArea, "transparent", true); // this shit did not work !!!!

    grid -> addWidget(ui -> linksArea, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    grid -> setColumnStretch(0, 0);

    grid -> addWidget(ui -> commonArea, 0, 1); ui -> commonArea -> setVisible(true);
    grid -> addWidget(ui -> viewsArea, 0, 1); ui -> viewsArea -> setVisible(false);
    grid -> addWidget(ui -> itemsArea, 0, 1); ui -> itemsArea -> setVisible(false);
    grid -> addWidget(ui -> hotkeysArea, 0, 1); ui -> hotkeysArea -> setVisible(false);
    grid -> addWidget(ui -> spectrumArea, 0, 1); ui -> spectrumArea -> setVisible(false);
    grid -> addWidget(extDialog, 0, 1, 1, 1, Qt::AlignLeft); extDialog -> setVisible(false);

    grid -> addWidget(ui -> buttons, 1, 0, 1, 2);

    setLayout(grid);

    QButtonGroup * group = new QButtonGroup(this);
    group -> setExclusive(true);

    group -> addButton(ui -> commonBtn, (int)ui -> commonArea);
    group -> addButton(ui -> viewsBtn, (int)ui -> viewsArea);
    group -> addButton(ui -> itemsBtn, (int)ui -> itemsArea);
    group -> addButton(ui -> hotkeysBtn, (int)ui -> hotkeysArea);
    group -> addButton(ui -> spectrumBtn, (int)ui -> spectrumArea);
    group -> addButton(ui -> extensionsBtn, (int)extDialog);

    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(tabClicked(int)));
    ui -> commonBtn -> animateClick();

    setMinimumSize(466, 360);
    setMaximumSize(466, 360);
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::obj().hotKeys();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++) {
        switch((*it) -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playNext()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playNextWithDel()));
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playPrev()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(stop()));
            break;}
            case HOTKEY_OPEN_SETTINGS: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(editActiveBar()));
            break;}

            case HOTKEY_POS_SLIDE_FORWARD: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slidePosForward()));
            break;}

            case HOTKEY_POS_SLIDE_BACKWARD: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slidePosBackward()));
            break;}

            case HOTKEY_VOL_SLIDE_FORWARD: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slideVolForward()));
            break;}

            case HOTKEY_VOL_SLIDE_BACKWARD: {
                HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString(), receiver, SLOT(slideVolBackward()));
            break;}
        }
    }
}

void SettingsDialog::on_cancelButton_clicked() {
    reject();
}

void SettingsDialog::on_resetButton_clicked() {
    if (ui -> commonBtn -> isChecked()) {
        Settings::obj().resetGlobalSettings();
        initGlobalSettings();
        return;
    }

    if (ui -> viewsBtn -> isChecked()) {
        Settings::obj().resetTabSettings();
        initViewSettings();
        return;
    }

    if (ui -> hotkeysBtn -> isChecked()) {
        Settings::obj().resetHotkeySettings();
        initHotkeysSettings();
        return;
    }

    if (ui -> itemsBtn -> isChecked()) {
        Settings::obj().resetItemSettings();
        initItemsSettings();

        Settings::obj().resetLibrarySettings();
        initLibrarySettings();

        return;
    }

    if (ui -> spectrumBtn -> isChecked()) {
        Settings::obj().resetSpectrumSettings();
        initSpectrumSettings();
        return;
    }

    if (ui -> spectrumBtn -> isChecked()) {
        Settings::obj().resetSpectrumSettings();
        initSpectrumSettings();
        return;
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
        setBackgroundColor(ui -> defaultColorButton, defaultColor);
}

void SettingsDialog::on_listenedColorButton_clicked() {
    if (execColorDialog(listenedColor))
        setBackgroundColor(ui -> listenedColorButton, listenedColor);
}

void SettingsDialog::on_likedColorButton_clicked() {
    if (execColorDialog(likedColor))
        setBackgroundColor(ui -> likedColorButton, likedColor);
}

void SettingsDialog::on_playedColorButton_clicked() {
    if (execColorDialog(playedColor))
        setBackgroundColor(ui -> playedColorButton, playedColor);
}

void SettingsDialog::on_folderColorButton_clicked() {
    if (execColorDialog(folderColor))
        setBackgroundColor(ui -> folderColorButton, folderColor);
}

void SettingsDialog::on_itemColorButton_clicked() {
    if (execColorDialog(itemColor))
        setBackgroundColor(ui -> itemColorButton, itemColor);
}

void SettingsDialog::on_defaultItemTextColorButton_clicked() {
    if (execColorDialog(itemTextColor))
        setBackgroundColor(ui -> defaultItemTextColorButton, itemTextColor);
}

void SettingsDialog::on_selectedItemTextColorButton_clicked() {
    if (execColorDialog(selectedItemTextColor))
        setBackgroundColor(ui -> selectedItemTextColorButton, selectedItemTextColor);
}

void SettingsDialog::on_defaultItemInfoTextColorButton_clicked() {
    if (execColorDialog(itemInfoTextColor))
        setBackgroundColor(ui -> defaultItemInfoTextColorButton, itemInfoTextColor);
}

void SettingsDialog::on_selectedItemInfoTextColorButton_clicked() {
    if (execColorDialog(selectedItemInfoTextColor))
        setBackgroundColor(ui -> selectedItemInfoTextColorButton, selectedItemInfoTextColor);
}

void SettingsDialog::on_spectrumColor_clicked() {
    if (execColorDialog(spectrumColor))
        setBackgroundColor(ui -> spectrumColor, spectrumColor);
}

void SettingsDialog::on_spectrumColor2_clicked() {
    if (execColorDialog(spectrumColor2))
        setBackgroundColor(ui -> spectrumColor2, spectrumColor2);
}

void SettingsDialog::on_spectrumColor3_clicked() {
    if (execColorDialog(spectrumColor3))
        setBackgroundColor(ui -> spectrumColor3, spectrumColor3);
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

    ui -> toolIconSize -> setValue(Settings::obj().toolIconSize());

    QStringList schemas;
//    schemas.append(QStringLiteral("Normal"));
    schemas.append(QStringLiteral("Light"));
    schemas.append(QStringLiteral("Dark"));

    ui -> colorScheme -> insertItems(0, schemas);
    ui -> colorScheme -> setCurrentIndex(Settings::obj().colorScheme() - 1);
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
    setBackgroundColor(ui -> defaultColorButton, defaultColor);

    listenedColor = Settings::obj().listenedItemColor();
    setBackgroundColor(ui -> listenedColorButton, listenedColor);

    likedColor = Settings::obj().likedItemColor();
    setBackgroundColor(ui -> likedColorButton, likedColor);

    playedColor = Settings::obj().playedItemColor();
    setBackgroundColor(ui -> playedColorButton, playedColor);

    folderColor = Settings::obj().folderItemColor();
    setBackgroundColor(ui -> folderColorButton, folderColor);

    itemColor = Settings::obj().itemColor();
    setBackgroundColor(ui -> itemColorButton, itemColor);


    itemTextColor = Settings::obj().itemTextColor();
    setBackgroundColor(ui -> defaultItemTextColorButton, itemTextColor);

    selectedItemTextColor = Settings::obj().selectedItemTextColor();
    setBackgroundColor(ui -> selectedItemTextColorButton, selectedItemTextColor);

    itemInfoTextColor = Settings::obj().itemInfoTextColor();
    setBackgroundColor(ui -> defaultItemInfoTextColorButton, itemInfoTextColor);

    selectedItemInfoTextColor = Settings::obj().selectedItemInfoTextColor();
    setBackgroundColor(ui -> selectedItemInfoTextColorButton, selectedItemInfoTextColor);

    ui -> itemHeightSize -> setValue(Settings::obj().itemHeight());
}
void SettingsDialog::initViewSettings() {
    ui -> showCheckboxes -> setChecked(Settings::obj().isCheckboxShow());

    ui -> showNumber -> setChecked(Settings::obj().isShowNumber());

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
    ui -> hotkeysArea -> setEditTriggers(QTreeView::AllEditTriggers);
    ui -> hotkeysArea -> setItemDelegate(new HotkeyDelegate(ui -> hotkeysArea));
    ui -> hotkeysArea -> setModel(new HotkeyModel(Settings::obj().hotKeys(), this));
    ui -> hotkeysArea -> hideColumn(2);
    ui -> hotkeysArea -> setColumnWidth(0, 230);
}
void SettingsDialog::initSpectrumSettings() {
    ui -> autoBarsAmount -> setChecked(Settings::obj().isAutoBarsAmount());
    ui -> autoBarWidth -> setValue(Settings::obj().autoBarWidth());

    spectrumColor = Settings::obj().spectrumColor();
    setBackgroundColor(ui -> spectrumColor, spectrumColor);

    spectrumColor2 = Settings::obj().spectrumColor2();
    setBackgroundColor(ui -> spectrumColor2, spectrumColor2);

    spectrumColor3 = Settings::obj().spectrumColor3();
    setBackgroundColor(ui -> spectrumColor3, spectrumColor3);

    ui -> spectrumCustomColorUse -> setChecked(Settings::obj().isCustomColorSpectrum());

    ui -> spectrumBarsCount -> setValue(Settings::obj().spectrumBarsCount());
    ui -> spectrumUpdateFrequecy -> setValue(Settings::obj().spectrumFreqRate());

    ui -> spectrumHeight -> setValue(Settings::obj().spectrumHeight());

    ui -> spectrumMultiplier -> setValue(Settings::obj().spectrumMultiplier());

    QStringList spectrumTypes;
    spectrumTypes.append(QStringLiteral("Combined"));
    spectrumTypes.append(QStringLiteral("By Channels"));

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
    extDialog -> findChild<QWidget *>(QStringLiteral("submitButtons")) -> setVisible(false);
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

    Settings::obj().setToolIconSize(ui -> toolIconSize -> value());
    Settings::obj().setOpenTimeOut(ui -> openTimeOut -> value());
    Settings::obj().setColorScheme(ui -> colorScheme -> currentIndex() + 1);
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
    Settings::obj().setShowNumber(ui -> showNumber -> isChecked());

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
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> hotkeysArea -> model());
    QList<HotkeyModelItem *> list = model -> toplevelItems();
    Settings::obj().setHotKeys(list);

    HotkeyManager::obj().clear();

    QList<HotkeyModelItem *>::Iterator it = list.begin();
    for(; it != list.end(); it++)
        HotkeyManager::obj().registerSequence((*it) -> data(2).toInt(), (*it) -> data(1).toString());
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

void Dialogs::SettingsDialog::on_colorScheme_activated(int index) {
    Settings::setCurrentStyle((IStylesheets::StyleType)(index + 1));
}

void Dialogs::SettingsDialog::tabClicked(int obj_id) {
    if (lastTab) lastTab -> setVisible(false);
    (lastTab = (QWidget *)obj_id) -> setVisible(true);
}

//void Dialogs::SettingsDialog::on_commonBtn_clicked(bool checked) {
//    ui -> commonArea -> setVisible(checked);
////    ui -> viewsArea -> setVisible(false);
////    ui -> itemsArea -> setVisible(false);
////    ui -> hotkeysArea -> setVisible(false);
////    ui -> spectrumArea -> setVisible(false);
////    extDialog -> setVisible(false);
//}

//void Dialogs::SettingsDialog::on_viewsBtn_clicked(bool checked) {
////    ui -> commonArea -> setVisible(false);
//    ui -> viewsArea -> setVisible(checked);
////    ui -> itemsArea -> setVisible(false);
////    ui -> hotkeysArea -> setVisible(false);
////    ui -> spectrumArea -> setVisible(false);
////    extDialog -> setVisible(false);
//}

//void Dialogs::SettingsDialog::on_itemsBtn_clicked(bool checked) {
////    ui -> commonArea -> setVisible(false);
////    ui -> viewsArea -> setVisible(false);
//    ui -> itemsArea -> setVisible(checked);
////    ui -> hotkeysArea -> setVisible(false);
////    ui -> spectrumArea -> setVisible(false);
////    extDialog -> setVisible(false);
//}

//void Dialogs::SettingsDialog::on_hotkeysBtn_clicked(bool checked) {
////    ui -> commonArea -> setVisible(false);
////    ui -> viewsArea -> setVisible(false);
////    ui -> itemsArea -> setVisible(false);
//    ui -> hotkeysArea -> setVisible(checked);
////    ui -> spectrumArea -> setVisible(false);
////    extDialog -> setVisible(false);
//}

//void Dialogs::SettingsDialog::on_spectrumBtn_clicked(bool checked) {
////    ui -> commonArea -> setVisible(false);
////    ui -> viewsArea -> setVisible(false);
////    ui -> itemsArea -> setVisible(false);
////    ui -> hotkeysArea -> setVisible(false);
//    ui -> spectrumArea -> setVisible(checked);
////    extDialog -> setVisible(false);
//}

//void Dialogs::SettingsDialog::on_extensionsBtn_clicked(bool checked) {
////    ui -> commonArea -> setVisible(false);
////    ui -> viewsArea -> setVisible(false);
////    ui -> itemsArea -> setVisible(false);
////    ui -> hotkeysArea -> setVisible(false);
////    ui -> spectrumArea -> setVisible(false);
//    extDialog -> setVisible(checked);
//}
