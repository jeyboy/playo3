#include "dockbars.h"

using namespace Presentation;
using namespace Dialogs;
using namespace Data;

Dockbars * Dockbars::self = 0;
Dockbars * Dockbars::instance(QWidget * parent) {
    if(!self)
        self = new Dockbars(parent);
    return self;
}

void Dockbars::load(const QJsonArray & bars) {
    int userTabsAmount = 0;
    MainWindow * window = (MainWindow *)parent();
    QList<QString> barsList;
    barsList << DOWNLOADS_TAB << LOGS_TAB /*<< SCREEN_TAB*/;

    if (bars.count() > 0) {
        QJsonObject obj;
        QString barName;
        QDockWidget * curr_bar;

        for(QJsonArray::ConstIterator it = bars.constBegin(); it != bars.constEnd(); it++) {
            obj = (*it).toObject();
            barName = obj.value(QStringLiteral("title")).toString();
            userTabsAmount += (!barsList.removeOne(barName));
            curr_bar = linkNameToToolbars(barName, View::Params(obj.value(QStringLiteral("set")).toObject()), obj.value(QStringLiteral("cont")).toObject());
            curr_bar -> setObjectName(obj.value(QStringLiteral("name")).toString(curr_bar -> objectName()));

            if (obj.value(QStringLiteral("stick")).toBool())
                ((DockBar *)curr_bar) -> markAsSticked();

            QString link = obj.value(QStringLiteral("link")).toString();
            if (!link.isEmpty())
                linkedTabs.insert(link, (DockBar *)curr_bar);

            ((DockBar *)curr_bar) -> useVerticalTitles(obj.value(QStringLiteral("vertical")).toBool());

            window -> addDockWidget(Qt::TopDockWidgetArea, curr_bar);

            if (obj.value(QStringLiteral("played")).toBool()) {
                IView * v = view(qobject_cast<DockBar *>(curr_bar));
                if (v) {
                    QString path = obj.value(QStringLiteral("played_item")).toString();
                    if (!path.isEmpty())
                        v -> execPath(path, true, obj.value(QStringLiteral("played_time")).toInt(), obj.value(QStringLiteral("played_duration")).toInt());
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
    View::Params defSettings;
    while(barsList.length() > 0) {
        QDockWidget * widg = linkNameToToolbars(barsList.takeFirst(), defSettings, def);
        widg -> hide();
        window -> addDockWidget(Qt::TopDockWidgetArea, widg);
    }
}

void Dockbars::save(DataStore * settings) {
    QList<DockBar *> bars = dockbars();

    if (bars.length() > 0) {
        QJsonArray bar_array = QJsonArray();
        QJsonObject curr_bar;

        QList<DockBar *>::Iterator it = bars.begin();

        for(; it != bars.end(); it++) {
            IView * v = view((*it));

            if ((*it) -> windowTitle() == COMMON_TAB) {
                if (v && v -> isCommon() && !Settings::instance() -> isSaveCommonTab())
                    continue;
            }

            curr_bar = QJsonObject();
            curr_bar.insert(QStringLiteral("title"), (*it) -> windowTitle());
            curr_bar.insert(QStringLiteral("name"), (*it) -> objectName());
            curr_bar.insert(QStringLiteral("stick"), (*it) -> isSticked());
            curr_bar.insert(QStringLiteral("vertical"), (*it) -> isUsedVerticalTitles());

            QString path = linkedTabs.key((*it), QString());
            if (!path.isEmpty())
                curr_bar.insert(QStringLiteral("link"), path);

            if ((*it) -> windowTitle() == LOGS_TAB) {
                //do nothing
            } else if ((*it) -> windowTitle() == DOWNLOADS_TAB) {
                curr_bar.insert(QStringLiteral("cont"), ((DownloadView *)(*it) -> mainWidget()) -> toJson());
            } else {
                if ((*it) == played) {
                    curr_bar.insert(QStringLiteral("played"), true);
                    if (Player::instance() -> playedIndex().isValid()) {
                        curr_bar.insert(QStringLiteral("played_item"), Player::instance() -> playedIndex().data(ITREEPATH).toString());
                        curr_bar.insert(QStringLiteral("played_time"), Player::instance() -> getPosition());
                        curr_bar.insert(QStringLiteral("played_duration"), Player::instance() -> getDuration());
                    }
                }

                if (v) {
                    curr_bar.insert(QStringLiteral("set"), v -> settings().toJson());
                    curr_bar.insert(QStringLiteral("cont"), v -> toJson());
                }
            }

            bar_array.append(curr_bar);
        }

        settings -> write(Dockbars::settingsName(), bar_array);
    }
}

QDockWidget * Dockbars::linkNameToToolbars(QString barName, View::Params settings, QJsonObject attrs) {
    if (barName == SCREEN_TAB) {
        return 0; // stub
    } else if (barName == COMMON_TAB) {
        return createDocBar(barName, settings, &attrs, false);
    } else if (barName == DOWNLOADS_TAB) {
        DockBar * bar = createDocBar(barName, false);
        bar -> setWidget(DownloadView::instance(&attrs, parentWidget()));
        connect(DownloadView::instance(), SIGNAL(downloadProceeded(QString)), this, SLOT(onDownloadProceeded(QString)));
        return bar;
    } else if (barName == LOGS_TAB) {
        DockBar * bar = createDocBar(barName, false);
        bar -> setWidget(Logger::instance() -> getEditor());
        return bar;
    } else return createDocBar(barName, settings, &attrs);
}
View::Params defSettings(level, true, false, false, true);

DockBar * Dockbars::commonBar() {
    if (!common) {
        common = createDocBar(COMMON_TAB, defSettings, 0, false);
    }

    return common;
}

DockBar * Dockbars::createLinkedDocBar(QString text, QString path, View::Params settings) {
    DockBar * bar = linkedTabs.value(path, 0);

    if (!bar) {
        DockBar * bar = createDocBar(text, settings, 0, true, true);
        QList<QUrl> urls;
        urls << QUrl::fromLocalFile(path.mid(0, path.length() - 1));// remove backslash
        view(bar) -> appendRows(urls);
        linkedTabs.insert(path, bar);
    }
    else activate(bar);

    return bar;
}

DockBar * Dockbars::createDocBar(QString name, View::Params settings, QJsonObject * attrs, bool closable, bool addToView, SearchSettings * search_settings) {
    DockBar * bar = createDocBar(name, closable);
    IView * view = ViewFactory::build(bar, settings, attrs);

    bar -> setWidget(view);
    bar -> initiateSearch();

    if (!attrs) {
        if (settings.type != search)
            ((IModel *)view -> model()) -> refresh();
        else
            ((SearchView *)view) -> search(*search_settings);
    }

    if (addToView)
        ((QMainWindow *)parent()) -> addDockWidget(Qt::TopDockWidgetArea, bar);

    return bar;
}

DockBar * Dockbars::createDocBar(QString name, bool closable, QWidget * content) {
    DockBar * dock = new DockBar(name, (QWidget *)parent(), closable, Qt::WindowMinMaxButtonsHint);

    connect(dock, SIGNAL(closing()), this, SLOT(barClosed()));
    connect(dock, SIGNAL(topLevelChanged(bool)), this, SLOT(updateActiveTabIcon(bool)));
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
            QMessageBox::warning(this, QStringLiteral("Settings"), QStringLiteral("This view type is not editable ..."));
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

void Dockbars::updateActiveTabIcon(bool isFloating) {
    if (isFloating) return;
    if (!played) return;

    if (lastTabData.index != -1)
        lastTabData.tabbar -> setTabIcon(lastTabData.index, QIcon());

    TabifyParams tabData = played -> tabIndex();

    if (tabData.index != -1) {
        tabData.tabbar -> setTabIcon(tabData.index, QIcon(QStringLiteral(":played_tab")));
        tabData.tabbar -> setIconSize(QSize(14, 14));
    }

    lastTabData = tabData;

//    TabifyParams tabData = played -> tabIndex();

//    if (tabData.index != -1) {
//        tabData.tabbar -> setTabIcon(tabData.index, QIcon(QStringLiteral(":played_tab")));
//        tabData.tabbar -> setIconSize(QSize(14, 14));
//    }

//    if (tabData == lastTabData) return;

//    if (lastTabData.index != -1)
//        lastTabData.tabbar -> setTabIcon(lastTabData.index, QIcon());

//    lastTabData = tabData;
}

void Dockbars::updateAllViews() { // update for item height
    QList<IView *> views = parent() -> findChildren<IView *>();
    int iconDimension = Settings::instance() -> iconHeight();

    QList<IView *>::Iterator it = views.begin();
    for(; it != views.end(); it++) {
        (*it) -> setUniformRowHeights(Settings::instance() -> isHeightUnificate());
        (*it) -> setIconSize(QSize(iconDimension, iconDimension));
    }
}

void Dockbars::onNextItemNeeded(Player::Reason reason) {
    initPlayed();
    IView * v = view(played);

    if (v) {
        if (reason == Player::init || (reason == Player::endMedia && v -> isPlaylist())) {
            v -> execNextIndex();
            return;
        }

        if (reason == Player::refreshNeed) {
            if (v -> isRequiredOnUpdate()) {
                ((IModel *)v -> model()) -> refresh(true);
            } else {
                if (IModel::restoreUrl(Player::instance() -> playedItem()))
                    Player::instance() -> playIndex(Player::instance() -> playedIndex());
                else {
                    Player::instance() -> stop();
                    Player::instance() -> playedIndexIsNotExist();
                }
            }

            return;
        }

        if (Settings::instance() -> isFindValid())
            v -> execNextIndex();
    }
}

void Dockbars::playNext() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execNextIndex();
}
void Dockbars::playNextWithDel() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execNextIndex(true);
}
void Dockbars::playPrevWithDel() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execPrevIndex(true);
}
void Dockbars::playPrev() {
    initPlayed();
    IView * v = view(played);
    if (v) v -> execPrevIndex();
}

void Dockbars::stop() { Player::instance() -> stop(); }
void Dockbars::playPause() { Player::instance() -> playPause(); }
void Dockbars::slidePosForward() { Player::instance() -> slidePosForward(); }
void Dockbars::slidePosBackward() { Player::instance() -> slidePosBackward(); }
void Dockbars::slideVolForward() { Player::instance() -> slideVolForward(); }
void Dockbars::slideVolBackward() { Player::instance() -> slideVolBackward(); }

void Dockbars::onDownloadProceeded(QString to) {
    for(QHash<QString, DockBar *>::Iterator it = linkedTabs.begin(); it != linkedTabs.end(); it++) {
        if (to.startsWith(it.key())) {
            QList<QUrl> urls;
            urls.append(QUrl::fromLocalFile(to));
            view(it.value()) -> appendRows(urls);
            return;
        }
    }
}

void Dockbars::barClosed() {
    DockBar * bar = (DockBar *)sender();

    linkedTabs.remove(linkedTabs.key(bar));

    if (active == bar) active = 0;
    if (played == bar) played = 0;

    IView * v = view(bar);

    if (v && Player::instance() -> playedIndex().model() == v -> model())
        Player::instance() -> playIndex(QModelIndex());
}
