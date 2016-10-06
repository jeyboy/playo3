#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qmenu.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/file_utils/data_store.h"

#include "modules/core/web/sources/_sources.h"

#include "modules/controls/time_label.h"
#include "modules/controls/metric_slider.h"
#include "modules/controls/toolbar.h"
#include "modules/controls/toolbar_user_button.h"
#include "modules/controls/spectrum_view.h"
#include "modules/controls/equalizer_view.h"

namespace Presentation {
    using namespace Controls;
    using namespace Core::Web;

    class ToolBars : public QObject, public Core::Singleton<ToolBars> {
        Q_OBJECT
    public:
        ~ToolBars() { }

        static inline QString settingsName() { return QStringLiteral("bars"); }

        QMenu * createPopupMenu();

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
        void itemFeaturesChanged();

        void loadMoreItem();
        void openRecsforItem();
        void openRecsforItemUser();
        void openRecsforItemArtist();
        void openTracksforArtist();
        void openAlbumsForArtist();
        void openRecsForItemTags();
        void openTracksForLabel();
        void openAlbumsForLabel();

        void playerStateChanged(const PlayerState &);
        void onFolderDrop(const QString & name, const QString & path);

        inline void onMovableChanged(bool /*movable*/) {
            updateBarStyle((QToolBar *)sender());
        }

        inline void onTopLevelChanged(bool /*topLevel*/) { Settings::currentStyle -> applyProperty((QToolBar *)sender(), "state", QStringLiteral("float")); }

    private:
        void updateBarStyle(QToolBar * bar);

        QToolBar * deiterateToToolBar(QWidget * obj);

        QToolBar * linkNameToToolbars(const QString & barName);

        QToolBar * createToolBar(const QString & name);
        QToolBar * precreateToolBar(const QString & name, bool oriented = false);

        QToolBar * createItemFeaturesBar();
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
        inline ToolBars() : QObject(), highlighted(0), equalizer(0), spectrum(0),
            underMouseBar(0), underMouseButton(0) { }

        QAction * play_btn;
        QAction * pause_btn;
        QAction * stop_btn;

        QToolBar * item_features;

        QAction * more_items_btn;
        QAction * item_song_btn;
        QAction * item_singer_btn;
        QAction * item_singer_song_btn;
        QAction * item_singer_album_btn;
        QAction * item_owner_btn;
        QAction * item_tags_btn;
        QAction * item_labels_song_btn;
        QAction * item_labels_album_btn;

        QToolBar * highlighted;
        MetricSlider * slider;

        EqualizerView * equalizer;
        SpectrumView * spectrum;
        QToolBar * underMouseBar;
        ToolbarUserButton * underMouseButton;

        QPoint lastClickPoint;
        QMainWindow * container;

        const QString toolbar_item_features_key =       LSTR("ItemFeatures");
        const QString toolbar_media_key =               LSTR("Media");
        const QString toolbar_media_plus_key =          LSTR("Media+");
        const QString toolbar_media_pos_key =           LSTR("Media+Position");
        const QString toolbar_media_pan_key =           LSTR("Media+Pan");
        const QString toolbar_media_time_key =          LSTR("Media+Time");
        const QString toolbar_media_volume_key =        LSTR("Media+Volume");
        const QString toolbar_controls_key =            LSTR("Controls");
        const QString toolbar_spectrum_key =            LSTR("Spectrum");
        const QString toolbar_equalizer_key =           LSTR("Equalizer");
        const QString toolbar_equalizer_button_key =    LSTR("EqualizerButton");
        const QString toolbar_search_key =              LSTR("Search");
        const QString toolbar_settings_key =            LSTR("Settings");
        const char * toolbar_service_mark = "service";
        const char * toolbar_block_mark = "blocked";
    };
}

#endif // TOOLBARS_H
