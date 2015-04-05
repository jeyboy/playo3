#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <QToolBar>
#include <QMainWindow>
#include <qmenu.h>

#include "media/player.h"
#include "misc/file_utils/data_store.h"

#include "modules/web/socials_api/vk_api.h"
#include "modules/web/socials_api/soundcloud_api.h"

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
            delete vkToolButton;
            delete soundcloudToolButton;
            delete spectrum;
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

        QToolButton * initiateVkButton();
        QToolButton * initiateSoundcloudButton();

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
        void updateBarStyle(QToolBar * bar);

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
