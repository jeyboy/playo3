#include "playo.h"
#include "ui_playo.h"

#include "misc/stylesheets.h"

using namespace Playo3;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);
//    ui-> centralWidget -> hide();
    QApplication::setWindowIcon(QIcon(QStringLiteral(":ico")));
    setWindowTitle(QStringLiteral("Playo"));
    setAcceptDrops(true);

    setTabShape(QTabWidget::Rounded);

    //    setAttribute(Qt::WA_DeleteOnClose);

    initialization();


    UserActionDialog * actionDialog = new UserActionDialog(this);
    QList<FormInput> inputs;
    inputs << FormInput(QStringLiteral("code"), QStringLiteral("Code from sms"));
    actionDialog -> buildForm(inputs);

    QString resendLink = QStringLiteral("http://ok.ru/dk?cmd=AnonymAccountRecovery2&amp;st.cmd=anonymAccountRecoveryNew&amp;st.recStep=PhoneCaptcha&amp;st.uid=511343312018&amp;st.ust=Active&amp;st.sms=on&amp;st._aid=AnonymPhoneCheck_ResendSms");
    QList<FormInput> extra_inputs;
    extra_inputs << FormInput(QStringLiteral("Resend sms"), resendLink, WebManager::manager(), SLOT(sendGet(QString&)));
    actionDialog -> extendForm(extra_inputs);

    if (actionDialog -> exec() == QDialog::Accepted) {
        QHash<QString, QString> attrs;
        attrs.insert("st.mobileCaptcha", actionDialog -> getValue(QStringLiteral("code")));
        qDebug() << "PATH" << attrs;
    }

    actionDialog -> buildCaptchaForm(WebManager::manager() -> openImage(QUrl("http://c22blog.files.wordpress.com/2010/10/input-black.gif")));
    if (actionDialog -> exec() == QDialog::Accepted) {
        QHash<QString, QString> attrs;
        attrs.insert("st.mobileCaptcha", actionDialog -> getValue(QStringLiteral("captcha")));
        qDebug() << "PATH" << attrs;
    }




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

//        qDebug() << cue.songs();

//        f.close();
//    }

//    QString s = QString(
//        "<html class='уеба' xmlns:fb='http://ogp.me/ns/fb#'>"
//        "<head coco lal='1' pido>"
//        "<title>лалка DESKTOP SABEResPODER</title>"
//        "<!-- Yandex.Metrika counter -->"
//        "<meta charset='UTF-8'></meta>"
//        "<meta content='' name='description'>"
//        "<meta content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0, minimal-ui' name='viewport'>"
//        "<link href='http://fonts.googleapis.com/css?family=Ropa+Sans' rel='stylesheet' type='text/css'>"
//        "<link rel='stylesheet' href='css/ie8.css'>"
//        "<!--"
//        "function displayMsg() {"
//        "    alert('Hello World!')"
//        "}"
//        "//-->"
//        "<script type='text/javascript'>"
//        "   var _gaq = _gaq || [];"
//        "   _gaq.push(['_setAccount','UA-42547686-1']);"
//        "   _gaq.push(['_setDomainName','auto']);"
//        "   _gaq.push(['_trackPageview']);"
//        "   (function() {"
//        "   var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;"
//        "   ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';"
//        "   var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);"
//        "   })();"
//        "</script>"
//        "</head>"
//        "<body class='home-show-page'>"
//        "    <ul class='caps title-area'>"
//        "        <li class='name'>ds fsd</li>"
//        "    </ul>"
//        "</body>"
//        "</html>"
//    );


//    QElapsedTimer t2;
//    t2.start();
//    HtmlParser p(s);
//    qDebug() << "PARSE HTML" << t2.nsecsElapsed() << t2.elapsed() << "ms";
//    p.output();
//    HtmlSet set = p.find(QStringLiteral("html meta[name='description']"));
//    qDebug() << set;

//    QNetworkReply * response = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(QUrl(QStringLiteral("https://myzuka.org/"))));

//    QFile f("F:/test_page/bug.htm");
//    if (f.open(QFile::ReadOnly)) {
//        QElapsedTimer t2;
//        t2.start();
//        Html::Document p(&f);
//        p.output();

//        QHash<QString, QString> hash;
//        hash.insert("st.mobileCaptcha", "hudo");
//        qDebug() << "PARSE HTML" << t2.nsecsElapsed() << p.find("form").first() -> toFormSubmit(hash);
//        f.close();
//    }
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

        Web::Apis::close();
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

    QSettings stateSettings(FRONT_SETTINGS_FILE_NAME, QSettings::IniFormat, this);
    settings = new DataStore(BACKEND_SETTINGS_FILE_NAME);

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
    Vk::Api::instance(this, settings -> read(SETTINGS_VK_SET_KEY).toObject());
    Soundcloud::Api::instance(settings -> read(SETTINGS_SOUNDCLOUD_SET_KEY).toObject());
    Fourshared::Api::instance(settings -> read(SETTINGS_FOURSHARED_SET_KEY).toObject());

    Settings::instance() -> fromJson(settings -> read(SETTINGS_SET_KEY).toObject());

    activation();

    SettingsDialog::registerHotkeys(Dockbars::instance());

    setTabPosition((QTabWidget::TabPosition)Settings::instance() -> tabPosition());

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

    QJsonArray bars = settings -> read(ToolBars::settingsName()).toArray();
    ToolBars::instance() -> load(bars);
    ToolBars::instance() -> setEqualizerSettings(settings -> read(SETTINGS_EQUALIZER_SET_KEY).toObject());

    QJsonArray docks = settings -> read(Dockbars::settingsName()).toArray();
    Dockbars::instance() -> load(docks);

    QVariant objState = stateSettings.value(SETTINGS_WINDOW_STATE_SET_KEY);
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
//    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));
    Dockbars::instance() -> updateActiveTabIcon();

    if (stateSettings.value(SETTINGS_WINDOW_MAXIMIZED_KEY).toBool()) {
        QApplication::processEvents();
        showMaximized();
    }
    Logger::instance() -> endMark(tr("Main"), tr("Loading"));
}

