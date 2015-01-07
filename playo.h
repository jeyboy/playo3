#ifndef PLAYO_H
#define PLAYO_H

#include <qdebug.h>
#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "toolbars.h"
#include "dockbars.h"

#include <QSettings>

#include "modules/json/data_store.h"

#include "dialogs/tabdialog.h"
//#include "dialogs/web_dialog.h"
#include "dialogs/settings_dialog.h"
//#include "dialogs/relationdialog.h"


using namespace Playo3;

namespace Ui {
    class Playo;
}

class Playo : public Playo3::MainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget *parent = 0);
    ~Playo();

protected:
    void closeEvent(QCloseEvent *);
    void init();
private:
    Ui::Playo *ui;

    DataStore * settings;
};

#endif // PLAYO_H
