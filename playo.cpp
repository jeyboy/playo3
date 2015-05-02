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

    setTabShape(QTabWidget::Rounded);

    //    setAttribute(Qt::WA_DeleteOnClose);

    initialization();
}

Playo::~Playo() {
    delete ui;

    ///////////////////////////////////////////////
    /// close singletons
    ///////////////////////////////////////////////
        Extensions::close();
        IconProvider::close();
        Settings::close();
//        Genre::close();

        VkApi::close();
//        SoundcloudApi::close();
    ///////////////////////////////////////////////

    delete settings;
}

void Playo::activation() {
    Library::instance(QApplication::allWindows().last());

    Stylesheets::initPens();
    new Tray(this);
    UserDialogBox::instance(this);
    Player::instance(this);
    ToolBars::instance(this);
    Dockbars::instance(this);
    HotkeyManager::instance(this);
}

void Playo::initialization() {
    Logger::instance() -> startMark();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    settings = new DataStore("settings.json");

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
    VkApi::instance(this, settings -> read("vk").toObject());
//    SoundcloudApi::instance(settings -> read("soundcloud").toObject());

    Settings::instance() -> fromJson(settings -> read("settings").toObject());

    activation();

    SettingsDialog::registerHotkeys(Dockbars::instance());

    setTabPosition((QTabWidget::TabPosition)Settings::instance() -> tabPosition());

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

    QJsonArray bars = settings -> read(ToolBars::settingsName()).toArray();
    ToolBars::instance() -> load(bars);

    QJsonArray docks = settings -> read(Dockbars::settingsName()).toArray();
    Dockbars::instance() -> load(docks);

    QVariant objState = stateSettings.value("windowState");
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
//    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));
    Dockbars::instance() -> updateActiveTabIcon();

    if (stateSettings.value("maximized").toBool()) {
        QApplication::processEvents();
        showMaximized();
    }
    Logger::instance() -> endMark("Main", "Loading");
}

QMenu * Playo::createPopupMenu() {
    return ToolBars::instance() -> createPopupMenu(this);
}

void Playo::closeEvent(QCloseEvent * e) {
    Logger::instance() -> unregisterEditor();
    bool is_maximized = isMaximized();
    showNormal();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.setValue("maximized", is_maximized);
    stateSettings.sync();

    setWindowState(Qt::WindowMinimized); // hiding window while savings going

    Logger::instance() -> startMark();

    Player::instance() -> pause();

    settings -> clear();

    settings -> write("vk", VkApi::instance() -> toJson());
//    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

    ToolBars::instance() -> save(settings);
    Dockbars::instance() -> save(settings);

    settings -> write("settings", Settings::instance() -> toJson());
    settings -> save();

    Logger::instance() -> endMark("Main", "Saving");

    MusicGenres::close();
    MainWindow::closeEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Playo::putToCommonTab(QList<QUrl> urls) {
    DockBar * bar = Dockbars::instance() -> commonBar();
    Dockbars::instance() -> view(bar) -> appendRows(urls);

//    if (!Player::instance() -> isPlayed())

    bar -> show();
}

void Playo::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasUrls())
        event -> accept();
    else event -> ignore();
}
void Playo::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> mimeData() -> hasUrls())
        event -> accept();
    else event -> ignore();
}

void Playo::dropEvent(QDropEvent * event) {
    if (event -> mimeData() -> hasUrls())
        putToCommonTab(event -> mimeData() -> urls());
    else event -> ignore();
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void Playo::receiveMessage(QString message) {
    Logger::instance() -> write("Main", "receiveMessage");
    QStringList list = message.split('|', QString::SkipEmptyParts);
    QList<QUrl> urls;

    QStringList::iterator it = list.begin();

    for(; it != list.end(); it++)
        urls.append(QUrl::fromLocalFile((*it)));

    putToCommonTab(urls);
}

//void MainWindow::showError(QString message) {
//    qDebug() << message;
//}

void Playo::openFolderTriggered() {
    ToolbarButton * button = (ToolbarButton *)QObject::sender();
    if (!(button -> keyboardModifiers() & Qt::ControlModifier) && Settings::instance() -> isOpenDropPointInTab()) {
        ViewSettings settings(Settings::instance() -> openDropPointInTabType(), false, false, false, true);
        DockBar * bar = Dockbars::instance() -> createDocBar(button -> text(), settings);
        addDockWidget(Qt::TopDockWidgetArea, bar);
        QList<QUrl> urls;
        urls << QUrl::fromLocalFile(button -> mainPath().mid(0, button -> mainPath().length() - 1));// remove backslash
        Dockbars::instance() -> view(bar) -> appendRows(urls);
    }
    else QDesktopServices::openUrl(QUrl::fromLocalFile(button -> mainPath()));
}

void Playo::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ToolBars::instance() -> getSpectrum() -> updateColors();
        ToolBars::instance() -> getSpectrum() -> changeBandCount(Settings::instance() -> spectrumBarsCount());
        ToolBars::instance() -> getSpectrum() -> changeHeight(Settings::instance() -> spectrumHeight());
        ToolBars::instance() -> getSpectrum() -> changeType(Settings::instance() -> spectrumType());
        Player::instance() -> setSpectrumFreq(Settings::instance() -> spectrumFreqRate());
        setTabPosition((QTabWidget::TabPosition)Settings::instance() -> tabPosition());
        Dockbars::instance() -> updateAllViews();
    }
}

void Playo::openVKTabDialog() {
    WebDialog dialog(this, VkApi::instance(), "VK auth");
    if (dialog.exec() == QDialog::Accepted) {
        ViewSettings settings(vk, false, false, false, true, VkApi::instance() -> getUserID());
        addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance() -> createDocBar("VK [YOU]", settings));
        ToolBars::instance() -> initiateVkButton();
    }
    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

void Playo::showVKTabDialog() {
    if (VkApi::instance() -> isConnected()) {
        ViewSettings settings(vk, false, false, false, true, VkApi::instance() -> getUserID());
        addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance() -> createDocBar("VK [YOU]", settings));
    }
    else openVKTabDialog();
}

void Playo::showVKRelTabDialog() {
    RelationsDialog dialog(VkApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted) {
        ViewSettings settings(vk, false, false, false, true, dialog.getId());
         addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance() -> createDocBar("VK [" + dialog.getName() + "]", settings));
    }
    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

//void MainWindow::showSoundcloudRelTabDialog() {
//    RelationDialog dialog(SoundcloudApi::instance(), this);
//    if (dialog.exec() == QDialog::Accepted) {
//        ui -> tabber -> addTab("SC [" + dialog.getName() + "]", TabDialog::soundcloudSettings(dialog.getId()));
//    } else {
////        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
//    }
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
