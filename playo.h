#ifndef PLAYO_H
#define PLAYO_H

#include <qdebug.h>
#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "modules/data_struct/view_index.h"

#include "toolbars.h"
#include "dockbars.h"

#include <QSettings>

#include "misc/file_utils/data_store.h"

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

public slots:
//    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);
    void showAttCurrTabDialog();

private slots:
//    void showError(QString);

    void showActiveElem();
    void showSettingsDialog();

//    void showVKRelTabDialog();
//    void showSoundcloudRelTabDialog();

//    void openVKTabDialog();
//    void showVKTabDialog();
//    void openSoundcloudTabDialog();
//    void showSoundcloudTabDialog();

    void showAttTabDialog(DockBar * bar = 0);
    void openFolderTriggered();

    void nextItemTriggered();
    void nextItemWithDelTriggered();
    void prevItemTriggered();

//    void dropEvent(QDropEvent * event);


protected:
//    void putToCommonTab(QList<QUrl> urls);
//    void dragEnterEvent(QDragEnterEvent *);
//    void dragMoveEvent(QDragMoveEvent *);
//    void dropEvent(QDropEvent *);
    QMenu * createPopupMenu();
    void closeEvent(QCloseEvent *);
    void init();
private:
    Ui::Playo *ui;

    DataStore * settings;
};

#endif // PLAYO_H
