#include "toolbars.h"
#include "dockbars.h"
#include "external_keys.h"
#include "modules/controls/toolbar_new_list_button.h"

#include <qdatetime.h>

using namespace Presentation;

QMenu * ToolBars::createPopupMenu() {
    QMenu * menu = new QMenu(container);

    lastClickPoint = QCursor::pos();
    QWidget * currentHover = container -> childAt(container -> mapFromGlobal(lastClickPoint));
    underMouseBar = deiterateToToolBar(currentHover);
    underMouseButton = qobject_cast<ToolbarUserButton *>(currentHover);
    bool isStatic = !qobject_cast<ToolBar *>(underMouseBar);

    menu -> addAction(QIcon(LSTR(":panel_add")), LSTR("Add panel"), this, SLOT(addPanelTriggered())/*, QKeySequence("Ctrl+P")*/);
    menu -> addAction(QIcon(LSTR(":panel_remove")), LSTR("Remove panel"), this, SLOT(removePanelTriggered())/*, QKeySequence("Ctrl+Shift+P")*/) -> setEnabled(!isStatic);
    menu -> addSeparator();

    if (underMouseBar) {
        if (underMouseBar -> isMovable()) {
            menu -> addAction(QIcon(LSTR(":locked")), LSTR("Lock bar"), this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(LSTR(":locked")), LSTR("Lock all bars"), this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+O")*/);
        } else {
            menu -> addAction(QIcon(LSTR(":unlocked")), LSTR("Unlock bar"), this, SLOT(changeToolbarMovable())/*, QKeySequence("Ctrl+L")*/);
            menu -> addAction(QIcon(LSTR(":unlocked")), LSTR("Unlock all bars"), this, SLOT(changeToolbarsMovable())/*, QKeySequence("Ctrl+L")*/);
        }

        menu -> addSeparator();
    }

    menu -> addAction(QIcon(LSTR(":drop_add")), LSTR("Add drop point"), this, SLOT(addPanelButtonTriggered())/*, QKeySequence("Ctrl+F")*/) -> setEnabled(!isStatic && underMouseBar);
    menu -> addAction(QIcon(LSTR(":drop_remove")), LSTR("Remove drop point"), this, SLOT(removePanelButtonTriggered())/*, QKeySequence("Ctrl+Shift+F")*/) -> setEnabled(underMouseButton);
    menu -> addAction(QIcon(LSTR(":edit_remove")), LSTR("Edit drop point"), this, SLOT(editPanelButtonTriggered())/*, QKeySequence("Ctrl+Shift+F")*/) -> setEnabled(underMouseButton);

    QList<QToolBar *> bars = toolbars();
    QList<DockBar *> docs = Dockbars::obj().dockbars();
    bool hasBars = bars.count() > 0, hasDocs = docs.count() > 0;

    if (hasBars || hasDocs)
        menu -> addSeparator();

    if (hasBars) {
        QMenu * barsMenu = menu -> addMenu(LSTR("Tool bars"));

        barsMenu -> addAction(LSTR("Show all"), this, SLOT(showAll()));
        barsMenu -> addAction(LSTR("Hide all"), this, SLOT(hideAll()));
        barsMenu -> addSeparator();

        for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
            if ((*bar) -> property(toolbar_block_mark).toBool()) continue;

            if ((*bar) == spectrum)
                spectrum -> generateContextMenu(barsMenu);
            else
                barsMenu -> addAction((*bar) -> toggleViewAction());
        }

        connect(barsMenu, SIGNAL(hovered(QAction *)), this, SLOT(panelHighlight(QAction *)));
        connect(barsMenu, SIGNAL(aboutToHide()), this, SLOT(removePanelHighlight()));
    }

    if (hasDocs) {
        QMenu * docsMenu = menu -> addMenu(LSTR("Tabs"));

        docsMenu -> addAction(LSTR("Show all"), &Dockbars::obj(), SLOT(showAll()));
        docsMenu -> addAction(LSTR("Hide all"), &Dockbars::obj(), SLOT(hideAll()));
        docsMenu -> addSeparator();
        docsMenu -> addAction(LSTR("Close all"), &Dockbars::obj(), SLOT(closeAll()));
        docsMenu -> addSeparator();


        for(QList<DockBar *>::Iterator doc = docs.begin(); doc != docs.end(); doc++)
            docsMenu -> addAction((*doc) -> toggleViewAction());
    }

    return menu;
}

