#ifndef TABBER_H
#define TABBER_H

#include <QTabWidget>
//#include <QHash>

//#include "../json/data_store.h"
//#include "dialogs/tabdialog.h"

//#include "tab.h"
//#include "tab_settings.h"

namespace Playo {
    class Tabber : public QTabWidget {
        Q_OBJECT
    public:
        explicit Tabber(QWidget *parent = 0);
        ~Tabber();

//        int addTab(QString name, TabSettings settings);
//        Tab * toActiveTab();
//        Tab * currentTab();
//        Tab * commonTab();
//        void load();
//        void save();

//        void updateIconSize();

//    private:
//        Tab * commonPlaylist;
//        Json::DataStore * store;

//    private slots:
//        void updateActiveTab(QWidget *, QWidget *);
//        void handleCurrentChanged(int index);
//        void handleTabCloseRequested(int index);
    };
}

#endif // TABBER_H
