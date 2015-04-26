#include "dockbars.h"
#include <qdebug.h>

using namespace Playo3;

Dockbars * Dockbars::self = 0;

Dockbars * Dockbars::instance(QWidget * parent) {
    if(!self)
        self = new Dockbars(parent);
    return self;
}

void Dockbars::load(QJsonArray & bars) {
    int userTabsAmount = 0;
    MainWindow * window = (MainWindow *)parent();
    QList<QString> barsList;
    barsList.append("Downloads"/*, "Screen"*/);

    if (bars.count() > 0) {
        QJsonObject obj;
        QString barName;
        QDockWidget * curr_bar;

        QJsonArray::iterator it = bars.begin();

        for(;it != bars.end(); it++) {
            obj = (*it).toObject();
            barName = obj.value("title").toString();
            userTabsAmount += (!barsList.removeOne(barName));
            curr_bar = linkNameToToolbars(barName, ViewSettings(obj.value("set").toObject()), obj.value("cont").toObject());
            curr_bar -> setObjectName(obj.value("name").toString(curr_bar -> objectName()));

            if (obj.value("stick").toBool())
                ((DockBar *)curr_bar) -> markAsSticked();

            ((DockBar *)curr_bar) -> useVerticalTitles(obj.value("vertical").toBool());

            window -> addDockWidget(Qt::TopDockWidgetArea, curr_bar);

            if (obj.value("played").toBool()) {
                IView * v = view(qobject_cast<DockBar *>(curr_bar));
                if (v) {
                    QString path = obj.value("played_item").toString();
                    if (!path.isEmpty())
                        v -> execPath(path, true, obj.value("played_time").toInt());
                }
            }
        }
    }

    if (!Settings::instance() -> isSaveCommonTab()) {
        window -> addDockWidget(Qt::TopDockWidgetArea, commonBar());
        if (userTabsAmount != 0)
            commonBar() -> hide();
    }

    QJsonObject def;
    ViewSettings defSettings;
    while(barsList.length() > 0)
        window -> addDockWidget(Qt::TopDockWidgetArea, linkNameToToolbars(barsList.takeFirst(), defSettings, def));
}

void Dockbars::save(DataStore * settings) {
    QList<DockBar *> bars = dockbars();

    if (bars.length() > 0) {
        QJsonArray bar_array = QJsonArray();
        QJsonObject curr_bar;

        QList<DockBar *>::Iterator it = bars.begin();

        for(; it != bars.end(); it++) {
            IView * v = view((*it));

            if ((*it) -> windowTitle() == "Common") {
                if (v && v -> isCommon() && !Settings::instance() -> isSaveCommonTab())
                    continue;
            }

            curr_bar = QJsonObject();
            curr_bar.insert("title", (*it) -> windowTitle());
            curr_bar.insert("name", (*it) -> objectName());
            curr_bar.insert("stick", (*it) -> isSticked());
            curr_bar.insert("vertical", (*it) -> isUsedVerticalTitles());

            if ((*it) -> windowTitle() == "Downloads") {
                curr_bar.insert("cont", ((DownloadView *)(*it) -> mainWidget()) -> toJson());
            } else {
                if ((*it) == played) {
                    curr_bar.insert("played", true);
                    if (Player::instance() -> playedIndex().isValid()) {
                        curr_bar.insert("played_item", Player::instance() -> playedIndex().data(ITREEPATH).toString());
                        curr_bar.insert("played_time", Player::instance() -> getPosition());
                    }
                }

                if (v) {
                    curr_bar.insert("set", v -> settings().toJson());
                    curr_bar.insert("cont", v -> toJson());
                }
            }

            bar_array.append(curr_bar);
        }

        settings -> write(Dockbars::settingsName(), bar_array);
    }
}

QDockWidget * Dockbars::linkNameToToolbars(QString barName, ViewSettings settings, QJsonObject attrs) {
    if (barName == "Screen") {
        return 0; // stub
    } else if (barName == "Common") {
        return createDocBar(barName, settings, &attrs, false);
    } else if (barName == "Downloads") {
        DockBar * bar = createDocBar(barName, false);
        bar -> setWidget(DownloadView::instance(&attrs, parentWidget()));
        return bar;
    } else return createDocBar(barName, settings, &attrs);
}

DockBar * Dockbars::commonBar() {
    if (!common) {
        ViewSettings defSettings(list, true, false, false, true);
        common = createDocBar("Common", defSettings, 0, false);
    }

    return common;
}

DockBar * Dockbars::createDocBar(QString name, ViewSettings settings, QJsonObject * attrs, bool closable) {
    IView * view;
    DockBar * bar = createDocBar(name, closable);

    switch(settings.type) {
        case list: {
            view = new ListView(bar, settings, attrs);
        break;}
        case level_tree: {
            view = new LevelTreeView(bar, settings, attrs);
        break;}
        case tree: {
            view = new TreeView(bar, settings, attrs);
        break;}
        case vk: {
            view = new VkView(bar, settings, attrs);
        break;}
//        case soundcloud: {
//            view = new SoundcloudView(bar, settings, attrs);
//        break;}

        default: view = 0;
    }
    bar -> setWidget(view);
    if (!attrs)
        ((IModel *)view -> model()) -> refresh();

    return bar;
}