void ToolBars::load(const QJsonArray & bars) {
    if (bars.count() > 0) {
        QList<QString> barsList;
        barsList << toolbar_media_key << toolbar_media_plus_key << toolbar_media_pos_key << toolbar_search_key
                 << toolbar_media_time_key << toolbar_media_pan_key << toolbar_media_volume_key
                 << toolbar_pl_item_features_key << toolbar_controls_key  << toolbar_settings_key
                 << toolbar_spectrum_key << toolbar_equalizer_key << toolbar_equalizer_button_key;

        QJsonObject obj, actionObj;
        QString barName;
        QToolBar * curr_bar;

        for(QJsonArray::ConstIterator bar = bars.constBegin(); bar != bars.constEnd(); bar++) {
            obj = (*bar).toObject();
            barName = obj.value(Keys::title).toString();
            barsList.removeOne(barName);
            curr_bar = linkNameToToolbars(barName);
            curr_bar -> setObjectName(obj.value(Keys::name).toString(curr_bar -> objectName()));
            curr_bar -> setMovable(obj.value(Keys::movable).toBool());

            container -> addToolBar(Qt::BottomToolBarArea, curr_bar);

            if (obj.contains(Keys::actions)) {
                QJsonArray actions = obj.value(Keys::actions).toArray();

                foreach(QJsonValue act, actions) { // rewrite on for
                    actionObj = act.toObject();
                    addPanelButton(actionObj.value(Keys::name).toString(), actionObj.value(Keys::path).toString(), curr_bar);
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
        QList<QAction *> actions;
        ToolbarUserButton * button;

        for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
//            if ((*bar) -> property(toolbar_block_mark).toBool()) continue;

            curr_tab = QJsonObject();

            curr_tab.insert(Keys::title, (*bar) -> windowTitle());
            curr_tab.insert(Keys::name, (*bar) -> objectName());
            curr_tab.insert(Keys::movable, (*bar) -> isMovable());

            if (!(*bar) -> property(toolbar_service_mark).toBool()) {
                actions = (*bar) -> actions();
                if (actions.length() > 0) {
                    QJsonArray action_array = QJsonArray();
                    QJsonObject curr_act;

                    for(QList<QAction*>::Iterator act = actions.begin(); act != actions.end(); act++) {
                        if ((*act) -> objectName() != LSTR("*Title") && QString((*act) -> metaObject() -> className()) == LSTR("QWidgetAction")) {
                            curr_act = QJsonObject();
                            button = (ToolbarUserButton *) (*bar) -> widgetForAction((*act));

                            curr_act.insert(Keys::path, button -> mainPath());
                            curr_act.insert(Keys::name, button -> text());
                            action_array.append(curr_act);
                        }
                    }

                    if (action_array.count() > 0)
                        curr_tab.insert(Keys::actions, action_array);
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
  container -> addToolBar(Qt::TopToolBarArea, createPlayedItemFeaturesBar());
  container -> addToolBar(Qt::BottomToolBarArea, createToolBar(LSTR("Folder linker 1")));
  container -> addToolBar(Qt::BottomToolBarArea, getSpectrum());
  container -> addToolBarBreak(Qt::BottomToolBarArea);

  QToolBar * eql = createEqualizerToolBar();
  container -> addToolBar(Qt::BottomToolBarArea, eql);
  eql -> hide();

  pl_item_features -> hide();
//  sl_item_features -> hide();
}

void ToolBars::updateBarStyle(QToolBar * bar) {
    if (!bar) return;
    bool movable = bar -> isMovable() || qobject_cast<ToolBar *>(bar) != 0;
    Settings::currentStyle -> applyProperty(bar, "state", movable ? LSTR("movable") : LSTR("fixed"));
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
    else if (barName == toolbar_pl_item_features_key)   return createPlayedItemFeaturesBar();
    else if (barName == toolbar_equalizer_button_key)   return createEqualizerButtonBar();
    else if (barName == toolbar_media_plus_key)         return createAdditionalMediaBar();
    else if (barName == toolbar_media_pos_key)          return createPositionMediaBar();
    else if (barName == toolbar_media_time_key)         return createTimeMediaBar();
    else if (barName == toolbar_media_volume_key)       return createVolumeMediaBar();
    else if (barName == toolbar_controls_key)           return createControlToolBar();
    else if (barName == toolbar_spectrum_key)           return getSpectrum();
    else if (barName == toolbar_equalizer_key)          return createEqualizerToolBar();
    else if (barName == toolbar_media_pan_key)          return createPanMediaBar();
    else if (barName == toolbar_search_key)             return createSearchButtonBar();
    else if (barName == toolbar_settings_key)           return createSettingsButtonBar();
    else                                                return createToolBar(barName);
}

QToolBar * ToolBars::createToolBar(const QString & name) {
    ToolBar * ptb = new ToolBar(name, container);

    ptb -> setFloatable(false);
    Settings::currentStyle -> applyProperty(ptb, "state", LSTR("movable"));

    ptb -> setMinimumSize(60, 60);
    ptb -> setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(ptb, SIGNAL(folderDropped(QString, QString)), this, SLOT(onFolderDrop(QString, QString)));
    connect(ptb, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
    connect(ptb, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));

    return ptb;
}

QToolBar * ToolBars::precreateToolBar(const QString & name, bool oriented) {
    QToolBar * ptb = new QToolBar(name, container);
    int icoSize = Settings::obj().toolIconSize();

    ptb -> setProperty(toolbar_service_mark, true);
    ptb -> setObjectName('_' % name);
    ptb -> setMinimumSize(icoSize, 30);
    ptb -> setIconSize(QSize(icoSize, icoSize));

    ptb -> setAttribute(Qt::WA_NoSystemBackground, true);
    ptb -> setAttribute(Qt::WA_TranslucentBackground, true);
    ptb -> setFloatable(false);

    if (oriented)
        connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(toolbarOrientationChanged(Qt::Orientation)));
    connect(ptb, SIGNAL(topLevelChanged(bool)), this, SLOT(onTopLevelChanged(bool)));
    connect(ptb, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));

    return ptb;
}

QToolBar * ToolBars::createPlayedItemFeaturesBar() {
    pl_item_features = precreateToolBar(toolbar_pl_item_features_key);
    pl_item_features -> setProperty(toolbar_block_mark, true);

    pl_item_features -> addSeparator();
    pl_more_items_btn = pl_item_features -> addAction(QIcon(LSTR(":/download")), LSTR("Load more"), this, SLOT(loadMoreItem()));
    pl_item_features -> addSeparator();

    pl_item_song_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_song")), LSTR("Recommendations for played item"), this, SLOT(openRecsforItem()));
    pl_item_singer_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_singer")), LSTR("Recommendations for played item artists"), this, SLOT(openRecsforItemArtist()));
    pl_item_singer_song_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_singer_song")), LSTR("Tracks from played item artists"), this, SLOT(openTracksforArtist()));
    pl_item_singer_album_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_singer_album")), LSTR("Albums from played item artists"), this, SLOT(openAlbumsForArtist()));
    pl_item_owner_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_owner")), LSTR("Recommendations for played item owner"), this, SLOT(openRecsforItemUser()));
    pl_item_tags_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_tag")), LSTR("Recommendations for played item tags"), this, SLOT(openRecsForItemTags()));
    pl_item_labels_song_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_label")), LSTR("Tracks from played item labels"), this, SLOT(openTracksForLabel()));
    pl_item_labels_album_btn = pl_item_features -> addAction(QIcon(LSTR(":/item_label")), LSTR("Tracks from played item labels"), this, SLOT(openAlbumsForLabel()));

    pl_item_features -> adjustSize();

    connect(&DataFactory::obj(), SIGNAL(itemFeaturesChanged()), this, SLOT(itemFeaturesChanged()));

    return pl_item_features;
}

QToolBar * ToolBars::createMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_key);

    play_btn = ptb -> addAction(QIcon(LSTR(":/play")), LSTR("Play"));
    PlayerFactory::obj().registerCallback(call_in, play_btn, SIGNAL(triggered(bool)), SLOT(play()));

    pause_btn = ptb -> addAction(QIcon(LSTR(":/pause")), LSTR("Pause"));
    PlayerFactory::obj().registerCallback(call_in, pause_btn, SIGNAL(triggered(bool)), SLOT(pause()));
    pause_btn -> setVisible(false);

    stop_btn = ptb -> addAction(QIcon(LSTR(":/stop")), LSTR("Stop"));
    PlayerFactory::obj().registerCallback(call_in, stop_btn, SIGNAL(triggered(bool)), SLOT(stop()));
    stop_btn -> setVisible(false);


    QAction * act = ptb -> addAction(QIcon(LSTR(":/cycling")), LSTR("Looping current track"));
    act -> setCheckable(true);
    PlayerFactory::obj().registerCallback(call_in, act, SIGNAL(triggered(bool)), SLOT(loop(bool)));

    PlayerFactory::obj().registerCallback(call_out, this, SIGNAL(stateChanged(const PlayerState &)), SLOT(playerStateChanged(const PlayerState &)));

    ptb -> setMinimumWidth(55);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createAdditionalMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_plus_key);

    //TODO: add del versions od buttons
    ptb -> addAction(QIcon(LSTR(":/prev")), LSTR("Prev track"), &Dockbars::obj(), SLOT(playPrev()));

    QIcon ico;
    ico.addPixmap(QPixmap(LSTR(":/like")), QIcon::Normal);
    ico.addPixmap(QPixmap(LSTR(":/unlike")), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, LSTR("Liked"));
    act -> setCheckable(true);
    connect(act, SIGNAL(triggered(bool)), &DataFactory::obj(), SLOT(changeLikeStatus(bool)));
    connect(&DataFactory::obj(), SIGNAL(likeChanged(bool)), act, SLOT(setChecked(bool)));

    ptb -> addAction(QIcon(LSTR(":/next")), LSTR("Next track"), &Dockbars::obj(), SLOT(playNext()));
    ptb -> setMinimumWidth(55);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createPositionMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_pos_key, true);

    slider = new MetricSlider(ptb);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(60, 30);
    Settings::currentStyle -> applyProperty(slider, "position", true);
    slider -> setMinimum(0);
    slider -> setMaximum(0);

    PlayerFactory::obj().registerCallback(call_out, slider, SIGNAL(positionChanged(int)), SLOT(setValueSilently(int)));
    PlayerFactory::obj().registerCallback(call_out, slider, SIGNAL(durationChanged(int)), SLOT(setMax(int)));
    PlayerFactory::obj().registerCallback(call_in, slider, SIGNAL(valueChanged(int)), SLOT(setPosition(int)));

    ptb -> addWidget(slider);
    ptb -> setMinimumWidth(70);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createPanMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_pan_key, true);

    ClickableSlider * pslider = new ClickableSlider(0, ptb);
    pslider -> setOrientation(Qt::Horizontal);
    pslider -> setMinimumSize(60, 30);
    Settings::currentStyle -> applyProperty(pslider, "pan", true);

    PlayerFactory::obj().registerCallback(call_in, pslider, SIGNAL(valueChanged(int)), SLOT(setPan(int)));
    PlayerFactory::obj().registerCallback(call_out, pslider, SIGNAL(panChanged(int)), SLOT(setValueSilently(int)));

    pslider -> setMinimum(-1000);
    pslider -> setMaximum(1000);
    pslider -> setValue(0);

    ptb -> addWidget(pslider);
    ptb -> setMinimumWidth(70);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createTimeMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_time_key);

    TimeLabel * timeLabel = new TimeLabel(LSTR("After click invert showing time") , QString(), ptb);
    Settings::currentStyle -> applyProperty(timeLabel, "timer", true);
    ptb -> addWidget(timeLabel);

    PlayerFactory::obj().registerCallback(call_out, timeLabel, SIGNAL(positionChanged(int)), SLOT(setPos(int)));
    PlayerFactory::obj().registerCallback(call_out, timeLabel, SIGNAL(durationChanged(int)), SLOT(setTotal(int)));

