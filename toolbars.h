#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qmenu.h>

#include "item_toolbar_features.h"

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/file_utils/data_store.h"

#include "modules/core/web/sources/_sources.h"

#include "modules/controls/time_label.h"
#include "modules/controls/metric_slider.h"
#include "modules/controls/toolbar.h"
#include "modules/controls/toolbar_user_button.h"
#include "modules/controls/spectrum_view.h"
#include "modules/controls/equalizer_view.h"

#define TOOLBAR_PL_ITEM_FEATURES_KEY    LSTR("PlayedItemFeatures")
#define TOOLBAR_SL_ITEM_FEATURES_KEY    LSTR("SelectedItemFeatures")
#define TOOLBAR_MEDIA_KEY               LSTR("Media")
#define TOOLBAR_MEDIA_PLUS_KEY          LSTR("Media+")
#define TOOLBAR_MEDIA_POS_KEY           LSTR("Media+Position")
#define TOOLBAR_MEDIA_PAN_KEY           LSTR("Media+Pan")
#define TOOLBAR_MEDIA_TIME_KEY          LSTR("Media+Time")
#define TOOLBAR_MEDIA_VOLUME_KEY        LSTR("Media+Volume")
#define TOOLBAR_CONTROLS_KEY            LSTR("Controls")
#define TOOLBAR_SPECTRUM_KEY            LSTR("Spectrum")
#define TOOLBAR_EQUALIZER_KEY           LSTR("Equalizer")
#define TOOLBAR_EQUALIZER_BUTTON_KEY    LSTR("EqualizerButton")
#define TOOLBAR_SEARCH_KEY              LSTR("Search")
#define TOOLBAR_SETTINGS_KEY            LSTR("Settings")
#define TOOLBAR_SERVICE_MARK            "service"
#define TOOLBAR_BLOCK_MARK              "blocked"


namespace Presentation {
    using namespace Controls;
    using namespace Core::Web;

    class ToolBars : public QObject, public Core::Singleton<ToolBars> {
        Q_OBJECT
    public:
        ~ToolBars() { }

        static inline QString settingsName() { return LSTR("bars"); }

        QMenu * createPopupMenu(QMenu * base_menu = 0);
        SelectedItemToolbarFeatures * selectedItemFeatures() const { return sl_item_features; }

        void load(const QJsonArray & bars);
        void save(Core::DataStore * settings);
        void createToolbars();
        void addPanelButton(const QString & name, const QString & path, QToolBar * bar);

        inline QList<QToolBar *> toolbars() { return container -> findChildren<QToolBar *>(); }
        SpectrumView * getSpectrum();
        inline void updateMetricSliders() { slider -> updateMetric(); }

        inline QJsonObject getEqualizerSettings() { return equalizer -> settings(); }
        inline void setEqualizerSettings(const QJsonObject & settings) { equalizer -> setSettings(settings); }
        inline void setIconsSize(int newSize) {
            QList<QToolBar *> bars = toolbars();
            QSize size(newSize, newSize);

            for(QList<QToolBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++)
                (*bar) -> setIconSize(size);
        }

        inline void setContainer(QMainWindow * ct) { container = ct; }
    public slots:
        void hideAll();
        void showAll();
        void changeToolbarMovable();
        void changeToolbarsMovable();
        void toolbarOrientationChanged(Qt::Orientation);

        void removePanelHighlight();
        void panelHighlight(QAction * action = 0);
        void addPanelTriggered();
        void removePanelTriggered();
        void addPanelButtonTriggered();
        void removePanelButtonTriggered();
        void editPanelButtonTriggered();

    protected slots:
        void playerStateChanged(const PlayerState &);
        void onFolderDrop(const QString & name, const QString & path);

        inline void onMovableChanged(bool /*movable*/) {
            updateBarStyle((QToolBar *)sender());
        }

        inline void onTopLevelChanged(bool /*topLevel*/) { Settings::currentStyle -> applyProperty((QToolBar *)sender(), "state", LSTR("float")); }

    private:
        void updateBarStyle(QToolBar * bar);

        QToolBar * deiterateToToolBar(QWidget * obj);

        QToolBar * linkNameToToolbars(const QString & barName);

        QToolBar * createToolBar(const QString & name);
        QToolBar * precreateToolBar(const QString & name, bool oriented = false);

        QToolBar * createSelectedItemFeaturesBar();
        QToolBar * createPlayedItemFeaturesBar();
        QToolBar * createMediaBar();
        QToolBar * createAdditionalMediaBar();
        QToolBar * createPositionMediaBar();
        QToolBar * createPanMediaBar();
        QToolBar * createTimeMediaBar();
        QToolBar * createVolumeMediaBar();
        QToolBar * createControlToolBar();
        QToolBar * createSearchButtonBar();
        QToolBar * createSettingsButtonBar();
        QToolBar * createEqualizerToolBar();
        QToolBar * createEqualizerButtonBar();

        friend class Core::Singleton<ToolBars>;
        inline ToolBars() : QObject(), play_btn(0), pause_btn(0), stop_btn(0), pl_item_features(0), sl_item_features(0),
            highlighted(0), equalizer(0), spectrum(0), underMouseBar(0), underMouseButton(0) { }

        QAction * play_btn;
        QAction * pause_btn;
        QAction * stop_btn;

        /////////////// item features ///////////////////////

        PlayedItemToolbarFeatures * pl_item_features;
        SelectedItemToolbarFeatures * sl_item_features;

        //////////////////////////////////////

        QToolBar * highlighted;
        MetricSlider * slider;

        EqualizerView * equalizer;
        SpectrumView * spectrum;
        QToolBar * underMouseBar;
        ToolbarUserButton * underMouseButton;

        QPoint lastClickPoint;
        QMainWindow * container;
    };
}

#endif // TOOLBARS_H
