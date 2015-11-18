#include "toolbars.h"
#include "dockbars.h"
#include "external_keys.h"

#include <qdatetime.h>

using namespace Presentation;

QMenu * ToolBars::createPopupMenu() {
    QMenu * menu = new QMenu(container);

    lastClickPoint = QCursor::pos();
    QWidget * currentHover = container -> childAt(container -> mapFromGlobal(lastClickPoint));
    underMouseBar = deiterateToToolBar(currentHover);
    underMouseButton = qobject_cast<ToolbarButton *>(currentHover);
    bool isStatic = !qobject_cast<ToolBar *>(underMouseBar);

    menu -> addAction(QIcon(QStringLiteral(":panel_add")), QStringLiteral("Add panel"), this, SLOT(addPanelTriggered())/*, QKeySequence("Ctrl+P")*/);
    menu -> addAction(QIcon(QStringLiteral(":panel_remove")), QStringLiteral("Remove panel"), this, SLOT(removePanelTriggered())/*, QKeySequence("Ctrl+Shift+P")*/) -> setEnabled(!isStatic);
    menu -> addSeparator();

    if (underMouseBar) {
        if (underMouseBar -> isMovable()) {
            menu -> addAction(QIcon(QStringLiteral(":locked")), QStringLiteral("Lock bar"), this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(QStringLiteral(":locked")), QStringLiteral("Lock all bars"), this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+O")*/);
        } else {
            menu -> addAction(QIcon(QStringLiteral(":unlocked")), QStringLiteral("Unlock bar"), this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(QStringLiteral(":unlocked")), QStringLiteral("Unlock all bars"), this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+L")*/);
        }

        menu -> addSeparator();
    }

    menu -> addAction(QIcon(QStringLiteral(":drop_add")), QStringLiteral("Add drop point"), this, SLOT(addPanelButtonTriggered())/*, QKeySequence("Ctrl+F")*/) -> setEnabled(!isStatic && underMouseBar);
    menu -> addAction(QIcon(QStringLiteral(":drop_remove")), QStringLiteral("Remove drop point"), this, SLOT(removePanelButtonTriggered())/*, QKeySequence("Ctrl+Shift+F")*/) -> setEnabled(underMouseButton);

    QList<QToolBar *> bars = toolbars();
    QList<DockBar *> docs = Dockbars::obj().dockbars();
    bool hasBars = bars.count() > 0, hasDocs = docs.count() > 0;

    if (hasBars || hasDocs)
        menu -> addSeparator();

    if (hasBars) {
        QMenu * barsMenu = menu -> addMenu(QStringLiteral("Tool bars"));

        barsMenu -> addAction(QStringLiteral("Show all"), this, SLOT(showAll()));
        barsMenu -> addAction(QStringLiteral("Hide all"), this, SLOT(hideAll()));
        barsMenu -> addSeparator();

        for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
            if ((*bar) == spectrum)
                spectrum -> generateContextMenu(barsMenu);
            else
                barsMenu -> addAction((*bar) -> toggleViewAction());
        }

        connect(barsMenu, SIGNAL(hovered(QAction *)), this, SLOT(panelHighlight(QAction *)));
        connect(barsMenu, SIGNAL(aboutToHide()), this, SLOT(removePanelHighlight()));
    }

    if (hasDocs) {
        QMenu * docsMenu = menu -> addMenu(QStringLiteral("Dock bars"));

        docsMenu -> addAction(QStringLiteral("Show all"), &Dockbars::obj(), SLOT(showAll()));
        docsMenu -> addAction(QStringLiteral("Hide all"), &Dockbars::obj(), SLOT(hideAll()));
        docsMenu -> addSeparator();


        for(QList<DockBar *>::Iterator doc = docs.begin(); doc != docs.end(); doc++)
            docsMenu -> addAction((*doc) -> toggleViewAction());
    }

    return menu;
}

void ToolBars::load(const QJsonArray & bars) {
    if (bars.count() > 0) {
        QList<QString> barsList;
        barsList << toolbar_media_key << toolbar_media_plus_key << toolbar_media_pos_key << toolbar_media_time_key << toolbar_media_pan_key
            << toolbar_media_volume_key << toolbar_controls_key << toolbar_spectrum_key << toolbar_equalizer_key << toolbar_equalizer_button_key;

        QJsonObject obj, actionObj;
        QString barName;
        QToolBar * curr_bar;

        for(QJsonArray::ConstIterator bar = bars.constBegin(); bar != bars.constEnd(); bar++) {
            obj = (*bar).toObject();
            barName = obj.value(Key::title).toString();
            barsList.removeOne(barName);
            curr_bar = linkNameToToolbars(barName);
            curr_bar -> setObjectName(obj.value(Key::name).toString(curr_bar -> objectName()));
            curr_bar -> setMovable(obj.value(Key::movable).toBool());

            container -> addToolBar(Qt::BottomToolBarArea, curr_bar);

            if (obj.contains(Key::actions)) {
                QJsonArray actions = obj.value(Key::actions).toArray();

                foreach(QJsonValue act, actions) { // rewrite on for
                    actionObj = act.toObject();
                    addPanelButton(actionObj.value(Key::name).toString(), actionObj.value(Key::path).toString(), curr_bar);
                }
            }
        }

        while(barsList.length() > 0)
            container -> addToolBar(Qt::BottomToolBarArea, linkNameToToolbars(barsList.takeFirst()));
    }
    else createToolbars();
}

void ToolBars::save(DataStore * settings) {
    QList<QToolBar *> bars = toolbars();

    if (bars.length() > 0) {
        QJsonArray toolbar_array = QJsonArray();
        QJsonObject curr_tab;
        QList<QAction*> actions;
        ToolbarButton* button;

        for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
            curr_tab = QJsonObject();

            curr_tab.insert(Key::title, (*bar) -> windowTitle());
            curr_tab.insert(Key::name, (*bar) -> objectName());
            curr_tab.insert(Key::movable, (*bar) -> isMovable());

            if (!(*bar) -> property(toolbar_service_mark).toBool()) {
                actions = (*bar) -> actions();
                if (actions.length() > 0) {
                    QJsonArray action_array = QJsonArray();
                    QJsonObject curr_act;

                    for(QList<QAction*>::Iterator act = actions.begin(); act != actions.end(); act++) {
                        if ((*act) -> objectName() != QStringLiteral("*Title") && QString((*act) -> metaObject() -> className()) == QStringLiteral("QWidgetAction")) {
                            curr_act = QJsonObject();
                            button = (ToolbarButton*) (*bar) -> widgetForAction((*act));

                            curr_act.insert(Key::path, button -> mainPath());
                            curr_act.insert(Key::name, button -> text());
                            action_array.append(curr_act);
                        }
                    }

                    if (action_array.count() > 0)
                        curr_tab.insert(Key::actions, action_array);
                }
            }

            toolbar_array.append(curr_tab);

    //        bar -> toolButtonStyle();
        }

        settings -> write(ToolBars::settingsName(), toolbar_array);
    }
}

