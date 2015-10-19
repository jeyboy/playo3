#ifndef PLAYO_H
#define PLAYO_H

#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "modules/controls/user_dialog_box.h"

#include "toolbars.h"
#include "dockbars.h"

#include <qsettings.h>

//#include "media/cue/cue.h"
//#include "media/library.h"
//#include "media/genres/music_genres.h"

#include "player/player.h"
#include "modules/core/misc/file_utils/data_store.h"
#include "modules/core/data_core.h"

#include "dialogs/search_dialog.h"
#include "dialogs/settings_dialog.h"
#include "dialogs/relations_dialog.h"
#include "dialogs/echonest_dialog.h"

#include "modules/plugins/plugins.h"

namespace Ui { class Playo; }

#define FRONT_SETTINGS_FILE_NAME QStringLiteral("settings.ini")
#define BACKEND_SETTINGS_FILE_NAME QStringLiteral("settings.json")

#define SETTINGS_SET_KEY QStringLiteral("settings")
#define SETTINGS_GEOMETRY_SET_KEY QStringLiteral("geometry")
#define SETTINGS_EQUALIZER_SET_KEY QStringLiteral("equalizer")
#define SETTINGS_WINDOW_STATE_SET_KEY QStringLiteral("windowState")
#define SETTINGS_WINDOW_MAXIMIZED_KEY QStringLiteral("maximized")

using namespace Controls;

class Playo : public MainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget * parent = 0);
    ~Playo();

    void worksoso(int c) {
        qDebug() << "In work soso" << c;
    }
    QJsonObject worksosoJson() {
        qDebug() << "In work soso json";
        QJsonObject o;
        o.insert("a", "312");
        o.insert("b", "1");
        return o;
    }
    QJsonArray worksosoJsonArray() {
        qDebug() << "In work soso json array";
        QJsonArray o;
        o.append("312");
        o.append("b");
        return o;
    }

    void soso() {
        qDebug() << "In soso";
    }
    void sosoJson(QJsonObject a) {
        qDebug() << "In json soso" << a;
    }
    void sosoJsonArray(QJsonArray a) {
        qDebug() << "In json array soso" << a;
    }


public slots:
//    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);

private slots:
//    void showError(QString);

    void showSearchDialog();
    void showSettingsDialog();

    void showEchonestDialog();

    void openOdTabDialog();

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
