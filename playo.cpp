#include "playo.h"
#include "ui_playo.h"

#include "stylesheets.h"

using namespace Presentation;
using namespace Dialogs;
using namespace Data;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    ui-> centralWidget -> hide();
    QApplication::setWindowIcon(QIcon(QStringLiteral(":ico")));
    setWindowTitle(QStringLiteral("Playo"));
    setAcceptDrops(true);

    setTabShape(QTabWidget::Rounded);

    //    setAttribute(Qt::WA_DeleteOnClose);

    initialization();

    //safe usage of pointers
//    QPointer dlg = new SomeDialog( this );

//    qDebug() << "LAL" << QByteArray::fromPercentEncoding("/Artist/85834/%D0%A1%D0%B5%D1%80%D0%B3%D0%B5%D0%B8-%D0%96%D1%83%D0%BA%D0%BE%D0%B2");
//    ba = QByteArray::fromPercentEncoding(ba);
//    QTextCodec* codec = QTextCodec::codecForName("windows-1251");
//    QString res  =codec->toUnicode(ba);

////    QFile f("E:/Muzon/LOSSLESS/Evanescence/2003 - My Immortal (UK 1)/My Immortal (UK Single 1).cue");
//    QFile f("F:/o.cue");
//    if (f.open(QFile::ReadOnly)) {
//        Cue cue(f);
}

Playo::~Playo() {
    delete ui;
    delete settings;
}

void Playo::activation() {
    Stylesheets::initPens();
    new Tray(this);
    UserDialogBox::instance(this);
    Player::instance(this);
    ToolBars::obj().setContainer(this);
    Dockbars::obj().setContainer(this);
    HotkeyManager::instance(this);
}

void Playo::initialization() {
    Logger::obj().startMark();

    QSettings stateSettings(FRONT_SETTINGS_FILE_NAME, QSettings::IniFormat, this);
    settings = new DataStore(BACKEND_SETTINGS_FILE_NAME);

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
    Web::Apis::initiate(this, settings -> obj());
    Settings::obj().fromJson(settings -> read(SETTINGS_SET_KEY).toObject());

    activation();

    SettingsDialog::registerHotkeys(&Dockbars::obj());

    setTabPosition((QTabWidget::TabPosition)Settings::obj().tabPosition());

    QVariant geometryState = stateSettings.value(SETTINGS_GEOMETRY_SET_KEY);
    if (geometryState.isValid())
        restoreGeometry(geometryState.toByteArray());

    ///////////////////////////////////////////////////////////
    ///location correction (test needed)
    ///////////////////////////////////////////////////////////
    locationCorrection();
    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////

    ToolBars::obj().load(settings -> read(ToolBars::settingsName()).toArray());
    ToolBars::obj().setEqualizerSettings(settings -> read(SETTINGS_EQUALIZER_SET_KEY).toObject());

    Dockbars::obj().load(settings -> read(Dockbars::settingsName()).toArray());
    connect(Player::instance(), SIGNAL(nextItemNeeded(Player::Reason)), &Dockbars::obj(), SLOT(onNextItemNeeded(Player::Reason)));

    QVariant objState = stateSettings.value(SETTINGS_WINDOW_STATE_SET_KEY);
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
//    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));
    Dockbars::obj().updateActiveTabIcon();

    if (stateSettings.value(SETTINGS_WINDOW_MAXIMIZED_KEY).toBool()) {
        QApplication::processEvents();
        showMaximized();
    }
    Logger::obj().endMark(tr("Main"), tr("Loading"));
}

QMenu * Playo::createPopupMenu() {
    return ToolBars::obj().createPopupMenu();
}

void Playo::closeEvent(QCloseEvent * e) {
    Logger::obj().unregisterEditor();
    bool is_maximized = isMaximized();
    showNormal();

    QSettings stateSettings(FRONT_SETTINGS_FILE_NAME, QSettings::IniFormat, this);
    stateSettings.setValue(SETTINGS_GEOMETRY_SET_KEY, saveGeometry());
    stateSettings.setValue(SETTINGS_WINDOW_STATE_SET_KEY, saveState());
    stateSettings.setValue(SETTINGS_WINDOW_MAXIMIZED_KEY, is_maximized);
    stateSettings.sync();

    setWindowState(Qt::WindowMinimized); // hiding window while savings going

    Logger::obj().startMark();

    Player::instance() -> pause();

    settings -> clear();

    settings -> write(SETTINGS_EQUALIZER_SET_KEY, ToolBars::obj().getEqualizerSettings());
    ToolBars::obj().save(settings);
    Dockbars::obj().save(settings);

    settings -> write(SETTINGS_SET_KEY, Settings::obj().toJson());
    Web::Apis::close(settings -> obj());
    settings -> save();

    Logger::obj().endMark(QStringLiteral("Main"), QStringLiteral("Saving"));

    MainWindow::closeEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Playo::putToCommonTab(QList<QUrl> urls) {
    DockBar * bar = Dockbars::obj().commonBar();
    Dockbars::obj().view(bar) -> appendRows(urls);

//    if (!Player::instance() -> isPlayed())

    bar -> show();
    Dockbars::obj().activate(bar);
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
    Logger::obj().write(QStringLiteral("Main"), QStringLiteral("receiveMessage"));
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
    if (!(button -> keyboardModifiers() & Qt::ControlModifier) && Settings::obj().isOpenDropPointInTab()) {
        View::Params settings(Settings::obj().openDropPointInTabType(), false, false, false, true);
        Dockbars::obj().createLinkedDocBar(button -> text(), button -> mainPath(), settings);
//        DockBar * bar = Dockbars::obj().createDocBar(button -> text(), settings, 0, true, true);
//        QList<QUrl> urls;
//        urls << QUrl::fromLocalFile(button -> mainPath().mid(0, button -> mainPath().length() - 1));// remove backslash
//        Dockbars::obj().view(bar) -> appendRows(urls);
    }
    else QDesktopServices::openUrl(QUrl::fromLocalFile(button -> mainPath()));
}

void Playo::showSearchDialog() {    
    SearchDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        View::Params settings(search, false, false, false, true);
        SearchSettings prms = dialog.params();
        Dockbars::obj().createDocBar("Search", settings, 0, true, true, &prms);
    }
}