void ToolBars::createToolbars() {
  container -> addToolBar(Qt::TopToolBarArea, createMediaBar());
  container -> addToolBar(Qt::TopToolBarArea, createTimeMediaBar());
  container -> addToolBar(Qt::TopToolBarArea, createPositionMediaBar());
  container -> addToolBarBreak(Qt::TopToolBarArea);
  container -> addToolBar(Qt::TopToolBarArea, createAdditionalMediaBar());
  container -> addToolBar(Qt::TopToolBarArea, createVolumeMediaBar());
  container -> addToolBar(Qt::TopToolBarArea, createControlToolBar());
  container -> addToolBar(Qt::BottomToolBarArea, createToolBar(QStringLiteral("Folder linker 1")));
  container -> addToolBar(Qt::BottomToolBarArea, getSpectrum());
  container -> addToolBarBreak(Qt::BottomToolBarArea);

  QToolBar * eql = createEqualizerToolBar();
  container -> addToolBar(Qt::BottomToolBarArea, eql);
  eql -> hide();
}

void ToolBars::updateBarStyle(QToolBar * bar) {
    if (!bar) return;
    if (bar -> isMovable() || qobject_cast<ToolBar *>(bar) != 0)
        bar -> setStyleSheet(Stylesheets::toolbarMovableStyle());
    else
        bar -> setStyleSheet(Stylesheets::toolbarFixedStyle());
}

