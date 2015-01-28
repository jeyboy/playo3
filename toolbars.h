#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <QToolBar>
#include <QMainWindow>
#include <qmenu.h>

#include "media/player.h"
#include "misc/file_utils/data_store.h"

//#include "web/socials/vk_api.h"
//#include "web/socials/soundcloud_api.h"

#include "modules/controls/metric_slider.h"
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

        static ToolBars * instance(QObject * parent = 0);
        static void close() { delete self; }

        static inline QString settingsName() { return "bars"; }

        QMenu * createPopupMenu(QMainWindow * window);

        void load(QJsonArray & bars);
        void save(DataStore * settings);
        void createToolbars(QMainWindow * window);
        void addPanelButton(QString name, QString path, QToolBar * bar);

        inline QList<QToolBar *> toolbars() { return parent() -> findChildren<QToolBar *>(); }
        Spectrum * getSpectrum();

    public slots:
        void hideAll();
        void showAll();
        void changeToolbarMovable();
        void changeToolbarsMovable();
        void toolbarOrientationChanged(Qt::Orientation);

        void removePanelHighlight();
        void panelHighlight(QAction *action = 0);
        void addPanelTriggered();
        void removePanelTriggered();
        void addPanelButtonTriggered();
        void removePanelButtonTriggered();

    protected slots:
        void onFolderDrop(QString name, QString path);

        inline void onMovableChanged(bool /*movable*/) {
            updateBarStyle((QToolBar *)sender());
        }

        inline void onTopLevelChanged(bool /*topLevel*/) {
            QToolBar * bar = (QToolBar *)sender();
            bar -> setStyleSheet(Stylesheets::toolbarFloatStyle());
        }

    private:
        inline void updateBarStyle(QToolBar * bar) {
            if (!bar) return;
            if (bar -> isMovable() || qobject_cast<ToolBar *>(bar) != 0)
                bar -> setStyleSheet(Stylesheets::toolbarMovableStyle());
            else
                bar -> setStyleSheet(Stylesheets::toolbarFixedStyle());
        }

        QToolBar * deiterateToToolBar(QWidget * obj);

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

        ToolBars(QObject * parent) : QObject(parent),
            vkToolButton(0), soundcloudToolButton(0), highlighted(0), spectrum(0),
            underMouseBar(0), underMouseButton(0) {

        }

        QToolButton * vkToolButton;
        QToolButton * soundcloudToolButton;

        QToolBar * highlighted;

        Spectrum * spectrum;
        QToolBar * underMouseBar;
        ToolbarButton * underMouseButton;

        QPoint lastClickPoint;

        static ToolBars *self;
    };
}

#endif // TOOLBARS_H
