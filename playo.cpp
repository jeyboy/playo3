#include "playo.h"
#include "ui_playo.h"

//#include "modules/core/misc/file_utils/filesystem_watcher.h"

using namespace Presentation;
using namespace Dialogs;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    setCentralWidget(new QWidget);
//    ui-> centralWidget -> hide();
    QApplication::setWindowIcon(QIcon(LSTR(":ico")));
    setWindowTitle(LSTR("Playo"));
    setAcceptDrops(true);

    setTabShape(QTabWidget::Rounded);

    //    setAttribute(Qt::WA_DeleteOnClose);
    initialization();

//    FileSystemWatcher::obj().registerPath(QStringLiteral("/home/jb/Desktop"), true);
//    connect(&FileSystemWatcher::obj(), SIGNAL(fileCreated(QString)), this, SLOT(messa(QString)));

//    QString s = QStringLiteral("► Paradise Lost - Ordinary Days \r\n       111");
//    qDebug() << Core::FilenameConversions::extraSymbolsFilter(s);


//    qDebug() << FuzzyComparison::compareStrings(QString("Michael George - One More Try (Live in Paris, 1988)"), QString("George Michael - One More Try"))
//        << FuzzyComparison::compareStrings2(QString("Michael George - One More Try (Live in Paris, 1988)"), QString("George Michael - One More Try"));

//    qDebug() << FuzzyComparison::compareStrings(QString("GeorgeMichael - One More Try"), QString("George Michael - One More Try"))
//        << FuzzyComparison::compareStrings2(QString("GeorgeMichael - One More Try"), QString("George Michael - One More Try"));

    //safe usage of pointers
//    QPointer dlg = new SomeDialog( this );
}

Playo::~Playo() {
    delete ui;
    delete settings;
}

void Playo::activation() {
    Settings::obj().anchorWidget(this);
    tray = new Tray(this);
    UserDialogBox::obj(); // link dialog with current thread
    ToolBars::obj().setContainer(this);
    Dockbars::obj().setContainer(this);
    PlayerFactory::obj().build(this, bass_player); // initiate default player for correct settings initialization
    connect(&DataFactory::obj(), SIGNAL(playedItemChanged(QString,QString)), this, SLOT(playedItemChanged(QString,QString)));
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
    Settings::setCurrentStyle((IStylesheets::StyleType)Settings::obj().colorScheme());

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
    Library::obj().declineStateRestoring();

    settings -> clear();

    settings -> write(SETTINGS_EQUALIZER_SET_KEY, ToolBars::obj().getEqualizerSettings());
    ToolBars::obj().save(settings);
    Dockbars::obj().save(settings);

    settings -> write(SETTINGS_SET_KEY, Settings::obj().toJson());
    Web::Apis::close(settings -> obj());
    settings -> save();

    HotkeyManager::obj().clear();
    FileSystemWatcher::obj().clear();

    Logger::obj().endMark(LSTR("Main"), LSTR("Saving"));
    MainWindow::closeEvent(e);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Playo::putToCommonTab(const QList<QUrl> & urls) {
    DockBar * bar = Dockbars::obj().commonBar();
    Dockbars::obj().view(bar) -> appendRows(urls);
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

void Playo::receiveMessage(const QString & message) {
    Logger::obj().write(LSTR("Main"), LSTR("receiveMessage"));
    QStringList list = message.split('|', QString::SkipEmptyParts);
    QList<QUrl> urls;

    for(QStringList::iterator it = list.begin(); it != list.end(); it++)
        urls.append(QUrl::fromLocalFile((*it)));

    putToCommonTab(urls);
}

//void MainWindow::showError(QString message) {
//    qDebug() << message;
//}

void Playo::openFolderTriggered() {
    ToolbarUserButton * button = (ToolbarUserButton *)QObject::sender();
    if (!(button -> keyboardModifiers() & Qt::ControlModifier) && Settings::obj().isOpenDropPointInTab()) {
        Models::Params settings(Settings::obj().openDropPointInTabType(), mpf_auto_play_next);
        Dockbars::obj().createLinkedDocBar(BarCreationNames(button -> text(), button -> mainPath()), settings, 0, true, true);
    }
    else QDesktopServices::openUrl(QUrl::fromLocalFile(button -> mainPath()));
}

void Playo::showSearchDialog() {    
    SearchDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Models::Params settings(dt_search, mpf_auto_play_next);
        SearchLimitLayers prms = dialog.params();
        Dockbars::obj().createDocBar(LSTR("Search"), settings, 0, true, true, &prms);
    }
}

void Playo::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ToolBars::obj().updateMetricSliders();
        ToolBars::obj().setIconsSize(Settings::obj().toolIconSize());
        ToolBars::obj().getSpectrum() -> updateSettings();
        DataFactory::obj().currPlayer() -> spectrumFreq(Settings::obj().spectrumFreqRate());
        setTabPosition((QTabWidget::TabPosition)Settings::obj().tabPosition());
        Dockbars::obj().updateAllViews();
    }
}

void Playo::playedItemChanged(const QString & prev, const QString & next) {
    // "(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ")

    if (!next.isEmpty()) {
        int show_tray_flag = Settings::obj().showPlayedInTrayMessage();
        bool show = show_tray_flag == Settings::tsi_show || (show_tray_flag == Settings::tsi_inform && !isActiveWindow());

        if (show)
            tray -> showMessage(
                QString(),
                prev.isEmpty() ? QString() : LSTR("Close: ") % prev % '\n' %
                LSTR("Start Playing: ") % next,
                QSystemTrayIcon::Information,
                Settings::obj().showTrayMessageTime()
            );
    }
}