QToolBar * ToolBars::deiterateToToolBar(QWidget * obj) {
    QToolBar * ret;
    while(obj) {
        if ((ret = qobject_cast<QToolBar *>(obj)))
            return ret;
        else obj = obj -> parentWidget();
    }

    return 0;
}

QToolBar * ToolBars::linkNameToToolbars(const QString & barName) {
    if (barName == toolbar_media_key)                   return createMediaBar();
    else if (barName == toolbar_equalizer_button_key)   return createEqualizerButtonBar();
    else if (barName == toolbar_media_plus_key)         return createAdditionalMediaBar();
    else if (barName == toolbar_media_pos_key)          return createPositionMediaBar();
    else if (barName == toolbar_media_time_key)         return createTimeMediaBar();
    else if (barName == toolbar_media_volume_key)       return createVolumeMediaBar();
    else if (barName == toolbar_controls_key)           return createControlToolBar();
    else if (barName == toolbar_spectrum_key)           return getSpectrum();
    else if (barName == toolbar_equalizer_key)          return createEqualizerToolBar();
    else if (barName == toolbar_media_pan_key)          return createPanMediaBar();
    else                                                return createToolBar(barName);
}

QToolBar * ToolBars::createToolBar(const QString & name) {
    ToolBar * ptb = new ToolBar(name, container);

    ptb -> setFloatable(false);
    ptb -> setStyleSheet(Stylesheets::toolbarMovableStyle());

    ptb -> setMinimumSize(60, 60);
    ptb -> setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(ptb, SIGNAL(folderDropped(QString, QString)), this, SLOT(onFolderDrop(QString, QString)));
    connect(ptb, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
    connect(ptb, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));

    return ptb;
}

QToolBar * ToolBars::precreateToolBar(const QString & name, bool oriented) {
    QToolBar * ptb = new QToolBar(name, container);
    ptb -> setProperty(toolbar_service_mark, true);
    ptb -> setObjectName(QStringLiteral("_") % name);
    ptb -> setMinimumSize(30, 30);

    ptb -> setAttribute(Qt::WA_NoSystemBackground, true);
    ptb -> setAttribute(Qt::WA_TranslucentBackground, true);
    ptb -> setFloatable(false);

    if (oriented)
        connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(toolbarOrientationChanged(Qt::Orientation)));
    connect(ptb, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
    connect(ptb, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));

    return ptb;
}

QToolBar * ToolBars::createMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_key);   

    play_btn = ptb -> addAction(QIcon(QStringLiteral(":/play")), QStringLiteral("Play"));
    PlayerFactory::obj().registerCallback(in, play_btn, SIGNAL(triggered(bool)), SLOT(play()));

    pause_btn = ptb -> addAction(QIcon(QStringLiteral(":/pause")), QStringLiteral("Pause"));
    PlayerFactory::obj().registerCallback(in, pause_btn, SIGNAL(triggered(bool)), SLOT(pause()));

    stop_btn = ptb -> addAction(QIcon(QStringLiteral(":/stop")), QStringLiteral("Stop"));
    PlayerFactory::obj().registerCallback(in, stop_btn, SIGNAL(triggered(bool)), SLOT(stop()));

    QAction * act = ptb -> addAction(QIcon(QStringLiteral(":/cycling")), QStringLiteral("Looping current track"));
    act -> setCheckable(true);
    PlayerFactory::obj().registerCallback(in, act, SIGNAL(triggered(bool)), SLOT(loop(bool)));

    PlayerFactory::obj().registerCallback(out, this, SIGNAL(stateChanged(const PlayerState &)), SLOT(playerStateChanged(const PlayerState &)));

    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createAdditionalMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_plus_key);

    //TODO: add del versions od buttons
    ptb -> addAction(QIcon(QStringLiteral(":/prev")), QStringLiteral("Prev track"), &Dockbars::obj(), SLOT(playPrev()));

    QIcon ico;
    ico.addPixmap(QPixmap(QStringLiteral(":/like")), QIcon::Normal);
    ico.addPixmap(QPixmap(QStringLiteral(":/unlike")), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, QStringLiteral("Liked"));
    act -> setCheckable(true);
    connect(act, SIGNAL(triggered(bool)), &DataFactory::obj(), SLOT(changeLikeStatus(bool)));
    connect(&DataFactory::obj(), SIGNAL(likeChanged(bool)), act, SLOT(setChecked(bool)));

    ptb -> addAction(QIcon(QStringLiteral(":/next")), QStringLiteral("Next track"), &Dockbars::obj(), SLOT(playNext()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createPositionMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_pos_key, true);

    slider = new MetricSlider(ptb);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);
    slider -> setProperty("position", true);
    slider -> style() -> unpolish(slider);
    slider -> style() -> polish(slider);  
    slider -> setMinimum(0);
    slider -> setMaximum(0);

