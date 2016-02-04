#include "dockbars.h"
#include "modules/controls/echonest_widget.h"

using namespace Presentation;
using namespace Dialogs;
using namespace Data;

void Dockbars::load(const QJsonArray & bars) {
    int userTabsAmount = 0;
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

            curr_bar = linkNameToToolbars(
                BarCreationNames(barName, obj.value(QStringLiteral("link")).toString(), obj.value(QStringLiteral("name")).toString()),
                Views::Params(obj.value(QStringLiteral("set")).toObject()),
                obj.value(QStringLiteral("cont")).toObject()
            );

            if (obj.value(QStringLiteral("stick")).toBool())
                ((DockBar *)curr_bar) -> markAsSticked();

            ((DockBar *)curr_bar) -> useVerticalTitles(obj.value(QStringLiteral("vertical")).toBool());

            container -> addDockWidget(Qt::TopDockWidgetArea, curr_bar);

            IView * v = view(qobject_cast<DockBar *>(curr_bar));

            if (obj.value(QStringLiteral("selection")).isString())
                v -> restoreSelection(obj.value(QStringLiteral("selection")).toString());

            if (obj.value(QStringLiteral("played")).toBool()) {
                if (v) {
                    QString path = obj.value(QStringLiteral("played_item")).toString();
                    if (!path.isEmpty())
                        v -> execPath(path, PlayerInitState::initiated, obj.value(QStringLiteral("played_time")).toInt());
                }
            }
        }
    }

    if (!Settings::obj().isSaveCommonTab()) {
        container -> addDockWidget(Qt::TopDockWidgetArea, commonBar());
        if (userTabsAmount != 0)
            commonBar() -> hide();
    }

    QJsonObject def;
    Views::Params defSettings;
    while(barsList.length() > 0) {
        QDockWidget * widg = linkNameToToolbars(barsList.takeFirst(), defSettings, def);
        widg -> hide();
        container -> addDockWidget(Qt::TopDockWidgetArea, widg);
    }
}

