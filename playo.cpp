#include "playo.h"
#include "ui_playo.h"

//#include "modules/controls/button.h"
#include <QDesktopServices>

using namespace Playo3;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    ui-> centralWidget -> hide();

    QApplication::setWindowIcon(QIcon(":ico"));
    setWindowTitle("Playo");
    setAcceptDrops(true);

    //    setAttribute(Qt::WA_DeleteOnClose);

    init();
}

Playo::~Playo() {
    delete ui;

    ///////////////////////////////////////////////
    /// close singletons
    ///////////////////////////////////////////////
//        Extensions::close();
//        IconProvider::close();
//        Library::close();
        Player::close();

        Settings::close();
        HotkeyManager::close();
//        Genre::close();

//        VkApi::close();
//        SoundcloudApi::close();
////        ToolBars::close();
    ///////////////////////////////////////////////

    delete settings;
}

QMenu * Playo::createPopupMenu() {
    return ToolBars::instance(this) -> improvePopupMenu(this, QMainWindow::createPopupMenu());
}

void Playo::closeEvent(QCloseEvent * e) {
    settings -> clear();

//    settings -> write("vk", VkApi::instance() -> toJson());
//    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

    ToolBars::instance(this) -> save(this, settings);
    Dockbars::instance(this) -> save(this, settings);

    settings -> write("settings", Settings::instance() -> toJson());
    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

//    ui -> tabber -> save();
    MainWindow::closeEvent(e);
}

void Playo::init() {
    new Tray(this);

    addDockWidget(Qt::TopDockWidgetArea, new QDockWidget("pido", this));
    addDockWidget(Qt::TopDockWidgetArea, new QDockWidget("pipido", this));

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    settings = new DataStore("settings.json");

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
//    VkApi::instance(settings -> read("vk").toObject());
//    SoundcloudApi::instance(settings -> read("soundcloud").toObject());

    Settings::instance() -> fromJson(settings -> read("settings").toObject());
    SettingsDialog::registerHotkeys(this);

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
    ToolBars::instance(this) -> load(this, bars);

    QJsonArray docks = settings -> read(Dockbars::settingsName()).toArray();
    Dockbars::instance(this) -> load(this, docks);

    QVariant objState = stateSettings.value("windowState");
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
//    ui -> tabber -> load();

//    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));
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

void Playo::receiveMessage(QString message) {
//    QStringList list = message.split('|', QString::SkipEmptyParts);
//    QList<QUrl> urls;

//    foreach(QString path, list)
//        urls.append(QUrl::fromLocalFile(path));

//    putToCommonTab(urls);
}

void Playo::showAttCurrTabDialog() {
////    emit showAttTabDialog(tabber -> currentTab());

//    if (ui -> tabber -> currentTab() -> isEditable())
//        emit showAttTabDialog(ui -> tabber -> currentTab());
//    else
//        QMessageBox::warning(this, "Settings", "This tab type did not have any settings...");
}

//void MainWindow::showError(QString message) {
//    qDebug() << message;
//}

void Playo::nextItemTriggered() {
//    if (ui -> tabber -> currentTab())
//        ui -> tabber -> currentTab() -> getView() -> proceedNext();
}
void Playo::nextItemWithDelTriggered() {
//    if (ui -> tabber -> currentTab())
//        ui -> tabber -> currentTab() -> getView() -> deleteCurrentProceedNext();
}
void Playo::prevItemTriggered() {
//    if (ui -> tabber -> currentTab())
//        ui -> tabber -> currentTab() -> getView() -> proceedPrev();
}

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
        ToolBars::instance(this) -> getSpectrum() -> bandCountChanged(Settings::instance() -> getSpectrumBarsCount());
        ToolBars::instance(this) -> getSpectrum() -> heightChanged(Settings::instance() -> getSpectrumHeight());
        Player::instance() -> setSpectrumFreq(Settings::instance() -> getSpectrumFreqRate());
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
//        ToolBars::instance(this) -> initiateVkButton();
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
//        ToolBars::instance(this) -> initiateSoundcloudButton();
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

//void MainWindow::showAttTabDialog(Tab * tab) {
//  TabDialog dialog(this);
//  if(tab) {
//      qDebug() << tab -> getName();
//      dialog.setSettings(tab -> getView() -> getSettings());
//      dialog.setName(tab -> getName());

//      while(true) {
//          if (dialog.exec() == QDialog::Accepted) {
//              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
//                  tab -> setName(dialog.getName());
//                  tab -> getView() -> setSettings(dialog.getSettings());
//                  return;
//              }
//          } else return;
//      }
//  } else {
//      while(true) {
//          if (dialog.exec() == QDialog::Accepted) {
//              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
//                  ui -> tabber -> addTab(dialog.getName(), dialog.getSettings());
//                  return;
//              }
//          } else return;
//      }
//  }
//}