//    connect(Settings::obj.currPlayer(), SIGNAL(positionChanged(int)), slider, SLOT(setValueSilently(int)));
    PlayerFactory::obj().registerCallback(out, slider, SIGNAL(positionChanged(int)), SLOT(setValueSilently(int)));

//    connect(Settings::obj.currPlayer(), SIGNAL(durationChanged(int)), slider, SLOT(setMax(int)));
    PlayerFactory::obj().registerCallback(out, slider, SIGNAL(durationChanged(int)), SLOT(setMax(int)));

//    connect(slider, SIGNAL(valueChanged(int)), Settings::obj.currPlayer(), SLOT(position(uint)));
    PlayerFactory::obj().registerCallback(in, slider, SIGNAL(valueChanged(int)), SLOT(setPosition(int)));

    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createPanMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_pan_key, true);

    ClickableSlider * pslider = new ClickableSlider(ptb);
    pslider -> setOrientation(Qt::Horizontal);
    pslider -> setMinimumSize(30, 30);
    pslider -> setProperty("pan", true);
    pslider -> style() -> unpolish(pslider);
    pslider -> style() -> polish(pslider);

//    connect(pslider, SIGNAL(valueChanged(int)), Settings::obj.currPlayer(), SLOT(pan(int)));
    PlayerFactory::obj().registerCallback(in, pslider, SIGNAL(valueChanged(int)), SLOT(setPan(int)));

//    connect(Settings::obj.currPlayer(), SIGNAL(panChanged(int)), pslider, SLOT(setValueSilently(int)));
    PlayerFactory::obj().registerCallback(out, pslider, SIGNAL(panChanged(int)), SLOT(setValueSilently(int)));

    pslider -> setMinimum(-1000);
    pslider -> setMaximum(1000);
    pslider -> setValue(0);

    ptb -> addWidget(pslider);
    ptb -> adjustSize();

    return ptb;
}


QToolBar * ToolBars::createTimeMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_time_key);

    TimeLabel * timeLabel = new TimeLabel(QStringLiteral("After click invert showing time") , QString(), ptb);
    timeLabel -> setStyleSheet(QStringLiteral("QLabel { font-weight: bold; font-size: 12px; }"));
    ptb -> addWidget(timeLabel);


//    connect(Settings::obj.currPlayer(), SIGNAL(positionChanged(int)), slider, SLOT(setValueSilently(int)));
    PlayerFactory::obj().registerCallback(out, timeLabel, SIGNAL(positionChanged(int)), SLOT(setPos(int)));

//    connect(Settings::obj.currPlayer(), SIGNAL(durationChanged(int)), slider, SLOT(setMax(int)));
    PlayerFactory::obj().registerCallback(out, timeLabel, SIGNAL(durationChanged(int)), SLOT(setTotal(int)));

    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createVolumeMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_volume_key, true);

    QIcon ico;
    ico.addPixmap(QPixmap(QStringLiteral(":/mute")), QIcon::Normal);
    ico.addPixmap(QPixmap(QStringLiteral(":/volume")), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, QStringLiteral("Mute"));
    act -> setCheckable(true);
    PlayerFactory::obj().registerCallback(in, act, SIGNAL(triggered(bool)), SLOT(mute(bool)));

    ClickableSlider * slider = new ClickableSlider(ptb);
    slider -> setProperty("volume", true);
    slider -> style() -> unpolish(slider);
    slider -> style() -> polish(slider);
    slider -> setTickInterval(2000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);
    slider -> setMaximum(10000);
    slider -> setValue(10000);