void Playo::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ToolBars::obj().updateMetricSliders();
        ToolBars::obj().getSpectrum() -> updateColors();
        ToolBars::obj().getSpectrum() -> changeBandCount();
        ToolBars::obj().getSpectrum() -> changeHeight(Settings::obj().spectrumHeight());
        ToolBars::obj().getSpectrum() -> changeType(Settings::obj().spectrumType());
        Player::instance() -> setSpectrumFreq(Settings::obj().spectrumFreqRate());
        setTabPosition((QTabWidget::TabPosition)Settings::obj().tabPosition());
        Dockbars::obj().updateAllViews();
    }
}

void Playo::showEchonestDialog() {
    EchonestDialog d(this);
    d.exec();
}

void Playo::openOdTabDialog() {
    if (Od::Api::instance() -> isConnected() || Od::Api::instance() -> connection())
        Dockbars::obj().createDocBar(QStringLiteral("OD [YOU]"), View::Params(od, Od::Api::instance() -> userID()), 0, true, true);
}

void Playo::openVKRecomendations() {
    Dockbars::obj().createDocBar(QStringLiteral("Rec for YOU"), View::Params(vk_rel, Vk::Api::instance() -> userID(), user_rel), 0, true, true);
}

void Playo::openVKTabDialog() {
    WebDialogInterface * dInt;
    if (Plugins::loadWebDialog(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, Web::Manager::prepare(), Vk::Api::instance() -> authUrl(), QStringLiteral("VK auth"));
        dInt -> registerActions(Vk::Api::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::obj().createDocBar(QStringLiteral("VK [YOU]"), View::Params(vk, Vk::Api::instance() -> userID()), 0, true, true);

        emit Logger::obj().write(QStringLiteral("VkApi"), QStringLiteral("Connection"), Vk::Api::instance() -> isConnected() ? QStringLiteral("true") : Vk::Api::instance() -> getError());
        delete dInt;
    }
//    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

void Playo::showVKTabDialog() {
    if (Vk::Api::instance() -> isConnected())
        Dockbars::obj().createDocBar(QStringLiteral("VK [YOU]"), View::Params(vk, Vk::Api::instance() -> userID()), 0, true, true);
    else openVKTabDialog();
}

void Playo::showVKRelTabDialog() {
    RelationsDialog dialog(Vk::Api::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
       Dockbars::obj().createDocBar(QStringLiteral("VK [") % dialog.getName() % QStringLiteral("]"), View::Params(vk_rel, dialog.getId(), user_rel), 0, true, true);

    emit Logger::obj().write(QStringLiteral("VkApi"), QStringLiteral("Open Relation"), Vk::Api::instance() -> getError());
}

void Playo::showSoundcloudRelTabDialog() {
    RelationsDialog dialog(Soundcloud::Api::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
        Dockbars::obj().createDocBar(QStringLiteral("SC [") % dialog.getName() % QStringLiteral("]"), View::Params(soundcloud, dialog.getId()), 0, true, true);
//    else QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
}

void Playo::openSoundcloudTabDialog() {
    WebDialogInterface * dInt;
    if (Plugins::loadWebDialog(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, Web::Manager::prepare(), Soundcloud::Api::instance() -> authUrl(), QStringLiteral("Soundcloud auth"));
        dInt -> registerActions(Soundcloud::Api::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::obj().createDocBar(QStringLiteral("SC [YOU]"), View::Params(soundcloud, Soundcloud::Api::instance() -> userID()), 0, true, true);

        emit Logger::obj().write(QStringLiteral("SoundcloudApi"), QStringLiteral("Connection"), Soundcloud::Api::instance() -> isConnected() ? QStringLiteral("true") : Soundcloud::Api::instance() -> getError());
        delete dInt;
    }
}

void Playo::showSoundcloudTabDialog() {
    if (Soundcloud::Api::instance() -> isConnected())
        Dockbars::obj().createDocBar(QStringLiteral("SC [YOU]"), View::Params(soundcloud, Soundcloud::Api::instance() -> userID()), 0, true, true);
    else openSoundcloudTabDialog();
}

//void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
//    if (to && !this -> isActiveWindow())
//        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
//}
