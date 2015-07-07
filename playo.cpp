#include "playo.h"
#include "ui_playo.h"

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

////    QFile f("E:/Muzon/LOSSLESS/Evanescence/2003 - My Immortal (UK 1)/My Immortal (UK Single 1).cue");
//    QFile f("F:/o.cue");
//    if (f.open(QFile::ReadOnly)) {
//        Cue cue(f);

//        qDebug() << cue.songs();

//        f.close();
//    }




    QString s = QString(
        "<html class='no-js' xmlns:fb='http://ogp.me/ns/fb#'>"
        "<head coco lal='1' pido>"
        "<title>DESKTOP SABEResPODER</title>"
        "<meta charset='UTF-8'></meta>"
        "<meta content='' name='description'>"
        "<meta content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0, minimal-ui' name='viewport'>"
        "<link href='http://fonts.googleapis.com/css?family=Ropa+Sans' rel='stylesheet' type='text/css'>"
        "<link rel='stylesheet' href='css/ie8.css'>"
        "<script type='text/javascript'>"
        "   var _gaq = _gaq || [];"
        "   _gaq.push(['_setAccount','UA-42547686-1']);"
        "   _gaq.push(['_setDomainName','auto']);"
        "   _gaq.push(['_trackPageview']);"
        "   (function() {"
        "   var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;"
        "   ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';"
        "   var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);"
        "   })();"
        "</script>"
        "</head>"
        "<body class='home-show-page'>"
        "    <ul class='caps title-area'>"
        "        <li class='name'>dsfsd</li>"
        "    </ul>"
        "</body>"
        "</html>"
    );

    HtmlParser p(s);
    p.output();
    int i = 0;
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
        SoundcloudApi::close();
    ///////////////////////////////////////////////

    delete settings;
}

void Playo::activation() {
    Library::instance((QObject *)QApplication::allWindows().last()); // maybe better create parent for main window and use it for this purpose ?

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
    SoundcloudApi::instance(settings -> read("soundcloud").toObject());

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
    ToolBars::instance() -> setEqualizerSettings(settings -> read("equalizer").toObject());

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
    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

    settings -> write("equalizer", ToolBars::instance() -> getEqualizerSettings());
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
    Dockbars::instance() -> activate(bar);
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
        Dockbars::instance() -> createLinkedDocBar(button -> text(), button -> mainPath(), settings);
//        DockBar * bar = Dockbars::instance() -> createDocBar(button -> text(), settings, 0, true, true);
//        QList<QUrl> urls;
//        urls << QUrl::fromLocalFile(button -> mainPath().mid(0, button -> mainPath().length() - 1));// remove backslash
//        Dockbars::instance() -> view(bar) -> appendRows(urls);
    }
    else QDesktopServices::openUrl(QUrl::fromLocalFile(button -> mainPath()));
}

void Playo::showSearchDialog() {    
    SearchDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ViewSettings settings(search, false, false, false, true);
        SearchSettings prms = dialog.params();
        Dockbars::instance() -> createDocBar("Search", settings, 0, true, true, &prms);
    }
}

void Playo::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ToolBars::instance() -> updateMetricSliders();
        ToolBars::instance() -> getSpectrum() -> updateColors();
        ToolBars::instance() -> getSpectrum() -> changeBandCount();
        ToolBars::instance() -> getSpectrum() -> changeHeight(Settings::instance() -> spectrumHeight());
        ToolBars::instance() -> getSpectrum() -> changeType(Settings::instance() -> spectrumType());
        Player::instance() -> setSpectrumFreq(Settings::instance() -> spectrumFreqRate());
        setTabPosition((QTabWidget::TabPosition)Settings::instance() -> tabPosition());
        Dockbars::instance() -> updateAllViews();
    }
}

void Playo::showEchonestDialog() {
    EchonestDialog d(this);
    d.exec();
}

void Playo::openVKRecomendations() {
    ViewSettings settings(vk_rel, false, false, false, true, VkApi::instance() -> userID(), user_rel);
    Dockbars::instance() -> createDocBar("Rec for YOU", settings, 0, true, true);
}

void Playo::openVKTabDialog() {
    WebDialogInterface * dInt;
    if (loadWebDialogPlugin(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, CustomNetworkAccessManager::manager(), VkApi::instance() -> authUrl(), "VK auth");
        dInt -> registerActions(VkApi::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::instance() -> createDocBar("VK [YOU]", ViewSettings::vk(VkApi::instance() -> userID()), 0, true, true);
        delete dInt;
    }
//    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

void Playo::showVKTabDialog() {
    if (VkApi::instance() -> isConnected())
        Dockbars::instance() -> createDocBar("VK [YOU]", ViewSettings::vk(VkApi::instance() -> userID()), 0, true, true);
    else openVKTabDialog();
}

void Playo::showVKRelTabDialog() {
    RelationsDialog dialog(VkApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
       Dockbars::instance() -> createDocBar("VK [" + dialog.getName() + "]", ViewSettings::vk(dialog.getId()), 0, true, true);
//    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

void Playo::showSoundcloudRelTabDialog() {
    RelationsDialog dialog(SoundcloudApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
        Dockbars::instance() -> createDocBar("SC [" + dialog.getName() + "]", ViewSettings::soundcloud(dialog.getId()), 0, true, true);
//    else QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
}

void Playo::openSoundcloudTabDialog() {
    WebDialogInterface * dInt;
    if (loadWebDialogPlugin(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, CustomNetworkAccessManager::manager(), SoundcloudApi::instance() -> authUrl(), "Soundcloud auth");
        dInt -> registerActions(SoundcloudApi::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::instance() -> createDocBar("SC [YOU]", ViewSettings::soundcloud(SoundcloudApi::instance() -> userID()), 0, true, true);
        delete dInt;
    }
}

void Playo::showSoundcloudTabDialog() {
    if (SoundcloudApi::instance() -> isConnected())
        Qt::TopDockWidgetArea, Dockbars::instance() -> createDocBar("SC [YOU]", ViewSettings::soundcloud(SoundcloudApi::instance() -> userID()), 0, true, true);
    else openSoundcloudTabDialog();
}

//void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
//    if (to && !this -> isActiveWindow())
//        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
//}