//    PlayerFactory::obj().registerCallback(out, pslider, SIGNAL(panChanged(int)), SLOT(setValueSilently(int)));

//    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(setChannelVolume(int)));
    PlayerFactory::obj().registerCallback(in, slider, SIGNAL(valueChanged(int)), SLOT(setVolume(int)));

//    connect(this, SIGNAL(volumeChanged(int)), this, SLOT(setVolTrackbarValue(int)));
    PlayerFactory::obj().registerCallback(out, slider, SIGNAL(volumeChanged(int)), SLOT(setValueSilently(int)));

    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createControlToolBar() {
    QToolBar * ptb = precreateToolBar(toolbar_controls_key);

    ptb -> addAction(QIcon(QStringLiteral(":/add")), QStringLiteral("Add new local tab"), &Dockbars::obj(), SLOT(createNewBar()));
    ptb -> addWidget(initiateVkButton());
    ptb -> addWidget(initiateSoundcloudButton());
    ptb -> addWidget(initiateOdButton());
    ptb -> addSeparator();
    ptb -> addWidget(initiateEchonestButton());
    ptb -> addSeparator();
    ptb -> addAction(QIcon(QStringLiteral(":/search")), QStringLiteral("Search"), container, SLOT(showSearchDialog()));
    ptb -> addSeparator();
    ptb -> addAction(QIcon(QStringLiteral(":/settings")), QStringLiteral("Common setting"), container, SLOT(showSettingsDialog()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createEqualizerToolBar() {
    QToolBar * ptb = precreateToolBar(toolbar_equalizer_key);

    equalizer = new EqualizerView(ptb);
    ptb -> addWidget(equalizer);
    ptb -> adjustSize();

    ptb -> setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    return ptb;
}

QToolBar * ToolBars::createEqualizerButtonBar() {
    QToolBar * ptb = precreateToolBar(toolbar_equalizer_button_key);

    QToolBar * bar = (QToolBar *)equalizer -> parent();
    QAction * act = bar -> toggleViewAction();
    act -> setIcon(QIcon(QStringLiteral(":/equalizer")));
    act -> setCheckable(true);
    act -> setChecked(bar -> isVisible());
    ptb -> addAction(act);
    return ptb;
}

SpectrumView * ToolBars::getSpectrum() {
    if (spectrum == 0) {
        spectrum = new SpectrumView(toolbar_spectrum_key, container);
        spectrum -> setProperty(toolbar_service_mark, true);

        connect(spectrum, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
        connect(spectrum, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));
    }

    return spectrum;
}

void ToolBars::disconnectVk() {
    Vk::Api::obj().disconnect();
    initiateVkButton();
}
void ToolBars::disconnectSoundcloud() {
    Soundcloud::Api::obj().disconnect();
    initiateSoundcloudButton();
}
void ToolBars::disconnectOd() {
    Od::Api::obj().disconnect();
    initiateOdButton();
}

QToolButton * ToolBars::initiateEchonestButton() {
    QToolButton * echoToolButton = new QToolButton(container);

    echoToolButton -> setIcon(QIcon(QStringLiteral(":/echonest")));
    connect(echoToolButton, SIGNAL(clicked()), container, SLOT(showEchonestDialog()));
    echoToolButton -> setToolTip(QStringLiteral("Echonest(the.echonest.com)"));

    return echoToolButton;
}

// move to the vk class
QToolButton * ToolBars::initiateVkButton() {
    if (vkToolButton == 0)
        vkToolButton = new QToolButton(container);
    else {
        vkToolButton -> setMenu(0);
        disconnect(vkToolButton, SIGNAL(clicked()), container, SLOT(openVKTabDialog()));
    }

    if (Vk::Api::obj().isConnected()) {
        vkToolButton -> setIcon(QIcon(QStringLiteral(":/add_vk_on")));
        vkToolButton -> setPopupMode(QToolButton::InstantPopup);
        vkToolButton -> setToolTip(QStringLiteral("VKontakte(vk.com)"));

        QMenu * vkMenu = new QMenu(vkToolButton);
        vkMenu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectVk()));
        vkMenu -> addAction(QStringLiteral("Open your tab"), container, SLOT(openVKTabDialog()));
        vkMenu -> addAction(QStringLiteral("Open friend/group tab"), container, SLOT(showVKRelTabDialog()));
        vkMenu -> addAction(QStringLiteral("Open recommendations"), container, SLOT(openVKRecomendations()));
        vkToolButton -> setMenu(vkMenu);
    } else {
        vkToolButton -> setIcon(QIcon(QStringLiteral(":/add_vk")));
        vkToolButton -> setToolTip(QStringLiteral("Connect to VKontakte(vk.com)"));
        connect(vkToolButton, SIGNAL(clicked()), container, SLOT(openVKTabDialog()));
    }

    return vkToolButton;
}

// move to the cloudsound class
QToolButton * ToolBars::initiateSoundcloudButton() {
    if (soundcloudToolButton == 0)
        soundcloudToolButton = new QToolButton(container);
    else {
        soundcloudToolButton -> setMenu(0);
        disconnect(soundcloudToolButton, SIGNAL(clicked()), container, SLOT(openSoundcloudTabDialog()));
    }

    if (Soundcloud::Api::obj().isConnected()) {
        soundcloudToolButton -> setIcon(QIcon(QStringLiteral(":/add_soundcloud_on")));
        soundcloudToolButton -> setToolTip(QStringLiteral("Soundcloud(soundcloud.com)"));
        soundcloudToolButton -> setPopupMode(QToolButton::InstantPopup);

        QMenu * vkMenu = new QMenu(soundcloudToolButton);
        vkMenu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectSoundcloud()));
        vkMenu -> addAction(QStringLiteral("Open your tab"), container, SLOT(openSoundcloudTabDialog()));
        vkMenu -> addAction(QStringLiteral("Open friend/group tab"), container, SLOT(showSoundcloudRelTabDialog()));
        soundcloudToolButton -> setMenu(vkMenu);
    } else {
        soundcloudToolButton -> setIcon(QIcon(QStringLiteral(":/add_soundcloud")));
        soundcloudToolButton -> setToolTip(QStringLiteral("Connect to Soundcloud(soundcloud.com)"));
        connect(soundcloudToolButton, SIGNAL(clicked()), container, SLOT(openSoundcloudTabDialog()));
    }

    return soundcloudToolButton;
}

