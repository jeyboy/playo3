#include "playo.h"
#include "ui_playo.h"

//#include "modules/controls/button.h"
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
    return ToolBars::instance(this) -> createPopupMenu(this);
}

void Playo::closeEvent(QCloseEvent * e) {
    settings -> clear();

//    settings -> write("vk", VkApi::instance() -> toJson());
//    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

    ToolBars::instance(this) -> save(settings);
    Dockbars::instance(this) -> save(settings);

    settings -> write("settings", Settings::instance() -> toJson());
    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

    MainWindow::closeEvent(e);
}

void Playo::init() {
    Stylesheets::initPens();
    new Tray(this);

//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU", new QLabel("sdfsfsdf")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU1", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU2", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU3", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU4", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU5", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU6", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU7", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU8", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU9", new QLabel("dfgdfgdfg")));
//    addDockWidget(Qt::TopDockWidgetArea, Dockbars::instance(this) -> createDocBar("PU10", new QLabel("dfgdfgdfg")));

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
    ToolBars::instance(this) -> load(bars);

    QJsonArray docks = settings -> read(Dockbars::settingsName()).toArray();
    Dockbars::instance(this) -> load(docks);

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

void Playo::showAttTabDialog(DockBar * bar) {
    TabDialog dialog(this);
    if(bar) {
        ViewInterface * view = dynamic_cast<ViewInterface *>(bar -> widget());
        dialog.setSettings(view -> settings());
        dialog.setName(bar -> windowTitle());

        if (dialog.exec() == QDialog::Accepted) {
            bar -> setWindowTitle(dialog.getName());
            view -> setSettings(dialog.getSettings());
        }
    } else {
        if (dialog.exec() == QDialog::Accepted) {
            ViewInterface * view;
            ViewSettings settings = dialog.getSettings();

            switch(settings.type) {
                case list: {
                    view = new ListView(0, settings);
                break;}
                case level_tree: {
                    view = new LevelTreeView(0, settings);
                break;}
                case tree: {
                    view = new TreeView(0, settings);
                break;}
                default: view = 0;
            }
            addDockWidget(
                Qt::LeftDockWidgetArea,
                Dockbars::instance(this) -> createDocBar(dialog.getName(), view)
            );
        }
    }
}
