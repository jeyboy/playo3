#include "playo.h"
#include "ui_playo.h"

using namespace Presentation;
using namespace Dialogs;
using namespace Data;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    setCentralWidget(new QWidget);
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
    Settings::obj().anchorWidget(this);
    new Tray(this);
    UserDialogBox::obj(); // link dialog with current thread
    ToolBars::obj().setContainer(this);
    Dockbars::obj().setContainer(this);
    PlayerFactory::obj().build(this, bass_player); // initiate default player for correct settings init and providing
}

void Playo::initialization() {
    Logger::obj().startMark();

    QSettings stateSettings(FRONT_SETTINGS_FILE_NAME, QSettings::IniFormat, this);
    settings = new DataStore(BACKEND_SETTINGS_FILE_NAME);

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
    Settings::obj().fromJson(settings -> read(SETTINGS_SET_KEY).toObject());
    activation();
    Web::Apis::initiate(settings -> obj());

    SettingsDialog::registerHotkeys(&Dockbars::obj());
    Settings::setCurrentStyle(DarkStylesheets());

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

    DataFactory::obj().currPlayer() -> pause();

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
        Views::Params settings(Settings::obj().openDropPointInTabType(), false, false, false, true);
        Dockbars::obj().createLinkedDocBar(button -> text(), button -> mainPath(), settings, 0, true, true);
    }
    else QDesktopServices::openUrl(QUrl::fromLocalFile(button -> mainPath()));
}

void Playo::showSearchDialog() {    
    SearchDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Views::Params settings(search, false, false, false, true);
        SearchSettings prms = dialog.params();
        Dockbars::obj().createDocBar(QStringLiteral("Search"), settings, 0, true, true, &prms);
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
        DataFactory::obj().currPlayer() -> spectrumFreq(Settings::obj().spectrumFreqRate());
        setTabPosition((QTabWidget::TabPosition)Settings::obj().tabPosition());
        Dockbars::obj().updateAllViews();
    }
}

void Playo::showEchonestDialog() {
    EchonestDialog(this).exec();
}

void Playo::openVKRecomendations() {
    Dockbars::obj().createDocBar(
        QStringLiteral("Rec for YOU"),
        Views::Params(vk_rel, Vk::Api::obj().userID(), user_rel), 0, true, true
    );
}

void Playo::openSoundcloudTabDialog() {
    if (Soundcloud::Api::obj().connection())
        Dockbars::obj().createLinkedDocBar(
            QStringLiteral("SC [YOU]"), Soundcloud::Api::obj().uidStr(Soundcloud::Api::obj().userID()),
            Views::Params(soundcloud, Soundcloud::Api::obj().userID()), 0, true, true
        );
}

void Playo::openOdTabDialog() {
    if (Od::Api::obj().connection())
        Dockbars::obj().createLinkedDocBar(
            QStringLiteral("OD [YOU]"), Od::Api::obj().uidStr(Od::Api::obj().userID()),
            Views::Params(od, Od::Api::obj().userID()), 0, true, true
        );
}

void Playo::openVKTabDialog() {
    if (Vk::Api::obj().connection())
        Dockbars::obj().createLinkedDocBar(
            QStringLiteral("VK [YOU]"), Vk::Api::obj().uidStr(Vk::Api::obj().userID()),
            Views::Params(vk, Vk::Api::obj().userID()), 0, true, true
        );
}

void Playo::showVKRelTabDialog() {
    RelationsDialog dialog(&Vk::Api::obj(), this);
    if (dialog.exec() == QDialog::Accepted)
        Dockbars::obj().createLinkedDocBar(
            QStringLiteral("VK [") % dialog.getName() % QStringLiteral("]"),
            Vk::Api::obj().uidStr(dialog.getId()),
            Views::Params(vk_rel, dialog.getId()), 0, true, true
        );

    Logger::obj().write(QStringLiteral("VkApi"), QStringLiteral("Open Relation"), Vk::Api::obj().lastError());
}

void Playo::showSoundcloudRelTabDialog() {
    RelationsDialog dialog(&Soundcloud::Api::obj(), this);
    if (dialog.exec() == QDialog::Accepted)
        Dockbars::obj().createLinkedDocBar(
            QStringLiteral("SC [") % dialog.getName() % QStringLiteral("]"),
            Od::Api::obj().uidStr(dialog.getId()),
            Views::Params(soundcloud, dialog.getId()), 0, true, true
        );
}

//void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
//    if (to && !this -> isActiveWindow())
//        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
//}
