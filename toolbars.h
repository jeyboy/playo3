#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qmenu.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/file_utils/data_store.h"

#include "modules/core/web/apis/social/_socials.h"

#include "modules/controls/metric_slider.h"
#include "modules/controls/toolbar.h"
#include "modules/controls/toolbarbutton.h"
#include "modules/controls/spectrum_view.h"
#include "modules/controls/equalizer_view.h"

#include "stylesheets.h"

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
        void addPanelButton(QString name, QString path, QToolBar * bar);

        inline QList<QToolBar *> toolbars() { return container -> findChildren<QToolBar *>(); }
        SpectrumView * getSpectrum();
        inline void updateMetricSliders() { slider -> updateMetric(); }

        inline QJsonObject getEqualizerSettings() { return equalizer -> settings(); }
        inline void setEqualizerSettings(QJsonObject settings) { equalizer -> setSettings(settings); }

        inline void setContainer(QMainWindow * ct) { container = ct; }
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

        void disconnectVk();
        void disconnectSoundcloud();
        void disconnectOd();

        QToolButton * initiateEchonestButton();
        QToolButton * initiateVkButton();
        QToolButton * initiateSoundcloudButton();
        QToolButton * initiateOdButton();

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
        QToolBar * createPanMediaBar();
        QToolBar * createTimeMediaBar();
        QToolBar * createVolumeMediaBar();
        QToolBar * createControlToolBar();
        QToolBar * createEqualizerToolBar();
        QToolBar * createEqualizerButtonBar();

        friend class Core::Singleton<ToolBars>;
        inline ToolBars() : QObject(),
            vkToolButton(0), soundcloudToolButton(0), odToolButton(0), highlighted(0), equalizer(0), spectrum(0),
            underMouseBar(0), underMouseButton(0) {

            connect(&Vk::Api::obj(), SIGNAL(authorized()), this, SLOT(initiateVkButton()));
            connect(&Soundcloud::Api::obj(), SIGNAL(authorized()), this, SLOT(initiateSoundcloudButton()));
            connect(&Od::Api::obj(), SIGNAL(authorized()), this, SLOT(initiateOdButton()));
        }

        QToolButton * vkToolButton;
        QToolButton * soundcloudToolButton;
        QToolButton * odToolButton;

        QToolBar * highlighted;
        MetricSlider * slider;

        EqualizerView * equalizer;
        SpectrumView * spectrum;
        QToolBar * underMouseBar;
        ToolbarButton * underMouseButton;

        QPoint lastClickPoint;
        QMainWindow * container;

        const QString toolbar_media_key = QStringLiteral("Media");
        const QString toolbar_media_plus_key = QStringLiteral("Media+");
        const QString toolbar_media_pos_key = QStringLiteral("Media+Position");
        const QString toolbar_media_pan_key = QStringLiteral("Media+Pan");
        const QString toolbar_media_time_key = QStringLiteral("Media+Time");
        const QString toolbar_media_volume_key = QStringLiteral("Media+Volume");
        const QString toolbar_controls_key = QStringLiteral("Controls");
        const QString toolbar_spectrum_key = QStringLiteral("Spectrum");
        const QString toolbar_equalizer_key = QStringLiteral("Equalizer");
        const QString toolbar_equalizer_button_key = QStringLiteral("EqualizerButton");
        const char * toolbar_service_mark = "service";
    };
}

#endif // TOOLBARS_H
