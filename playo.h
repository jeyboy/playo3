#ifndef PLAYO_H
#define PLAYO_H

#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "modules/controls/user_dialog_box.h"

#include "toolbars.h"
#include "dockbars.h"

#include <qsettings.h>

#include "misc/web_utils/html_parser.h"
//#include "media/cue/cue.h"
#include "media/library.h"
#include "media/genres/music_genres.h"
#include "misc/file_utils/data_store.h"

#include "dialogs/search_dialog.h"
#include "dialogs/settings_dialog.h"
#include "dialogs/relations_dialog.h"
#include "dialogs/echonest_dialog.h"

#include "modules/plugins/loader.h"

using namespace Playo3;

namespace Ui { class Playo; }

#define FRONT_SETTINGS_FILE_NAME QStringLiteral("settings.ini")
#define BACKEND_SETTINGS_FILE_NAME QStringLiteral("settings.json")

#define SETTINGS_VK_SET_KEY QStringLiteral("vk")
#define SETTINGS_SOUNDCLOUD_SET_KEY QStringLiteral("soundcloud")
#define SETTINGS_SET_KEY QStringLiteral("settings")
#define SETTINGS_GEOMETRY_SET_KEY QStringLiteral("geometry")
#define SETTINGS_EQUALIZER_SET_KEY QStringLiteral("equalizer")
#define SETTINGS_WINDOW_STATE_SET_KEY QStringLiteral("windowState")
#define SETTINGS_WINDOW_MAXIMIZED_KEY QStringLiteral("maximized")

class Playo : public Playo3::MainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget * parent = 0);
    ~Playo();

public slots:
//    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);

private slots:
//    void showError(QString);

    void showSearchDialog();
    void showSettingsDialog();

    void showEchonestDialog();

    void openVKRecomendations();
    void openVKTabDialog();
    void showVKTabDialog();
    void showVKRelTabDialog();

    void showSoundcloudRelTabDialog();
    void openSoundcloudTabDialog();
    void showSoundcloudTabDialog();

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
    Ui::Playo * ui;

    DataStore * settings;
};

#endif // PLAYO_H