//    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createVolumeMediaBar() {
    QToolBar * ptb = precreateToolBar(toolbar_media_volume_key, true);

    QIcon ico;
    ico.addPixmap(QPixmap(LSTR(":/mute")), QIcon::Normal);
    ico.addPixmap(QPixmap(LSTR(":/volume")), QIcon::Normal, QIcon::On);

    QAction * act = ptb -> addAction(ico, LSTR("Mute"));
    act -> setCheckable(true);
    PlayerFactory::obj().registerCallback(call_in, act, SIGNAL(triggered(bool)), SLOT(mute(bool)));
    PlayerFactory::obj().registerCallback(call_out, act, SIGNAL(muteChanged(bool)), SLOT(setChecked(bool)));

    ClickableSlider * slider = new ClickableSlider(100000000, ptb, LSTR("volume"));

    slider -> setTickInterval(2000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(45, 30);
    slider -> setMaximum(10000);
    slider -> setValue(10000);

    PlayerFactory::obj().registerCallback(call_in, slider, SIGNAL(valueChanged(int)), SLOT(setVolume(int)));
    PlayerFactory::obj().registerCallback(call_out, slider, SIGNAL(volumeChanged(int)), SLOT(setValueSilently(int)));

    ptb -> addWidget(slider);
    ptb -> setMinimumWidth(55);
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createControlToolBar() {
    QToolBar * ptb = precreateToolBar(toolbar_controls_key);

    ToolbarNewListButton * listBtn = new ToolbarNewListButton(QIcon(LSTR(":/add")), ptb);
    listBtn -> setToolTip(LSTR("Create new tab"));
    connect(listBtn, SIGNAL(clicked(bool)), &Dockbars::obj(), SLOT(createNewBar()));
    connect(listBtn, SIGNAL(folderAdded(QString,QUrl)), &Dockbars::obj(), SLOT(createNewBar(QString,QUrl)));
    ptb -> addWidget(listBtn);

    QMenu * menu = new QMenu();

    QMap<DataSubType, ISource *> apis = Web::Apis::sourcesList();
    for(QMap<DataSubType, ISource *>::Iterator api = apis.begin(); api != apis.end(); api++) {
        QToolButton * btn = api.value() -> initButton(container);
        if (btn) {
            if (api.value() -> isPrimary())
                ptb -> addWidget(btn);
            else { // TODO: not tested
                if (btn -> menu())
                    menu -> addMenu(new QMenu(btn -> menu()));
                else menu -> addAction(LSTR("Connect"), api.value(), SLOT(openTab()));

                btn -> deleteLater();
            }
        }
    }

    if (menu -> actions().isEmpty())
        menu -> deleteLater();
    else {
        QToolButton * other_button = new QToolButton(ptb);
        other_button -> setToolTip(LSTR("Other sources"));
        other_button -> setPopupMode(QToolButton::InstantPopup);
        other_button -> setMenu(menu);
        menu -> setParent(other_button);

        ptb -> addWidget(other_button);
    }

    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createSearchButtonBar() {
    QToolBar * ptb = precreateToolBar(toolbar_search_key);

    ptb -> addAction(QIcon(LSTR(":/search")), LSTR("Search"), container, SLOT(showSearchDialog()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar * ToolBars::createSettingsButtonBar() {
    QToolBar * ptb = precreateToolBar(toolbar_settings_key);

    ptb -> addAction(QIcon(LSTR(":/settings")), LSTR("Common setting"), container, SLOT(showSettingsDialog()));
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
    act -> setIcon(QIcon(LSTR(":/equalizer")));
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

void ToolBars::addPanelButton(const QString & name, const QString & path, QToolBar * bar) {
    ToolbarUserButton * button = new ToolbarUserButton(name, path);
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
        Settings::currentStyle -> applyProperty(highlighted, "state", LSTR("lighted"));
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

void ToolBars::editPanelButtonTriggered() {
    ToolbarUserButton * btn = (ToolbarUserButton *)underMouseButton;
    UserActionDialog dialog(container);
    dialog.buildToolbarButtonForm(btn -> text(), btn -> mainPath());

    if (dialog.exec() == QDialog::Accepted) {
        btn -> setText(dialog.getValue(dialog.name_key));
        btn -> setMainPath(dialog.getValue(dialog.path_key));
    }
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

void ToolBars::itemFeaturesChanged() {
    IItem * it = DataFactory::obj().playedItem();
    bool show = false;

    if (it) {
        ISource * source = Web::Apis::source(it -> dataType());

        if (source) {
            bool has_more_items = it -> parent() -> hasMoreItems()/* || it -> hasMoreItems()*/;
            pl_more_items_btn -> setVisible(has_more_items);

            bool has_item_recs = source -> hasSimillarAudioByAudio();
            pl_item_song_btn -> setVisible(has_item_recs);

            bool has_owner_recs = source -> hasSimillarAudioByUser();
            pl_item_owner_btn -> setVisible(has_owner_recs);

            bool has_artist_recs = source -> hasSimillarAudioByArtist();
            pl_item_singer_btn -> setVisible(has_artist_recs);

            bool has_artist_songs = source -> hasAudioFromSameArtist();
            pl_item_singer_song_btn -> setVisible(has_artist_songs);

            bool has_artist_albums = source -> hasAlbumsFromSameArtist();
            pl_item_singer_album_btn -> setVisible(has_artist_albums);

            bool has_tags_recs = it -> isAudio() ? source -> hasSimillarAudioByTag() : source -> hasSimillarVideoByTag();
            pl_item_tags_btn -> setVisible(has_tags_recs);

            bool has_label_song_recs = source -> hasAudioFromSameLabel();
            pl_item_labels_song_btn -> setVisible(has_label_song_recs);

            bool has_label_album_recs = source -> hasAlbumsFromSameLabel();
            pl_item_labels_album_btn -> setVisible(has_label_album_recs);

            show |= has_more_items || has_item_recs || has_owner_recs ||
                    has_artist_recs || has_tags_recs || has_label_song_recs ||
                    has_artist_albums || has_artist_songs || has_label_album_recs;
        }
    }

    pl_item_features -> setHidden(!show);
}

void ToolBars::loadMoreItem() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            view -> runItemCmd(it);
    }
}

void ToolBars::openRecsforItem() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForItem(it);
    }
}
void ToolBars::openRecsforItemUser() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForUser(it);
    }
}
void ToolBars::openRecsforItemArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForArtist(it);
    }
}
void ToolBars::openTracksforArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameArtist(it);
    }
}
void ToolBars::openAlbumsForArtist() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameArtist(it);
    }
}
void ToolBars::openRecsForItemTags() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedRecsForTags(it);
    }
}
void ToolBars::openTracksForLabel() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAudioFromSameLabel(it);
    }
}
void ToolBars::openAlbumsForLabel() {
    IView * view = Dockbars::obj().playedView();
    if (view) {
        IItem * it = DataFactory::obj().playedItem();
        if (it)
            ((Models::IModel *)view -> model()) -> proceedAlbumsFromSameLabel(it);
    }
}

void ToolBars::playerStateChanged(const PlayerState & state) {
    play_btn -> setVisible(state != PlayingState);
    pause_btn -> setVisible(state == PlayingState);
    stop_btn -> setVisible(state == PlayingState || state == PausedState);
}

void ToolBars::onFolderDrop(const QString & name, const QString & path) {
    addPanelButton(name, path, (QToolBar *)QObject::sender());
}