void Dockbars::save(DataStore * settings) {
    QList<DockBar *> bars = dockbars();

    if (bars.length() > 0) {
        QJsonArray bar_array = QJsonArray();
        QJsonObject curr_bar;

        for(QList<DockBar *>::Iterator it = bars.begin(); it != bars.end(); it++) {
            if ((*it) -> windowTitle() == ECHONEST_TAB && (*it) -> isHidden()) continue;

            IView * v = view((*it));

            if ((*it) -> windowTitle() == COMMON_TAB) {
                if (v && v -> isCommon() && !Settings::obj().isSaveCommonTab())
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

            if ((*it) -> windowTitle() == LOGS_TAB || (*it) -> windowTitle() == ECHONEST_TAB) {
                //do nothing
            } else if ((*it) -> windowTitle() == DOWNLOADS_TAB) {
                curr_bar.insert(QStringLiteral("cont"), ((DownloadView *)(*it) -> mainWidget()) -> toJson());
            } else {
                if ((*it) == played) {
                    curr_bar.insert(QStringLiteral("played"), true);
                    if (DataFactory::obj().playedIndex().isValid()) {
                        curr_bar.insert(QStringLiteral("played_item"), DataFactory::obj().playedItem() -> buildTreePath());
                        curr_bar.insert(QStringLiteral("played_time"), QJsonValue::fromVariant(DataFactory::obj().currPlayer() -> position()));
//                        curr_bar.insert(QStringLiteral("played_duration"), QJsonValue::fromVariant(DataFactory::obj().currPlayer() -> duration()));
                    }
                }

                if (v) {
                    curr_bar.insert(QStringLiteral("set"), v -> settings().toJson());
                    curr_bar.insert(QStringLiteral("cont"), v -> toJson());

                    if (v -> currentIndex().isValid())
                        curr_bar.insert(QStringLiteral("selection"), v -> currentIndex().data(ITREEPATH).toString());
                }
            }

            if (!v || (v && v -> isSearch()))
                bar_array.append(curr_bar);
            else
                bar_array.prepend(curr_bar);
        }

        settings -> write(Dockbars::settingsName(), bar_array);
    }
}

QDockWidget * Dockbars::linkNameToToolbars(const BarCreationNames & names, const Views::Params & settings, QJsonObject attrs) {
    if (names.is(SCREEN_TAB)) {
        return 0; // stub
    } else if (names.is(COMMON_TAB)) {
        return createDocBar(names, settings, &attrs, false);
    } else if (names.is(DOWNLOADS_TAB)) {
        DockBar * bar = createDocBar(names, false);
        bar -> setWidget(&DownloadView::linked_obj_with_init(&attrs, container));
        connect(&DownloadView::obj(), SIGNAL(downloadProceeded(QString)), this, SLOT(onDownloadProceeded(QString)));
        return bar;
    } else if (names.is(LOGS_TAB)) {
        DockBar * bar = createDocBar(names, false);
        bar -> setWidget(Logger::obj().getEditor());
        return bar;
    } else if (names.is(ECHONEST_TAB)) {
        return echonestBar();
    } else {
        if (names.linkable_uid.isEmpty())
            return createDocBar(names, settings, &attrs);
        else
            return createLinkedDocBar(names, settings, &attrs);
    }
}

Views::Params defSettings(level, true, false, false, true);

DockBar * Dockbars::commonBar() {
    if (!common) common = createDocBar(BarCreationNames(COMMON_TAB), defSettings, 0, false);
    return common;
}

DockBar * Dockbars::echonestBar() {
    if (!echonest) {
        echonest = createDocBar(BarCreationNames(ECHONEST_TAB), false, new EchonestWidget(this));
        container -> addDockWidget(Qt::TopDockWidgetArea, echonest);
    }

    return echonest;
}

DockBar * Dockbars::createLinkedDocBar(const BarCreationNames & names, const Views::Params & settings, QJsonObject * attrs,
        bool closable, bool addToView, SearchSettings * search_settings)
{
    bool with_head = names.linkable_uid.startsWith(UID_HEAD);
    QString identifier = with_head ? names.linkable_uid : UID_HEAD % names.linkable_uid;

    DockBar * bar = linkedTabs.value(identifier, 0);

    if (!bar) {
        bar = createDocBar(names, settings, attrs, closable, addToView, search_settings);
        if (!with_head) {
            QList<QUrl> urls;
            urls << QUrl::fromLocalFile(names.linkable_uid.mid(0, names.linkable_uid.length() - 1));// remove backslash and head symbol
            view(bar) -> appendRows(urls);
        }
        linkedTabs.insert(identifier, bar);
    }
    else activate(bar);

    return bar;
}

DockBar * Dockbars::createDocBar(const BarCreationNames & names, const Views::Params & settings, QJsonObject * attrs, bool closable, bool addToView, SearchSettings * search_settings) {
    DockBar * bar = createDocBar(names, closable);
    IView * view = ViewFactory::build(bar, settings, attrs);

    bar -> setWidget(view);
    bar -> initiateSearch();

    if (!attrs) {
        if (settings.type != Data::Type::search)
            ((IModel *)view -> model()) -> refresh();
        else
            ((SearchView *)view) -> search(*search_settings);
    }

    if (addToView)
        container -> addDockWidget(Qt::TopDockWidgetArea, bar);

    return bar;
}

DockBar * Dockbars::createDocBar(const BarCreationNames & names, bool closable, QWidget * content) {
    DockBar * dock = new DockBar(names.name, container, closable, Qt::WindowMinMaxButtonsHint, names.innerName);

    connect(dock, SIGNAL(closing()), this, SLOT(barClosed()));
    connect(dock, SIGNAL(topLevelChanged(bool)), this, SLOT(updateActiveTabIcon(bool)));
  //    dock -> showFullScreen();

    if (content) {
        content -> setParent(dock);
        dock -> setWidget(content);
    }

    return dock;
//    container->tabifyDockWidget(dockWidget1,dockWidget2);
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
    TabDialog dialog(container);
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
            container -> addDockWidget(
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
    QList<IView *> views = container -> findChildren<IView *>();
    int iconDimension = Settings::obj().iconHeight();

    QList<IView *>::Iterator it = views.begin();
    for(; it != views.end(); it++) {
        (*it) -> setUniformRowHeights(Settings::obj().isHeightUnificate());
        (*it) -> setIconSize(QSize(iconDimension, iconDimension));
    }
}

void Dockbars::scrollToActive() {
    if (played) {
        IView * v = view(played);

        if (!v) {
            qDebug() << "VIEW IS NOT FINDED FOR PLAYLIST";
            return;
        }

        activate(played);
        v -> scrollToActive();
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

void Dockbars::stop()               { DataFactory::obj().currPlayer() -> stop(); }
void Dockbars::playPause()          { DataFactory::obj().currPlayer() -> playPause(); }
void Dockbars::slidePosForward()    { DataFactory::obj().currPlayer() -> slidePosForward(); }
void Dockbars::slidePosBackward()   { DataFactory::obj().currPlayer() -> slidePosBackward(); }
void Dockbars::slideVolForward()    { DataFactory::obj().currPlayer() -> slideVolForward(); }
void Dockbars::slideVolBackward()   { DataFactory::obj().currPlayer() -> slideVolBackward(); }

void Dockbars::onDownloadProceeded(QString to) {
    QString key = UID_HEAD % to;
    for(QHash<QString, DockBar *>::Iterator it = linkedTabs.begin(); it != linkedTabs.end(); it++) {
        if (key.startsWith(it.key())) {
            QList<QUrl> urls;
            urls.append(QUrl::fromLocalFile(to));
            view(it.value()) -> appendRows(urls);
            return;
        }
    }
}

void Dockbars::barClosed() {
    DockBar * bar = (DockBar *)sender();

    linkedTabs.remove(linkedTabs.key(bar)); // TODO: need to recheck this

    if (active == bar) active = 0;
    if (played == bar) played = 0;

    IView * v = view(bar);

    if (v && DataFactory::obj().currentPlaylist() == v)
        DataFactory::obj().resetPlaying();
}