QMenu * Playo::createPopupMenu() {
    return ToolBars::instance() -> createPopupMenu(this);
}

void Playo::closeEvent(QCloseEvent * e) {
    Logger::instance() -> unregisterEditor();
    bool is_maximized = isMaximized();
    showNormal();

    QSettings stateSettings(FRONT_SETTINGS_FILE_NAME, QSettings::IniFormat, this);
    stateSettings.setValue(SETTINGS_GEOMETRY_SET_KEY, saveGeometry());
    stateSettings.setValue(SETTINGS_WINDOW_STATE_SET_KEY, saveState());
    stateSettings.setValue(SETTINGS_WINDOW_MAXIMIZED_KEY, is_maximized);
    stateSettings.sync();

    setWindowState(Qt::WindowMinimized); // hiding window while savings going

    Logger::instance() -> startMark();

    Player::instance() -> pause();

    settings -> clear();

    settings -> write(SETTINGS_VK_SET_KEY, Vk::Api::instance() -> toJson());
    settings -> write(SETTINGS_SOUNDCLOUD_SET_KEY, Soundcloud::Api::instance() -> toJson());
    settings -> write(SETTINGS_FOURSHARED_SET_KEY, Fourshared::Api::instance() -> toJson());

    settings -> write(SETTINGS_EQUALIZER_SET_KEY, ToolBars::instance() -> getEqualizerSettings());
    ToolBars::instance() -> save(settings);
    Dockbars::instance() -> save(settings);

    settings -> write(SETTINGS_SET_KEY, Settings::instance() -> toJson());
    settings -> save();

    Logger::instance() -> endMark(QStringLiteral("Main"), QStringLiteral("Saving"));

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
    Logger::instance() -> write(QStringLiteral("Main"), QStringLiteral("receiveMessage"));
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
    ViewSettings settings(vk_rel, false, false, false, true, Vk::Api::instance() -> userID(), user_rel);
    Dockbars::instance() -> createDocBar(QStringLiteral("Rec for YOU"), settings, 0, true, true);
}

void Playo::openVKTabDialog() {
    WebDialogInterface * dInt;
    if (Plugins::loadWebDialog(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, WebManager::manager(), Vk::Api::instance() -> authUrl(), QStringLiteral("VK auth"));
        dInt -> registerActions(Vk::Api::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::instance() -> createDocBar(QStringLiteral("VK [YOU]"), ViewSettings::vk(Vk::Api::instance() -> userID()), 0, true, true);

        Logger::instance() -> writeToStream(QStringLiteral("VkApi"), QStringLiteral("Connection"), Vk::Api::instance() -> isConnected() ? QStringLiteral("true") : Vk::Api::instance() -> getError());
        delete dInt;
    }
//    else QMessageBox::information(this, "VK", VkApi::instance() -> getError());
}

void Playo::showVKTabDialog() {
    if (Vk::Api::instance() -> isConnected())
        Dockbars::instance() -> createDocBar(QStringLiteral("VK [YOU]"), ViewSettings::vk(Vk::Api::instance() -> userID()), 0, true, true);
    else openVKTabDialog();
}

void Playo::showVKRelTabDialog() {
    RelationsDialog dialog(Vk::Api::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
       Dockbars::instance() -> createDocBar(QStringLiteral("VK [") % dialog.getName() % QStringLiteral("]"), ViewSettings::vk(dialog.getId()), 0, true, true);

    Logger::instance() -> writeToStream(QStringLiteral("VkApi"), QStringLiteral("Open Relation"), Vk::Api::instance() -> getError());
}

void Playo::showSoundcloudRelTabDialog() {
    RelationsDialog dialog(Soundcloud::Api::instance(), this);
    if (dialog.exec() == QDialog::Accepted)
        Dockbars::instance() -> createDocBar(QStringLiteral("SC [") % dialog.getName() % QStringLiteral("]"), ViewSettings::soundcloud(dialog.getId()), 0, true, true);
//    else QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
}

void Playo::openSoundcloudTabDialog() {
    WebDialogInterface * dInt;
    if (Plugins::loadWebDialog(dInt)) {
        QDialog * dialog = dInt -> createDialog(this, WebManager::manager(), Soundcloud::Api::instance() -> authUrl(), QStringLiteral("Soundcloud auth"));
        dInt -> registerActions(Soundcloud::Api::instance());

        if (dialog -> exec() == QDialog::Accepted)
            Dockbars::instance() -> createDocBar(QStringLiteral("SC [YOU]"), ViewSettings::soundcloud(Soundcloud::Api::instance() -> userID()), 0, true, true);

        Logger::instance() -> writeToStream(QStringLiteral("SoundcloudApi"), QStringLiteral("Connection"), Soundcloud::Api::instance() -> isConnected() ? QStringLiteral("true") : Soundcloud::Api::instance() -> getError());
        delete dInt;
    }
}

void Playo::showSoundcloudTabDialog() {
    if (Soundcloud::Api::instance() -> isConnected())
        Dockbars::instance() -> createDocBar(QStringLiteral("SC [YOU]"), ViewSettings::soundcloud(Soundcloud::Api::instance() -> userID()), 0, true, true);
    else openSoundcloudTabDialog();
}

//void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
//    if (to && !this -> isActiveWindow())
//        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
//}
