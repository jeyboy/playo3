#include "dockbars.h"
#include <qdebug.h>

using namespace Playo3;

Dockbars *Dockbars::self = 0;

Dockbars *Dockbars::instance(QWidget * parent) {
    if(!self)
        self = new Dockbars(parent);
    return self;
}

void Dockbars::load(QJsonArray & bars) {
    QMainWindow * window = (QMainWindow *)parent();
    QList<QString> barsList;
//    barsList.append("Screen");

    if (bars.count() > 0) {
        QJsonObject obj;
        QString barName;
        QDockWidget * curr_bar;

        foreach(QJsonValue bar, bars) {
            obj = bar.toObject();
            barName = obj.value("title").toString();
            barsList.removeOne(barName);
            curr_bar = linkNameToToolbars(barName);
            curr_bar -> setObjectName(obj.value("name").toString(curr_bar -> objectName()));

            if (obj.value("stick").toBool())
                ((DockBar *)curr_bar) -> markAsSticked();

            window -> addDockWidget(Qt::TopDockWidgetArea, curr_bar);
//            window -> addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);

//            if (obj.contains("actions")) {
//                QJsonArray actions = obj.value("actions").toArray();

//                foreach(QJsonValue act, actions) {
//                    actionObj = act.toObject();
//                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
//                }
//            }
        }
    } else {
        // Do something if we did not have any bars
    }

    while(barsList.length() > 0)
        window -> addDockWidget(Qt::TopDockWidgetArea, linkNameToToolbars(barsList.takeFirst()));
}

void Dockbars::save(DataStore * settings) {
    QList<DockBar *> bars = dockbars();

    if (bars.length() > 0) {
        QJsonArray bar_array = QJsonArray();
        QJsonObject curr_bar;

        foreach(DockBar * bar, bars) {
            curr_bar = QJsonObject();
            curr_bar.insert("title", bar -> windowTitle());
            curr_bar.insert("name", bar -> objectName());
            curr_bar.insert("stick", bar -> isSticked());

            // save tab content

            bar_array.append(curr_bar);
        }

        settings -> write(Dockbars::settingsName(), bar_array);
    }
}

QDockWidget * Dockbars::linkNameToToolbars(QString barName) {
    if (barName == "Screen") {
        return 0; // stub
    } else {
        return createDocBar(barName, 0);
    }
}

DockBar * Dockbars::createDocBar(QString name, QWidget * content) {
    DockBar * dock = new DockBar(name, (QWidget *)parent(), Qt::WindowMinMaxButtonsHint);
    connect(dock, SIGNAL(activating()), this, SLOT(activeChanged()));
    connect(dock, SIGNAL(closing()), this, SLOT(barClosed()));
//    active = dock;

//    dock -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, ((QWidget *)parent())));
//    dock -> showFullScreen();
    if (content) {
        content -> setParent(dock);
        dock -> setWidget(content);
        dock -> layout() -> setAlignment(content, Qt::AlignCenter);
    }

    return dock;
//    ((QWidget *)parent())->tabifyDockWidget(dockWidget1,dockWidget2);
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

void Dockbars::hideAll() {
    foreach(DockBar * bar, dockbars())
        bar -> setHidden(true);
}
void Dockbars::showAll() {
    foreach(DockBar * bar, dockbars())
        bar -> setHidden(false);
}

void Dockbars::activeChanged() {
    active = (DockBar *)sender();
}

void Dockbars::barClosed() {
    if (active == (DockBar *)sender())
        active = 0;
}
