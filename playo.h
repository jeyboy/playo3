#ifndef PLAYO_H
#define PLAYO_H

#include <qsettings.h>

#include "modules/controls/window.h"
#include "modules/controls/tray.h"
#include "modules/controls/user_dialog_box.h"

#include "toolbars.h"
#include "dockbars.h"

#include "modules/core/misc/file_utils/data_store.h"
#include "modules/core/data_factory.h"

#include "dialogs/search_dialog.h"
#include "dialogs/settings_dialog.h"

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

    void postinitialization();
public slots:
//    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(const QString &);

private slots:
     void playedItemChanged(const QString &, const QString &);

    void messa(const QString & str) {
        qDebug() << "MESSA" << str;
    }
//    void showError(QString);

    void showSearchDialog();
    void showSettingsDialog();

    void openFolderTriggered();
protected:
    void activation();
    void init();

    void putToCommonTab(const QList<QUrl> & urls, const bool & play = false);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
    QMenu * createPopupMenu();
    void closeEvent(QCloseEvent *);
private:
    Ui::Playo * ui;

    Core::DataStore * settings;
    Tray * tray;
};

#endif // PLAYO_H
