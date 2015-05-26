#include "toolbars.h"
#include "dockbars.h"
#include <qdatetime.h>

using namespace Playo3;

ToolBars * ToolBars::self = 0;

ToolBars * ToolBars::instance(QObject * parent) {
    if(!self)
        self = new ToolBars(parent);
    return self;
}

QMenu * ToolBars::createPopupMenu(QMainWindow * window) {
    QMenu * menu = new QMenu(window);

    lastClickPoint = QCursor::pos();
    QWidget * currentHover = window -> childAt(window -> mapFromGlobal(lastClickPoint));
    underMouseBar = deiterateToToolBar(currentHover);
    underMouseButton = qobject_cast<ToolbarButton *>(currentHover);
    bool isStatic = !qobject_cast<ToolBar *>(underMouseBar);

    menu -> addAction(QIcon(":panel_add"), "Add panel", this, SLOT(addPanelTriggered())/*, QKeySequence("Ctrl+P")*/);
    menu -> addAction(QIcon(":panel_remove"), "Remove panel", this, SLOT(removePanelTriggered())/*, QKeySequence("Ctrl+Shift+P")*/) -> setEnabled(!isStatic);
    menu -> addSeparator();

    if (underMouseBar) {
        if (underMouseBar -> isMovable()) {
            menu -> addAction(QIcon(":locked"), "Lock bar", this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(":locked"), "Lock all bars", this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+O")*/);
        } else {
            menu -> addAction(QIcon(":unlocked"), "Unlock bar", this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(":unlocked"), "Unlock all bars", this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+L")*/);
        }

        menu -> addSeparator();
    }

    menu -> addAction(QIcon(":drop_add"), "Add drop point", this, SLOT(addPanelButtonTriggered())/*, QKeySequence("Ctrl+F")*/) -> setEnabled(!isStatic && underMouseBar);
    menu -> addAction(QIcon(":drop_remove"), "Remove drop point", this, SLOT(removePanelButtonTriggered())/*, QKeySequence("Ctrl+Shift+F")*/) -> setEnabled(underMouseButton);

    QList<QToolBar *> bars = toolbars();
    QList<DockBar *> docs = Dockbars::instance(window) -> dockbars();
    bool hasBars = bars.count() > 0, hasDocs = docs.count() > 0;

    if (hasBars || hasDocs)
        menu -> addSeparator();

    if (hasBars) {
        QMenu * barsMenu = menu -> addMenu("Tool bars");

        barsMenu -> addAction("Show all", this, SLOT(showAll()));
        barsMenu -> addAction("Hide all", this, SLOT(hideAll()));
        barsMenu -> addSeparator();

        foreach(QToolBar * bar, bars) {
            if (bar == spectrum) {
                spectrum -> generateContextMenu(barsMenu);
            } else
                barsMenu -> addAction(bar -> toggleViewAction());
        }

        connect(barsMenu, SIGNAL(hovered(QAction *)), this, SLOT(panelHighlight(QAction *)));
        connect(barsMenu, SIGNAL(aboutToHide()), this, SLOT(removePanelHighlight()));
    }

    if (hasDocs) {
        QMenu * docsMenu = menu -> addMenu("Dock bars");

        docsMenu -> addAction("Show all", Dockbars::instance(window), SLOT(showAll()));
        docsMenu -> addAction("Hide all", Dockbars::instance(window), SLOT(hideAll()));
        docsMenu -> addSeparator();

        foreach(DockBar * doc, docs)
            docsMenu -> addAction(doc -> toggleViewAction());
    }

    return menu;
}

void ToolBars::load(QJsonArray & bars) {
    QMainWindow * window = (QMainWindow *)parent();

    if (bars.count() > 0) {
        QList<QString> barsList;
        barsList.append("Media"); barsList.append("Media+"); barsList.append("Media+Position"); barsList.append("Media+Time");
        barsList.append("Media+Volume"); barsList.append("Controls"); barsList.append("Spectrum");

        QJsonObject obj, actionObj;
        QString barName;
        QToolBar * curr_bar;

        foreach(QJsonValue bar, bars) {
            obj = bar.toObject();
            barName = obj.value("title").toString();
            barsList.removeOne(barName);
            curr_bar = linkNameToToolbars(barName);
            curr_bar -> setObjectName(obj.value("name").toString(curr_bar -> objectName()));
            curr_bar -> setMovable(obj.value("movable").toBool());

            window -> addToolBar(Qt::BottomToolBarArea, curr_bar);

            if (obj.contains("actions")) {
                QJsonArray actions = obj.value("actions").toArray();

                foreach(QJsonValue act, actions) {
                    actionObj = act.toObject();
                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
                }
            }
        }

        while(barsList.length() > 0)
            window -> addToolBar(Qt::BottomToolBarArea, linkNameToToolbars(barsList.takeFirst()));
    }
    else createToolbars(window);
}

void ToolBars::save(DataStore * settings) {
    QList<QToolBar *> bars = toolbars();

    if (bars.length() > 0) {
        QJsonArray toolbar_array = QJsonArray();
        QJsonObject curr_tab;
        QList<QAction*> actions;
        ToolbarButton* button;

        foreach(QToolBar * bar, bars) {
            curr_tab = QJsonObject();

            curr_tab.insert("title", bar -> windowTitle());
            curr_tab.insert("name", bar -> objectName());
            curr_tab.insert("movable", bar -> isMovable());

            if (bar -> windowTitle() != "Media"
                    && bar -> windowTitle() != "Media+"
                    && bar -> windowTitle() != "Media+Position"
                    && bar -> windowTitle() != "Media+Time"
                    && bar -> windowTitle() != "Media+Volume"
                    && bar -> windowTitle() != "Controls"
                    && bar -> windowTitle() != "Spectrum"
               ) {
                actions = bar -> actions();
                if (actions.length() > 0) {
                    QJsonArray action_array = QJsonArray();
                    QJsonObject curr_act;

                    foreach(QAction * act, actions) {
                        if (act -> objectName() != "*Title" && QString(act -> metaObject() -> className()) == "QWidgetAction") {
                            curr_act = QJsonObject();
                            button = (ToolbarButton*) bar -> widgetForAction(act);

                            curr_act.insert("path", button -> mainPath());
                            curr_act.insert("name", button -> text());
                            action_array.append(curr_act);
                        }
                    }

                    if (action_array.count() > 0)
                        curr_tab.insert("actions", action_array);
                }
            }

            toolbar_array.append(curr_tab);

    //        bar -> toolButtonStyle();
        }

        settings -> write(ToolBars::settingsName(), toolbar_array);
    }
}

void ToolBars::createToolbars(QMainWindow * window) {
  window -> addToolBar(Qt::TopToolBarArea, createMediaBar());
  window -> addToolBar(Qt::TopToolBarArea, createTimeMediaBar());
  window -> addToolBar(Qt::TopToolBarArea, createPositionMediaBar());
  window -> addToolBarBreak();
  window -> addToolBar(Qt::TopToolBarArea, createAdditionalMediaBar());
  window -> addToolBar(Qt::TopToolBarArea, createVolumeMediaBar());
  window -> addToolBar(Qt::TopToolBarArea, createControlToolBar());
  window -> addToolBar(Qt::BottomToolBarArea, createToolBar("Folder linker 1"));
  window -> addToolBar(Qt::BottomToolBarArea, getSpectrum());
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

QToolBar * ToolBars::linkNameToToolbars(QString barName) {
    if (barName == "Media") {
        return createMediaBar();
    } else if (barName == "Media+") {
        return createAdditionalMediaBar();
    } else if (barName == "Media+Position") {
        return createPositionMediaBar();
    } else if (barName == "Media+Time") {
        return createTimeMediaBar();
    } else if (barName == "Media+Volume") {
        return createVolumeMediaBar();
    } else if (barName == "Controls") {
        return createControlToolBar();
    } else if (barName == "Spectrum") {
        return getSpectrum();
    } else {
        return createToolBar(barName);
    }
}

QToolBar * ToolBars::createToolBar(QString name) {
    ToolBar * ptb = new ToolBar(name, (QWidget *)parent());

    ptb -> setFloatable(false);
    ptb -> setStyleSheet(Stylesheets::toolbarMovableStyle());

    ptb -> setMinimumSize(60, 60);
    ptb -> setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(ptb, SIGNAL(folderDropped(QString, QString)), this, SLOT(onFolderDrop(QString, QString)));
    connect(ptb, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
    connect(ptb, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));

    return ptb;
}

QToolBar * ToolBars::precreateToolBar(QString name, bool oriented) {
    QToolBar * ptb = new QToolBar(name);
    ptb -> setObjectName("_" + name);
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
    QToolBar * ptb = precreateToolBar("Media");

    Player::instance() -> setPlayButton(ptb -> addAction(QIcon(":/play"), "Play"));
    Player::instance() -> setPauseButton(ptb -> addAction(QIcon(":/pause"), "Pause"));
    Player::instance() -> setStopButton(ptb -> addAction(QIcon(":/stop"), "Stop"));

    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createAdditionalMediaBar() {
    QToolBar * ptb = precreateToolBar("Media+");

    //TODO: add del versions od buttons
    ptb -> addAction(QIcon(":/prev"), "Prev track", Dockbars::instance(), SLOT(prevExecTriggering()));

    QIcon ico;
    ico.addPixmap(QPixmap(":/like"), QIcon::Normal);
    ico.addPixmap(QPixmap(":/unlike"), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, "Liked");
    act -> setCheckable(true);

    Player::instance() -> setLikeButton(act);

    ptb -> addAction(QIcon(":/next"), "Next track", Dockbars::instance(), SLOT(nextExecTriggering()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createPositionMediaBar() {
    QToolBar * ptb = precreateToolBar("Media+Position", true);

    slider = new MetricSlider(ptb);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);

    Player::instance() -> setTrackBar(slider);

    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createTimeMediaBar() {
    QToolBar * ptb = precreateToolBar("Media+Time");

    ClickableLabel * timeLabel = new ClickableLabel("After click invert showing time" ,"00:00", ptb);
    timeLabel -> setStyleSheet("QLabel { font-weight: bold; font-size: 12px; }");
    ptb -> addWidget(timeLabel);
    Player::instance() -> setTimePanel(timeLabel);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createVolumeMediaBar() {
    QToolBar * ptb = precreateToolBar("Media+Volume", true);

    QIcon ico;
    ico.addPixmap(QPixmap(":/mute"), QIcon::Normal);
    ico.addPixmap(QPixmap(":/volume"), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, "Mute");
    act -> setCheckable(true);

    Player::instance() -> setMuteButton(act);

    ClickableSlider * slider = new ClickableSlider(ptb);
    slider -> setTickInterval(2000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);

    Player::instance() -> setVolumeTrackBar(slider);
    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createControlToolBar() {
    QToolBar * ptb = precreateToolBar("Controls");

    ptb -> addAction(QIcon(":/add"), "Add new local tab", Dockbars::instance(), SLOT(createNewBar()));
    ptb -> addWidget(initiateVkButton());
    ptb -> addWidget(initiateSoundcloudButton());
    ptb -> addSeparator();
    ptb -> addAction(QIcon(":/search"), "Search", parent(), SLOT(showSearchDialog()));
    ptb -> addSeparator();
    ptb -> addAction(QIcon(":/settings"), "Common setting", parent(), SLOT(showSettingsDialog()));
    ptb -> adjustSize();

    return ptb;
}

Spectrum * ToolBars::getSpectrum() {
    if (spectrum == 0) {
        spectrum = new Spectrum((QWidget *)parent());

        connect(spectrum, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
        connect(spectrum, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));
    }

    return spectrum;
}

// move to the vk class
QToolButton * ToolBars::initiateVkButton() {
    if (vkToolButton == 0)
        vkToolButton = new QToolButton();
    else
        disconnect(vkToolButton, SIGNAL(clicked()), parent(), SLOT(showVKTabDialog()));

    if (VkApi::instance() -> isConnected()) {
        vkToolButton -> setIcon(QIcon(":/add_vk_on"));
        vkToolButton -> setPopupMode(QToolButton::InstantPopup);
        vkToolButton -> setToolTip("VKontakte(vk.com)");

        QMenu * vkMenu = new QMenu(vkToolButton);
        vkMenu -> addAction("Reconect", parent(), SLOT(openVKTabDialog()));
        vkMenu -> addAction("Open your tab", parent(), SLOT(showVKTabDialog()));
        vkMenu -> addAction("Open friend/group tab", parent(), SLOT(showVKRelTabDialog()));
        vkMenu -> addAction("Open recommendations", parent(), SLOT(openVKRecomendations()));
        vkToolButton -> setMenu(vkMenu);
    } else {
        vkToolButton -> setIcon(QIcon(":/add_vk"));
        vkToolButton -> setToolTip("Connect to VKontakte(vk.com)");
        connect(vkToolButton, SIGNAL(clicked()), parent(), SLOT(showVKTabDialog()));
    }

    return vkToolButton;
}

// move to the cloudsound class
QToolButton * ToolBars::initiateSoundcloudButton() {
    if (soundcloudToolButton == 0)
        soundcloudToolButton = new QToolButton();
    else
        disconnect(soundcloudToolButton, SIGNAL(clicked()), parent(), SLOT(showSoundcloudTabDialog()));

    if (SoundcloudApi::instance() -> isConnected()) {
        soundcloudToolButton -> setIcon(QIcon(":/add_soundcloud_on"));
        soundcloudToolButton -> setToolTip("Soundcloud(soundcloud.com)");
        soundcloudToolButton -> setPopupMode(QToolButton::InstantPopup);

        QMenu * vkMenu = new QMenu(soundcloudToolButton);
        vkMenu -> addAction("Reconect", parent(), SLOT(openSoundcloudTabDialog()));
        vkMenu -> addAction("Open your tab", parent(), SLOT(showSoundcloudTabDialog()));
        vkMenu -> addAction("Open friend/group tab", parent(), SLOT(showSoundcloudRelTabDialog()));
        soundcloudToolButton -> setMenu(vkMenu);
    } else {
        soundcloudToolButton -> setIcon(QIcon(":/add_soundcloud"));
        soundcloudToolButton -> setToolTip("Connect to Soundcloud(soundcloud.com)");
        connect(soundcloudToolButton, SIGNAL(clicked()), parent(), SLOT(showSoundcloudTabDialog()));
    }

    return soundcloudToolButton;
}

void ToolBars::addPanelButton(QString name, QString path, QToolBar * bar) {
    ToolbarButton * button = new ToolbarButton(name, path);
    connect(button, SIGNAL(clicked()), parent(), SLOT(openFolderTriggered()));
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
    ToolbarDialog dialog((QWidget *)parent());

    if (dialog.exec() == QDialog::Accepted) {
        QToolBar * bar = createToolBar(dialog.getName());
        bar -> setObjectName(dialog.getName() + QString::number(QDateTime::currentMSecsSinceEpoch()));
        QMainWindow * window = (QMainWindow *)parent();
        Qt::ToolBarArea area = underMouseBar ? window -> toolBarArea(underMouseBar) : Qt::BottomToolBarArea;
        window -> addToolBar(area, bar);
    }
}

void ToolBars::removePanelTriggered() {
    ((QMainWindow *)parent()) -> removeToolBar(underMouseBar);
    delete underMouseBar; //TODO: maybe use deleteLater() ?
    underMouseBar = 0;
}

void ToolBars::addPanelButtonTriggered() {
    ToolbarButtonDialog dialog((QWidget *)parent());

    if (dialog.exec() == QDialog::Accepted)
        addPanelButton(dialog.getName(), dialog.getPath(), underMouseBar);
}

void ToolBars::removePanelButtonTriggered() {
    QToolBar * bar = (QToolBar *)underMouseButton -> parentWidget();
    bar -> removeAction(bar -> actionAt(bar -> mapFromGlobal(lastClickPoint)));
}

void ToolBars::toolbarOrientationChanged(Qt::Orientation orientation) {
    QToolBar * bar = (QToolBar *)QObject::sender();

    QList<QSlider *> orientables = bar -> findChildren<QSlider *>();

    foreach(QSlider * orientable, orientables) {  //TODO: use iterator
        orientable -> setOrientation(orientation);
//        orientable -> setInvertedAppearance(orientation == Qt::Vertical);
    }
}

void ToolBars::hideAll() {
    foreach(QToolBar * bar, toolbars()) //TODO: use iterator
        bar -> setHidden(true);
}
void ToolBars::showAll() {
    foreach(QToolBar * bar, toolbars()) //TODO: use iterator
        bar -> setHidden(false);
}

void ToolBars::changeToolbarMovable() {
    if (!underMouseBar) return;
    underMouseBar -> setMovable(!underMouseBar -> isMovable());
}

void ToolBars::changeToolbarsMovable() {
    if (!underMouseBar) return;
    bool movable = !underMouseBar -> isMovable();
    foreach(QToolBar * bar, toolbars())  //TODO: use iterator
        if (movable || (!movable && !bar -> isFloating()))
            bar -> setMovable(movable);
}

void ToolBars::onFolderDrop(QString name, QString path) {
    addPanelButton(name, path, (QToolBar *)QObject::sender());
}