DockBar * Dockbars::createDocBar(QString name, bool closable, QWidget * content) {
    DockBar * dock = new DockBar(name, (QWidget *)parent(), closable, Qt::WindowMinMaxButtonsHint);

    connect(dock, SIGNAL(closing()), this, SLOT(barClosed()));
    connect(dock, SIGNAL(topLevelChanged(bool)), this, SLOT(updateActiveTabIcon()));
//    active = dock;
//    dock -> showFullScreen();

    if (content) {
        content -> setParent(dock);
        dock -> setWidget(content);
    }

    return dock;
//    ((QWidget *)parent())->tabifyDockWidget(dockWidget1,dockWidget2);
}

void Dockbars::useVeticalTitles(bool vertical) {
    QList<DockBar *> bars = dockbars();
    QList<DockBar *>::Iterator it = bars.begin();

    for(; it != bars.end(); it++)
        (*it) -> useVerticalTitles(vertical);
}

void Dockbars::changeBarsVisibility(bool hide) {
    QList<DockBar *> bars = dockbars();
    QList<DockBar *>::Iterator it = bars.begin();

    for(; it != bars.end(); it++)
        (*it) -> setHidden(hide);
}

void Dockbars::initPlayed() {
    if (!played) {
        played = active;

        if (!played) {
            QList<DockBar *> bars = dockbars();

            if (bars.count() > 0) {
                QList<DockBar *>::Iterator it = bars.begin();

                for(; it != bars.end(); it++) {
                    if ((*it) -> isVisible() && view(*it)) {
                        activate(played = (*it));
                        break;
                    }
                }
            }
        }
    }
}

void Dockbars::showViewSettingsDialog(DockBar * bar) {
    TabDialog dialog(parentWidget());
    if (bar) {
        IView * view = dynamic_cast<IView *>(bar -> widget());

        if (!view -> isEditable()) {
            QMessageBox::warning(this, "Settings", "This view type is not editable ...");
            return;
        }

        dialog.setSettings(view -> settings());
        dialog.setName(bar -> windowTitle());

        if (dialog.exec() == QDialog::Accepted) {
            bar -> setWindowTitle(dialog.getName());
            view -> setSettings(dialog.getSettings());
        }
    } else {
        if (dialog.exec() == QDialog::Accepted) {
            ((QMainWindow *)parentWidget()) -> addDockWidget(
                Qt::TopDockWidgetArea,
                createDocBar(dialog.getName(), dialog.getSettings())
            );
        }
    }
}

//A tabified dockwidget can be set as the selected tab like this:
//dockwidget.raise()


//// when the floating property of dockWidget is changed from docked to floating
//// we make it a top level window (with minmize, maximize, and close button in the title bar)
//// by calling setWindowFlags(Qt::Window)
//// The dockWidget will automatically regain it's Qt::widget flag when it becomes docked again (by dragging it to the right place or double clicking the title bar)
//void CMainWindow::dockWidget_topLevelChanged(bool isFloating)
//{
//if(isFloating)
//{ ui.dockWidget->setWindowFlags(Qt::Window);
//// setWindowFlags calls setParent() when changing the flags for a window, causing the widget to be hidden.
//// You must call show() to make the widget visible again
//ui.dockWidget->show();
//}
//}

void Dockbars::updateActiveTabIcon() {
    if (!played) return;

    TabifyParams tabData = played -> tabIndex();

    if (tabData.index != -1) {
        tabData.tabbar -> setTabIcon(tabData.index, QIcon(":played_tab"));
        tabData.tabbar -> setIconSize(QSize(14, 14));

        if (tabData == lastTabData) return;

        if (lastTabData.index != -1)
            lastTabData.tabbar -> setTabIcon(lastTabData.index, QIcon());

        lastTabData = tabData;
    }
}

void Dockbars::updateAllViews() { // update for item height
    QList<IView *> views = parent() -> findChildren<IView *>();
    int iconDimension = Settings::instance() -> iconHeight();

    QList<IView *>::Iterator it = views.begin();
    for(; it != views.end(); it++)
        (*it) -> setIconSize(QSize(iconDimension, iconDimension));
}

void Dockbars::onNextItemNeeded(Player::Reason reason) {
    initPlayed();
    IView * v = view(played);

    if (v) {
        if (reason == Player::refreshNeed && v -> isRequiredOnUpdate()) {
            ((IModel *)v -> model()) -> refresh(true);
            return;
        }

        if (reason == Player::init || (reason == Player::endMedia && v -> isPlaylist()))
            v -> execNextIndex();
    }
}

void Dockbars::nextExecTriggering() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execNextIndex();
}
void Dockbars::nextExecWithDelTriggering() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execNextIndex(true);
}
void Dockbars::prevExecWithDelTriggering() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execPrevIndex(true);
}
void Dockbars::prevExecTriggering() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execPrevIndex();
}

void Dockbars::barClosed() {
    DockBar * bar = (DockBar *)sender();

    if (active == bar)
        active = 0;

    IView * v = view(bar);

    if (v && Player::instance() -> playedIndex().model() == v -> model())
        Player::instance() -> playIndex(QModelIndex());
}
