#include "playo.h"
#include "ui_playo.h"

#include <QDesktopServices>
#include "misc/stylesheets.h"

using namespace Playo3;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    ui-> centralWidget -> hide();

    QApplication::setWindowIcon(QIcon(":ico"));
    setWindowTitle("Playo");
    setAcceptDrops(true);

    //    setAttribute(Qt::WA_DeleteOnClose);

    activation();
    initialization();
}

Playo::~Playo() {
    delete ui;

    ///////////////////////////////////////////////
    /// close singletons
    ///////////////////////////////////////////////
        Extensions::close();
        IconProvider::close();
//        Library::close();

        Settings::close();
//        Genre::close();

//        VkApi::close();
//        SoundcloudApi::close();
    ///////////////////////////////////////////////

    delete settings;
}

void Playo::activation() {
    Stylesheets::initPens();
    new Tray(this);

    UserDialogBox::instance(this);

    Player::instance(this);
    ToolBars::instance(this);
    Dockbars::instance(this);
    HotkeyManager::instance(this);
}

void Playo::initialization() {
    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    settings = new DataStore("settings.json");

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
//    VkApi::instance(settings -> read("vk").toObject());
//    SoundcloudApi::instance(settings -> read("soundcloud").toObject());

    Settings::instance() -> fromJson(settings -> read("settings").toObject());
    SettingsDialog::registerHotkeys(Dockbars::instance());

    QVariant geometryState = stateSettings.value("geometry");
    if (geometryState.isValid())
        restoreGeometry(geometryState.toByteArray());

    ///////////////////////////////////////////////////////////
    ///location correction (test needed)
    ///////////////////////////////////////////////////////////
    locationCorrection();
    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////
    qint64 v = QDateTime::currentMSecsSinceEpoch();

    QJsonArray bars = settings -> read(ToolBars::settingsName()).toArray();
    ToolBars::instance() -> load(bars);

    QJsonArray docks = settings -> read(Dockbars::settingsName()).toArray();
    Dockbars::instance() -> load(docks);

    qDebug() << "LULU " << QDateTime::currentMSecsSinceEpoch() - v;

    QVariant objState = stateSettings.value("windowState");
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
//    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));
}

QMenu * Playo::createPopupMenu() {
    return ToolBars::instance() -> createPopupMenu(this);
}

void Playo::closeEvent(QCloseEvent * e) {
//    hide(); // hiding window while savings going

    Player::instance() -> pause();

    settings -> clear();

//    settings -> write("vk", VkApi::instance() -> toJson());
//    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

    qint64 v = QDateTime::currentMSecsSinceEpoch();
    ToolBars::instance() -> save(settings);
    Dockbars::instance() -> save(settings);
    qDebug() << "LALA " << QDateTime::currentMSecsSinceEpoch() - v;

    settings -> write("settings", Settings::instance() -> toJson());
    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

    MainWindow::closeEvent(e);
}


/////////////////////////////////////////////////////////////////////////////////////////////////

//void MainWindow::putToCommonTab(QList<QUrl> urls) {
//    ui -> tabber -> commonTab() -> getView() -> getModel() -> dropProcession(urls);
//    ui -> tabber -> commonTab() -> getView() -> getModel() -> refresh();

//    if (!Player::instance() -> isPlayed()) {
//        ui -> tabber -> commonTab() -> getView() -> proceedNext();
//    }
//}

//void MainWindow::dragEnterEvent(QDragEnterEvent * event) {
//    if (event -> mimeData() -> hasUrls()) {
//        event -> accept();
//    } else event -> ignore();
//}
//void MainWindow::dragMoveEvent(QDragMoveEvent * event) {
//    if (event -> mimeData() -> hasUrls()) {
//        event -> accept();
//    } else event -> ignore();
//}

//void MainWindow::dropEvent(QDropEvent * event) {
////    if (event -> source() != this && event -> mimeData() -> hasUrls()) {
//    if (event -> mimeData() -> hasUrls()) {
//        putToCommonTab(event -> mimeData() -> urls());
//    } else event -> ignore();
//}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void Playo::receiveMessage(QString /*message*/) {
//    QStringList list = message.split('|', QString::SkipEmptyParts);
//    QList<QUrl> urls;

//    foreach(QString path, list)
//        urls.append(QUrl::fromLocalFile(path));

//    putToCommonTab(urls);
}

//void MainWindow::showError(QString message) {
//    qDebug() << message;
//}

void Playo::openFolderTriggered() {
    ToolbarButton * button = (ToolbarButton *)QObject::sender();
    QDesktopServices::openUrl(QUrl::fromLocalFile(button -> path));
}

void Playo::showActiveElem() {
//    Tab * tab = ui -> tabber -> toActiveTab();
//    if (tab)
//        tab -> getView() -> scrollToActive();
}

void Playo::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
//        if (dialog.isIconSizeChanged())
//            ui -> tabber -> updateIconSize();

//        ui -> tabber -> setTabPosition((QTabWidget::TabPosition)Settings::instance() -> getTabPosition());
//        ui -> tabber -> setUsesScrollButtons(Settings::instance() -> getScrollButtonUsage());
        ToolBars::instance() -> getSpectrum() -> updateColors();
        ToolBars::instance() -> getSpectrum() -> changeBandCount(Settings::instance() -> getSpectrumBarsCount());
        ToolBars::instance() -> getSpectrum() -> changeHeight(Settings::instance() -> getSpectrumHeight());
        Player::instance() -> setSpectrumFreq(Settings::instance() -> getSpectrumFreqRate());
        Dockbars::instance() -> updateAllViews();
    }
}

//void MainWindow::showVKRelTabDialog() {
//    RelationDialog dialog(VkApi::instance(), this);
//    if (dialog.exec() == QDialog::Accepted) {
//        ui -> tabber -> addTab("VK [" + dialog.getName() + "]", TabDialog::VKSettings(dialog.getId()));
//    } else {
////        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
//    }
//}

//void MainWindow::showSoundcloudRelTabDialog() {
//    RelationDialog dialog(SoundcloudApi::instance(), this);
//    if (dialog.exec() == QDialog::Accepted) {
//        ui -> tabber -> addTab("SC [" + dialog.getName() + "]", TabDialog::soundcloudSettings(dialog.getId()));
//    } else {
////        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
//    }
//}

//void MainWindow::openVKTabDialog() {
//    WebDialog dialog(this, VkApi::instance(), "VK auth");
//    if (dialog.exec() == QDialog::Accepted) {
//        ui -> tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
//        ToolBars::instance() -> initiateVkButton();
//    } else {
//        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
//    }
//}

//void MainWindow::showVKTabDialog() {
//    if (VkApi::instance() -> isConnected()) {
//        ui -> tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
//    } else openVKTabDialog();
//}

//void MainWindow::openSoundcloudTabDialog() {
//    WebDialog dialog(this, SoundcloudApi::instance(), "Soundcloud auth");
//    if (dialog.exec() == QDialog::Accepted) {
//        ui -> tabber -> addTab("SC [YOU]", TabDialog::soundcloudSettings());
//        ToolBars::instance() -> initiateSoundcloudButton();
//    } else {
//        QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
//    }
//}

//void MainWindow::showSoundcloudTabDialog() {
//    if (SoundcloudApi::instance() -> isConnected()) {
//        ui -> tabber -> addTab("SC [YOU]", TabDialog::soundcloudSettings());
//    } else openSoundcloudTabDialog();
//}

//void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
//    if (to && !this -> isActiveWindow())
//        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
//}
