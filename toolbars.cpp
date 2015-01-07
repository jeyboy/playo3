#include "toolbars.h"
#include <qdebug.h>

using namespace Playo3;

ToolBars *ToolBars::self = 0;

ToolBars *ToolBars::instance(QObject * parent) {
    if(!self)
        self = new ToolBars(parent);
    return self;
}

QMenu * ToolBars::improvePopupMenu(QMainWindow * window, QMenu * menu) {
    connect(menu, SIGNAL(hovered(QAction *)), this, SLOT(panelHighlight(QAction *)));
    connect(menu, SIGNAL(aboutToHide()), this, SLOT(removePanelHighlight()));

    menu -> insertSeparator(menu -> actions().first());

    lastClickPoint = QCursor::pos();
    QWidget * widget = window -> childAt(window -> mapFromGlobal(lastClickPoint));
    QString widgetClassName = QString(widget -> metaObject() -> className());

    if (widgetClassName == "Playo3::ToolbarButton") {
        underMouseButton = ((ToolbarButton*)widget);
        underMouseBar = ((ToolBar *)underMouseButton -> parentWidget());
    } else if (widgetClassName == "QLabel") {
        underMouseBar = (ToolBar *)widget -> parentWidget();
    } else {
        underMouseBar = ((ToolBar *)widget);
    }

    QAction * removeButtonAct = new QAction(QIcon(":drop_remove"), "Remove drop point", menu);
    removeButtonAct -> setEnabled(widgetClassName == "Playo3::ToolbarButton");
    menu -> insertAction(menu->actions().first(), removeButtonAct);
    connect(removeButtonAct, SIGNAL(triggered(bool)), this, SLOT(removePanelButtonTriggered()));


    QAction * addButtonAct = new QAction(QIcon(":drop_add"), "Add drop point", menu);
    addButtonAct -> setEnabled(widgetClassName == "Playo3::ToolBar");
    menu -> insertAction(menu->actions().first(), addButtonAct);
    connect(addButtonAct, SIGNAL(triggered(bool)), this, SLOT(addPanelButtonTriggered()));

//    menu -> insertSection(menu->actions().first(), QIcon(":drops"),  "Drop points");
    menu -> insertSeparator(menu->actions().first());

    QAction * removePanelAct = new QAction(QIcon(":panel_remove"), "Remove panel", menu);
    removePanelAct -> setEnabled(widgetClassName == "Playo3::ToolBar" || widgetClassName == "QLabel");
    connect(removePanelAct, SIGNAL(triggered(bool)), this, SLOT(removePanelTriggered()));
    menu -> insertAction(menu->actions().first(), removePanelAct);

    QAction * addPanelAct = new QAction(QIcon(":panel_add"), "Add panel", menu);
    connect(addPanelAct, SIGNAL(triggered(bool)), this, SLOT(addPanelTriggered()));
    menu -> insertAction(menu->actions().first(), addPanelAct);

//    menu -> insertSection(menu->actions().first(), QIcon(":panels"), "Panel");
    menu -> insertSeparator(menu->actions().first());

    //    activeBar

    ////////////////////////// for bar movable fixing ////////////////////////////////
    if (widgetClassName == "QToolBar" || widgetClassName == "Playo3::ToolBar" || widgetClassName == "Playo3::Spectrum") {
        activeBar = ((QToolBar*)widget);
    } else {
        activeBar = ((QToolBar*)widget -> parentWidget());
    }

    QAction * fixToolbarAct, * fixToolbarsAct;

    if (activeBar -> isMovable()) {
        fixToolbarAct = new QAction(QIcon(":locked"), "Static bar", menu);
        fixToolbarsAct = new QAction(QIcon(":locked"), "All bars to Static", menu);
    } else {
        fixToolbarAct = new QAction(QIcon(":unlocked"), "Movable bar", menu);
        fixToolbarsAct = new QAction(QIcon(":unlocked"), "All bars to Movable", menu);
    }

    menu -> insertAction(menu -> actions().first(), fixToolbarsAct);
    connect(fixToolbarsAct, SIGNAL(triggered(bool)), this, SLOT(changeToolbarsMovable()));

    menu -> insertAction(menu -> actions().first(), fixToolbarAct);
    connect(fixToolbarAct, SIGNAL(triggered(bool)), this, SLOT(changeToolbarMovable()));

    //////////////////////////////////////////////////////////////////////////////////

    return menu;
}

void ToolBars::load(QMainWindow * window, QJsonArray & bars) {
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
            updateToolbarMovable(curr_bar, obj.value("movable").toBool());

            window -> addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);

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
    } else {
        createToolbars(window);
    }
}

