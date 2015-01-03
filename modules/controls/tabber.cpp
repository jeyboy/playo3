#include "tabber.h"
#include "misc/stylesheets.h"
#include <QApplication>
#include <QMessageBox>

using namespace Playo3;

Tabber::Tabber(QWidget *parent) : QTabWidget(parent) {
//    commonPlaylist = 0;

//    tabber -> setTabBar(0); // TODO: custom tab bar
//    setTabPosition((QTabWidget::TabPosition)Settings::instance() -> getTabPosition());
//    setUsesScrollButtons(Settings::instance() -> getScrollButtonUsage());

    setStyleSheet(Stylesheets::treeViewStyles());

//    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseRequested(int)));
//    connect(this, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
//    connect(Player::instance(), SIGNAL(playlistChanged(QWidget *, QWidget *)), this, SLOT(updateActiveTab(QWidget *, QWidget *)));

//    store = new DataStore("tabs.json");
}

Tabber::~Tabber() {
//    delete store;
}

//int Tabber::addTab(QString name, void * settings) {
//    int index = QTabWidget::addTab(new Tab(settings, this), name);
//    (static_cast<Tab *>(widget(index))) -> updateHeader();
//    setCurrentIndex(index);
//    return index;
//}

//Tab * Tabber::toActiveTab() {
//    if (Player::instance() -> currentPlaylist()) {
//        Tab * tab = (Tab *)Player::instance() -> currentPlaylist() -> parentWidget();
//        setCurrentIndex(indexOf(tab));
//        return tab;
//    } else return 0;
//}

//Tab * Tabber::currentTab() {
//    return static_cast<Tab *>(currentWidget());
//}

//Tab * Tabber::commonTab() {
//    if (commonPlaylist == 0) {
//        TabDialog dialog;
//        CBHash settings = dialog.getSettings(); // get default settings
//        settings.insert("p", 1); // is playlist
//        settings.insert("t", 2); // is one level tree
//        settings.insert("c", 1); // is common

//        addTab("Common", settings);
//        commonPlaylist = currentTab();
//    } else {
//        setCurrentIndex(indexOf(commonPlaylist));
//    }

//    return commonPlaylist;
//}

//void Tabber::load() {
//    if (store -> state) {
//        QJsonObject tab;

//        foreach(QString key, store -> keys()) {
//            tab = store -> read(key).toObject();
//            Tab * new_tab = new Tab(tab, this);

//            if (new_tab -> getView() -> isCommon())
//                commonPlaylist = new_tab;

//            QTabWidget::addTab(new_tab, tab["n"].toString());
//            new_tab -> updateHeader();

//            if (tab.contains("pv")) {
//                new_tab -> getView() -> execItem(new_tab -> getView() -> fromPath(tab.value("pp").toString()), true);
//                if (tab.contains("pt")) {
//                    Player::instance() -> setStartPosition(tab.value("pt").toInt());
////                  Player::instance() -> play();
//                }
//            }
//        }
//    }
//}

//void Tabber::save() {
//    Player::instance() -> pause();
//    store -> clear();
//    Tab * tab;
//    for(int i = 0; i < count(); i++) {
//        tab = (Tab*)(widget(i));
//        if (tab == commonPlaylist) {
//            // logic for common playlist // at this time common list did not save
//        } else {
//            store -> write(QString::number(i) + "h", tab -> toJSON(tab -> getName()));
//        }
//    }

//    store -> save();
//    Player::instance() -> stop();
//}



//void Tabber::updateIconSize() {
//    Tab * tab;
//    int dimension = Settings::instance() -> getIconHeight();
//    QSize size(dimension, dimension);
//    for(int i = 0; i < count(); i++) {
//        tab = (Tab*)(widget(i));
//        tab -> getView() -> setIconSize(size);
//    }
//}

//void Tabber::updateActiveTab(QWidget * last, QWidget * current) {
//    int index;

//    if (last != 0) {
//        index = indexOf(last -> parentWidget());
//        setTabIcon(index, QIcon());
//    }

//    if (current != 0) {
//        index = indexOf(current -> parentWidget());
//        setTabIcon(index, QIcon(":/active_tab"));
//    }
//}

//void Tabber::handleCurrentChanged(int index) {
//    if (index == -1) {
//        Player::instance() -> setActivePlaylist(0);
//    } else {
//        Tab * new_tab = static_cast<Tab *>(widget(index));

//        Player::instance() -> setActivePlaylist(const_cast<View *>(new_tab -> getView()));
//    }
//}

//void Tabber::handleTabCloseRequested(int index) {
//    Tab * del_tab = static_cast<Tab *>(widget(index));

//    if (del_tab -> getView() -> getModel() -> itemsCount() > 0)
//        if (QMessageBox::warning(this, "Tab deletion", "Tab is not empty. Are you sure?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) != QMessageBox::Ok)
//            return;

//    if (del_tab == commonPlaylist)
//        commonPlaylist = 0;

//    if (Player::instance() -> currentPlaylist() == del_tab -> getView()) {
//        Player::instance() -> removePlaylist();
//    }

//    if (Player::instance() -> currentActivePlaylist() == del_tab -> getView()) {
//        Player::instance() -> setActivePlaylist(0);
//    }

//    removeTab(index);
//}