QToolButton * ToolBars::initiateOdButton() {
    if (odToolButton == 0)
        odToolButton = new QToolButton(container);
    else {
        odToolButton -> setMenu(0);
        disconnect(odToolButton, SIGNAL(clicked()), container, SLOT(openOdTabDialog()));
    }

    if (Od::Api::obj().isConnected()) {
        odToolButton -> setIcon(QIcon(QStringLiteral(":/add_od_on")));
        odToolButton -> setToolTip(QStringLiteral("Od(ok.ru)"));
        odToolButton -> setPopupMode(QToolButton::InstantPopup);

        QMenu * odMenu = new QMenu(odToolButton);
        odMenu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectOd()));
//        vkMenu -> addAction(QStringLiteral("Reconect"), container, SLOT(openSoundcloudTabDialog()));
        odMenu -> addAction(QStringLiteral("Open your tab"), container, SLOT(openOdTabDialog()));
//        vkMenu -> addAction(QStringLiteral("Open friend/group tab"), container, SLOT(showSoundcloudRelTabDialog()));
        odToolButton -> setMenu(odMenu);
    } else {
        odToolButton -> setIcon(QIcon(QStringLiteral(":/add_od")));
        odToolButton -> setToolTip(QStringLiteral("Connect to Od(ok.ru)"));
        connect(odToolButton, SIGNAL(clicked()), container, SLOT(openOdTabDialog()));
    }

    return odToolButton;
}