void ToolBars::save(QMainWindow * window, DataStore * settings) {
    QList<QToolBar *> bars = toolbars();

    if (bars.length() > 0) {
        QJsonArray toolbar_array = QJsonArray();
        QJsonObject curr_tab;
        QList<QAction*> actions;
        ToolbarButton* button;

        foreach(QToolBar * bar, bars) {
            curr_tab = QJsonObject();

            curr_tab.insert("area", window -> toolBarArea(bar));
            curr_tab.insert("title", bar -> windowTitle());
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

                            curr_act.insert("path", button -> path);
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
//  window -> addToolBar(Qt::BottomToolBarArea, getSpectrum());
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
    ptb -> setMinimumSize(60, 60);
    ptb -> setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(ptb, SIGNAL(folderDropped(QString, QString)), this, SLOT(folderDropped(QString, QString)));
    return ptb;
}

QToolBar* ToolBars::createMediaBar() {
    QToolBar* ptb = new QToolBar("Media");
    ptb -> setObjectName("_Media");
    ptb -> setMinimumSize(30, 30);

    connect(ptb, SIGNAL(visibilityChanged(bool)), this, SLOT(toolbarVisibilityChanged(bool)));

    Player::instance() -> setPlayButton(ptb -> addAction(QIcon(":/play"), "Play"));
    Player::instance() -> setPauseButton(ptb -> addAction(QIcon(":/pause"), "Pause"));
    Player::instance() -> setStopButton(ptb -> addAction(QIcon(":/stop"), "Stop"));

    ptb -> adjustSize();

    return ptb;
}

QToolBar* ToolBars::createAdditionalMediaBar() {
    QToolBar* ptb = new QToolBar("Media+");
    ptb -> setObjectName("_Media+");
    ptb -> setMinimumSize(30, 30);

    ptb -> addAction(QIcon(":/prev"), "Prev track", parent(), SLOT(prevItemTriggered()));
    Player::instance() -> setLikeButton(ptb -> addAction(QIcon(":/like"), "Liked"));
    ptb -> addAction(QIcon(":/next"), "Next track", parent(), SLOT(nextItemTriggered()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar* ToolBars::createPositionMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Position");
    ptb -> setObjectName("_Media+Position");
    ptb -> setMinimumSize(30, 30);

    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(toolbarOrientationChanged(Qt::Orientation)));

    Slider * slider = new Slider(ptb, true);
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);

    Player::instance() -> setTrackBar(slider);

    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* ToolBars::createTimeMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Time");
    ptb -> setObjectName("_Media+Time");
    ptb -> setMinimumSize(30, 30);

    ClickableLabel * timeLabel = new ClickableLabel("00:00");
    timeLabel -> setStyleSheet("QLabel { font-weight: bold; font-size: 12px; }");
    ptb -> addWidget(timeLabel);
    Player::instance() -> setTimePanel(timeLabel);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* ToolBars::createVolumeMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Volume");
    ptb -> setObjectName("_Media+Volume");
    ptb -> setMinimumSize(30, 30);

    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(toolbarOrientationChanged(Qt::Orientation)));

    QIcon ico;
    ico.addPixmap(QPixmap(":/mute"), QIcon::Normal);
    ico.addPixmap(QPixmap(":/volume"), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, "Mute");
    act -> setCheckable(true);

    Player::instance() -> setMuteButton(act);

    Slider * slider = new Slider();
    slider -> setTickInterval(2000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);

    Player::instance() -> setVolumeTrackBar(slider);
    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* ToolBars::createControlToolBar() {
    QToolBar* ptb = new QToolBar("Controls");
    ptb -> setObjectName("_Controls");
    ptb -> setMinimumSize(30, 30);

    ptb -> addAction(QIcon(":/add"), "Add new local tab", parent(), SLOT(showAttTabDialog()));
    ptb -> addWidget(initiateVkButton());
    ptb -> addWidget(initiateSoundcloudButton());
    ptb -> addSeparator();
    ptb -> addAction(QIcon(":/settings"), "Common setting", parent(), SLOT(showSettingsDialog()));
    ptb -> adjustSize();

    return ptb;
}

Spectrum * ToolBars::getSpectrum() {
    if (spectrum == 0) {
        spectrum = new Spectrum((QWidget *)parent());
//        spectrum -> setAutoFillBackground(true);
//        spectrum -> setPalette(pal);
    }

    return spectrum;
}

// move to the vk class
QToolButton * ToolBars::initiateVkButton() {
    if (vkToolButton == 0) {
        vkToolButton = new QToolButton();
    }
    else
        disconnect(vkToolButton, SIGNAL(clicked()), parent(), SLOT(showVKTabDialog()));

//    if (VkApi::instance() -> isConnected()) {
//        vkToolButton -> setIcon(QIcon(":/add_vk_on"));
//        vkToolButton -> setPopupMode(QToolButton::InstantPopup);
//        vkToolButton -> setToolTip("VKontakte(vk.com)");

//        QMenu * vkMenu = new QMenu(vkToolButton);
//        vkMenu -> addAction("Reconect", parent(), SLOT(openVKTabDialog()));
//        vkMenu -> addAction("Open your tab", parent(), SLOT(showVKTabDialog()));
//        vkMenu -> addAction("Open friend/group tab", parent(), SLOT(showVKRelTabDialog()));
//        vkToolButton -> setMenu(vkMenu);
//    } else {
//        vkToolButton -> setIcon(QIcon(":/add_vk"));
//        vkToolButton -> setToolTip("Connect to VKontakte(vk.com)");
//        connect(vkToolButton, SIGNAL(clicked()), parent(), SLOT(showVKTabDialog()));
//    }

    return vkToolButton;
}

// move to the cloudsound class
QToolButton * ToolBars::initiateSoundcloudButton() {
    if (soundcloudToolButton == 0) {
        soundcloudToolButton = new QToolButton();
    }
    else
        disconnect(soundcloudToolButton, SIGNAL(clicked()), parent(), SLOT(showSoundcloudTabDialog()));

//    if (SoundcloudApi::instance() -> isConnected()) {
//        soundcloudToolButton -> setIcon(QIcon(":/add_soundcloud_on"));
//        soundcloudToolButton -> setToolTip("Soundcloud(soundcloud.com)");
//        soundcloudToolButton -> setPopupMode(QToolButton::InstantPopup);

//        QMenu * vkMenu = new QMenu(soundcloudToolButton);
//        vkMenu -> addAction("Reconect", parent(), SLOT(openSoundcloudTabDialog()));
//        vkMenu -> addAction("Open your tab", parent(), SLOT(showSoundcloudTabDialog()));
//        vkMenu -> addAction("Open friend/group tab", parent(), SLOT(showSoundcloudRelTabDialog()));
//        soundcloudToolButton -> setMenu(vkMenu);
//    } else {
//        soundcloudToolButton -> setIcon(QIcon(":/add_soundcloud"));
//        soundcloudToolButton -> setToolTip("Connect to Soundcloud(soundcloud.com)");
//        connect(soundcloudToolButton, SIGNAL(clicked()), parent(), SLOT(showSoundcloudTabDialog()));
//    }

    return soundcloudToolButton;
}

void ToolBars::addPanelButton(QString name, QString path, QToolBar * bar) {
    ToolbarButton * button = new ToolbarButton(name, path);
    bar -> addWidget(button);
    connect(button, SIGNAL(clicked()), parent(), SLOT(openFolderTriggered()));
}

bool ToolBars::isToolbarNameUniq(QString name) {
    foreach(QToolBar * bar, toolbars()) {
        if (bar -> windowTitle() == name)
            return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void ToolBars::panelHighlight(QAction *action) {
    QString widgetClassName = QString(action -> parentWidget() -> metaObject() -> className());

    if (highlighted != 0)
        emit removePanelHighlight();

    if (widgetClassName == "Playo3::ToolBar" || widgetClassName == "QToolBar") {
        highlighted = (QToolBar *)action -> parentWidget();
        highlighted -> setStyleSheet("QToolBar { border: 2px dashed #00FFFF; }" );
    }
}

void ToolBars::removePanelHighlight() {
    if (highlighted != 0) {
        updateToolbarMovable(highlighted, highlighted -> isMovable());
        highlighted = 0;
    }
}

void ToolBars::addPanelTriggered() {
    ToolbarDialog dialog((QWidget *)parent());

    while (dialog.exec() == QDialog::Accepted) {
        if (isToolbarNameUniq(dialog.getName())) {
            ((QMainWindow *)parent()) -> addToolBar(Qt::BottomToolBarArea, createToolBar(dialog.getName()));
            return;
        }
    }
}

void ToolBars::removePanelTriggered() {
    ((QMainWindow *)parent()) -> removeToolBar(underMouseBar);
    delete underMouseBar;
    underMouseBar = 0;
}

void ToolBars::addPanelButtonTriggered() {
    ToolbarButtonDialog dialog((QWidget *)parent());

    if (dialog.exec() == QDialog::Accepted) {
        addPanelButton(dialog.getName(), dialog.getPath(), underMouseBar);
    }
}

void ToolBars::removePanelButtonTriggered() {
    QToolBar * bar = (QToolBar *)underMouseButton -> parentWidget();
    bar -> removeAction(bar -> actionAt(bar -> mapFromGlobal(lastClickPoint)));
}

void ToolBars::toolbarVisibilityChanged(bool visible) {
  if (visible == false)
    ((QToolBar*)QObject::sender()) -> setVisible(true);
}

void ToolBars::toolbarOrientationChanged(Qt::Orientation orientation) {
    QToolBar * bar = (QToolBar*)QObject::sender();

    QList<QSlider *> orientables = bar -> findChildren<QSlider *>();

    foreach(QSlider * orientable, orientables) {
        orientable -> setOrientation(orientation);
//        orientable -> setInvertedAppearance(orientation == Qt::Vertical);
    }
}

void ToolBars::changeToolbarMovable() {
    updateToolbarMovable(activeBar, !activeBar -> isMovable());
}

void ToolBars::changeToolbarsMovable() {
    bool movable = !activeBar -> isMovable();
    foreach(QToolBar * bar, toolbars())
        updateToolbarMovable(bar, movable);
}

void ToolBars::folderDropped(QString name, QString path) {
    addPanelButton(name, path, (QToolBar*)QObject::sender());
}
