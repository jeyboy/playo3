#ifndef PLAYO_H
#define PLAYO_H

#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "modules/controls/user_dialog_box.h"

#include "toolbars.h"
#include "dockbars.h"

#include <QSettings>

#include "media/library.h"
#include "media/genres/music_genres.h"
#include "misc/file_utils/data_store.h"

#include "dialogs/settings_dialog.h"
#include "dialogs/web_dialog.h"
#include "dialogs/relations_dialog.h"

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

private slots:
//    void showError(QString);

    void showSettingsDialog();

    void openVKTabDialog();
    void showVKTabDialog();
    void showVKRelTabDialog();

//    void showSoundcloudRelTabDialog();
//    void openSoundcloudTabDialog();
//    void showSoundcloudTabDialog();

    void openFolderTriggered();

//    void dropEvent(QDropEvent * event);


protected:
    void activation();
    void initialization();

    void putToCommonTab(QList<QUrl> urls);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
    QMenu * createPopupMenu();
    void closeEvent(QCloseEvent *);
private:
    Ui::Playo *ui;

    DataStore * settings;
};

#endif // PLAYO_H