void ToolBars::addPanelButton(const QString & name, const QString & path, QToolBar * bar) {
    ToolbarButton * button = new ToolbarButton(name, path);
    connect(button, SIGNAL(clicked()), container, SLOT(openFolderTriggered()));
    bar -> addWidget(button);
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void ToolBars::panelHighlight(QAction * action) {
    if (highlighted != 0)
        emit removePanelHighlight();

    if (!(highlighted = qobject_cast<QToolBar *>(action -> parentWidget()))) {
        QMenu * m = qobject_cast<QMenu *>(action -> parentWidget());
        if (m)
            highlighted = qobject_cast<QToolBar *>(m -> actions().first() -> parentWidget());
    }

    if (highlighted)
        highlighted -> setStyleSheet(Stylesheets::toolbarHighLightStyle());
}

void ToolBars::removePanelHighlight() {
    if (highlighted != 0) {
        updateBarStyle(highlighted);
        highlighted = 0;
    }
}

void ToolBars::addPanelTriggered() {
    UserActionDialog dialog(container);
    dialog.buildToolbarForm();

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getValue(dialog.name_key);
        QToolBar * bar = createToolBar(name);
        bar -> setObjectName(name + QString::number(QDateTime::currentMSecsSinceEpoch()));
        Qt::ToolBarArea area = underMouseBar ? container -> toolBarArea(underMouseBar) : Qt::BottomToolBarArea;
        container -> addToolBar(area, bar);
    }
}

void ToolBars::removePanelTriggered() {
    container -> removeToolBar(underMouseBar);
    delete underMouseBar; //TODO: maybe use deleteLater() ?
    underMouseBar = 0;
}

void ToolBars::addPanelButtonTriggered() {
    UserActionDialog dialog(container);
    dialog.buildToolbarButtonForm();

    if (dialog.exec() == QDialog::Accepted) {
        qDebug() << "PIDO" << dialog.getValue(dialog.name_key) << dialog.getValue(dialog.path_key);
        addPanelButton(dialog.getValue(dialog.name_key), dialog.getValue(dialog.path_key), underMouseBar);
    }
}

void ToolBars::removePanelButtonTriggered() {
    QToolBar * bar = (QToolBar *)underMouseButton -> parentWidget();
    bar -> removeAction(bar -> actionAt(bar -> mapFromGlobal(lastClickPoint)));
}

void ToolBars::toolbarOrientationChanged(Qt::Orientation orientation) {
    QToolBar * bar = (QToolBar *)QObject::sender();

    QList<QSlider *> orientables = bar -> findChildren<QSlider *>();

    for(QList<QSlider *>::Iterator slider = orientables.begin(); slider != orientables.end(); slider++) {
        (*slider) -> setOrientation(orientation);
//        (*orientable) -> setInvertedAppearance(orientation == Qt::Vertical);
    }
}

void ToolBars::hideAll() {
    QList<QToolBar *> bars = toolbars();
    for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++)
        (*bar) -> setHidden(true);
}
void ToolBars::showAll() {
    QList<QToolBar *> bars = toolbars();
    for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++)
        (*bar) -> setHidden(false);
}

void ToolBars::changeToolbarMovable() {
    if (!underMouseBar) return;
    underMouseBar -> setMovable(!underMouseBar -> isMovable());
}

void ToolBars::changeToolbarsMovable() {
    if (!underMouseBar) return;
    bool movable = !underMouseBar -> isMovable();
    QList<QToolBar *> bars = toolbars();

    for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++)
        if (movable || (!movable && !(*bar) -> isFloating()))
            (*bar) -> setMovable(movable);
}

void ToolBars::playerStateChanged(const PlayerState & state) {
    play_btn -> setVisible(state != PlayingState);
    pause_btn -> setVisible(state == PlayingState);
    stop_btn -> setVisible(state == PlayingState || state == PausedState);
}

void ToolBars::onFolderDrop(const QString & name, const QString & path) {
    addPanelButton(name, path, (QToolBar *)QObject::sender());
}
