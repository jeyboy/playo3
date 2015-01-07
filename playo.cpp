#include "playo.h"
#include "ui_playo.h"

#include "modules/controls/button.h"

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
//        Player::close();

        Settings::close();
        HotkeyManager::close();
//        Genre::close();

//        VkApi::close();
//        SoundcloudApi::close();
//        ToolBars::close();
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
    addToolBar("lalka");
    addToolBar("lalka2");

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
