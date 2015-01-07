#include "dockbars.h"

using namespace Playo3;

Dockbars *Dockbars::self = 0;

Dockbars *Dockbars::instance(QWidget * parent) {
    if(!self)
        self = new Dockbars(parent);
    return self;
}

void Dockbars::load(QMainWindow * window, QJsonArray & bars) {
//    if (bars.count() > 0) {
//        QList<QString> barsList;
//        barsList.append("Media"); barsList.append("Media+"); barsList.append("Media+Position"); barsList.append("Media+Time");
//        barsList.append("Media+Volume"); barsList.append("Controls"); barsList.append("Spectrum");

//        QJsonObject obj, actionObj;
//        QString barName;
//        QToolBar * curr_bar;

//        foreach(QJsonValue bar, bars) {
//            obj = bar.toObject();
//            barName = obj.value("title").toString();
//            barsList.removeOne(barName);
//            curr_bar = linkNameToToolbars(barName);
//            curr_bar -> setMovable(obj.value("movable").toBool());

//            window -> addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);

//            if (obj.contains("actions")) {
//                QJsonArray actions = obj.value("actions").toArray();

//                foreach(QJsonValue act, actions) {
//                    actionObj = act.toObject();
//                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
//                }
//            }
//        }

//        recreateToolbars(window, barsList);
//    } else {
//        createToolbars(window);
//    }
}

void Dockbars::save(QMainWindow * window, DataStore * settings) {
//    QList<QToolBar *> toolbars = window -> findChildren<QToolBar *>();
//    qDebug() << toolbars.length();

//    if (toolbars.length() > 0) {
//        QJsonArray toolbar_array = QJsonArray();
//        QJsonObject curr_tab;
//        QList<QAction*> actions;
//        ToolbarButton* button;

//        foreach(QToolBar * bar, toolbars) {
//            curr_tab = QJsonObject();

//            curr_tab.insert("area", window -> toolBarArea(bar));
//            curr_tab.insert("title", bar -> windowTitle());
//            curr_tab.insert("movable", bar -> isMovable());

//            if (bar -> windowTitle() != "Media"
//                    && bar -> windowTitle() != "Media+"
//                    && bar -> windowTitle() != "Media+Position"
//                    && bar -> windowTitle() != "Media+Time"
//                    && bar -> windowTitle() != "Media+Volume"
//                    && bar -> windowTitle() != "Controls"
//                    && bar -> windowTitle() != "Spectrum"
//               ) {
//                actions = bar -> actions();
//                if (actions.length() > 0) {
//                    QJsonArray action_array = QJsonArray();
//                    QJsonObject curr_act;

//                    foreach(QAction * act, actions) {
//                        if (QString(act -> metaObject() -> className()) == "QWidgetAction") {
//                            curr_act = QJsonObject();
//                            button = (ToolbarButton*) bar -> widgetForAction(act);

//                            curr_act.insert("path", button -> path);
//                            curr_act.insert("name", button -> text());
//                        }
//                        action_array.append(curr_act);
//                    }

//                    if (action_array.count() > 0)
//                        curr_tab.insert("actions", action_array);
//                }
//            }

//            toolbar_array.append(curr_tab);

//    //        bar -> toolButtonStyle();
//        }

//        settings -> write(Dockbars::settingsName(), toolbar_array);
//    }
}

QDockWidget * Dockbars::createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place) {
    QDockWidget * dock = new QDockWidget(name, (QWidget *)parent());
    dock -> setWindowFlags(Qt::WindowMinMaxButtonsHint);
    dock -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, ((QWidget *)parent())));
//    dock -> showFullScreen();
    dock -> setWidget(content);
    dock -> layout() -> setAlignment(content, Qt::AlignCenter);
    ((QMainWindow *)parent()) -> addDockWidget(place, dock);
    //    viewMenu->addAction(dock->toggleViewAction());
    return dock;
//    ((QWidget *)parent())->tabifyDockWidget(dockWidget1,dockWidget2);
}

//A tabified dockwidget can be set as the selected tab like this:
//dockwidget.raise()

// icons
//QStyle::SP_TitleBarNormalButton
//QStyle::SP_TitleBarMinButton
//QStyle::SP_TitleBarMaxButton
//QStyle::SP_DockWidgetCloseButton


//// connect dockWidget's topLevelChanged signal, which is emitted when its floating property changes, to a user-defined slot
//connect(ui.dockWidget, SIGNAL(topLevelChanged(bool)), this, SLOT(dockWidget_topLevelChanged(bool)));

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

