#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <QToolBar>
#include <QMainWindow>
#include <QSlider>
#include <qmenu.h>

#include "media/player.h"
#include "modules/json/data_store.h"

//#include "web/socials/vk_api.h"
//#include "web/socials/soundcloud_api.h"

#include "modules/controls/slider.h"
#include "modules/controls/toolbar.h"
#include "modules/controls/toolbarbutton.h"
#include "modules/controls/spectrum.h"

#include "dialogs/toolbardialog.h"
#include "dialogs/toolbarbuttondialog.h"
#include "misc/stylesheets.h"

namespace Playo3 {
    class ToolBars : public QObject {
        Q_OBJECT
    public:
        ~ToolBars() {
            if (vkToolButton)
                delete vkToolButton -> menu();
            delete vkToolButton;

            if (soundcloudToolButton)
                delete soundcloudToolButton -> menu();
            delete soundcloudToolButton;

    //        delete spectrum;
        }

        static ToolBars * instance(QObject * parent);
        static void close() {
            delete self;
        }

        static inline QString settingsName() { return "bars"; }

        QMenu * improvePopupMenu(QMainWindow * window, QMenu * menu);

        void load(QJsonArray & bars);
        void save(DataStore * settings);
        void createToolbars(QMainWindow * window);
        void addPanelButton(QString name, QString path, QToolBar * bar);

        inline QList<QToolBar *> toolbars() { return parent() -> findChildren<QToolBar *>(); }
        Spectrum * getSpectrum();

    public slots:
        void folderDropped(QString name, QString path);
        void changeToolbarMovable();
        void changeToolbarsMovable();
        void toolbarVisibilityChanged(bool visible);
        void toolbarOrientationChanged(Qt::Orientation);

        void removePanelHighlight();
        void panelHighlight(QAction *action = 0);
        void addPanelTriggered();
        void removePanelTriggered();
        void addPanelButtonTriggered();
        void removePanelButtonTriggered();
    private:
        QToolBar * linkNameToToolbars(QString barName);

        QToolBar * createToolBar(QString name);
        QToolBar * precreateToolBar(QString name, bool oriented = false);
        QToolBar * createMediaBar();
        QToolBar * createAdditionalMediaBar();
        QToolBar * createPositionMediaBar();
        QToolBar * createTimeMediaBar();
        QToolBar * createVolumeMediaBar();
        QToolBar * createControlToolBar();

        QToolButton * initiateVkButton();
        QToolButton * initiateSoundcloudButton();

        inline void updateToolbarMovable(QToolBar * bar, bool movable) {
            bar -> setMovable(movable);

            if (!movable && QString(bar -> metaObject() -> className()) != "Playo3::ToolBar")
                bar -> setStyleSheet(Stylesheets::toolbarStyle());
            else bar -> setStyleSheet("");
        }

        ToolBars(QObject * parent) : QObject(parent),
            vkToolButton(0), soundcloudToolButton(0), highlighted(0), spectrum(0),
            underMouseBar(0), activeBar(0), underMouseButton(0) {

        }

        QToolButton * vkToolButton;
        QToolButton * soundcloudToolButton;

        QToolBar * highlighted;

        Spectrum * spectrum;
        QToolBar * underMouseBar, * activeBar;
        ToolbarButton * underMouseButton;

        QPoint lastClickPoint;

        static ToolBars *self;
    };
}

#endif // TOOLBARS_H
