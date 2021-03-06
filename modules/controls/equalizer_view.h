#ifndef EQUALIZER_VIEW
#define EQUALIZER_VIEW

#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qscrollarea.h>

#include <qgridlayout.h>
#include <qboxlayout.h>


#include "modules/core/defines.h"
#include "settings.h"
#include "dialogs/user_action_dialog.h"
#include "clickable_slider.h"

#define EQ_MIN_INTERVAL -150
#define EQ_MAX_INTERVAL 150
#define EQ_BASE 10.0
#define DB_STR QLatin1String(" dB")

namespace Controls {
    #define DEFAULT_PRESET QLatin1String("Manual")
    #define EQUALIZER_JSON_KEY QLatin1String("presets")

    class EqualizerView : public QWidget {
        Q_OBJECT
    public:
        EqualizerView(QWidget * parent = 0);
        ~EqualizerView();

        QJsonObject settings();
        void setSettings(const QJsonObject & json);

    protected slots:
        void createPreset();
        void removePreset();

        void presetTypeChanged();
        void presetChanged(QString name);
        void eqValueChanged(int);
        void reset();

    private:
        void initTopLayout(QHBoxLayout * layout);
        void initBottomLayout(QGridLayout * layout);

        bool presetChanging;

        QList<QLabel *> dbOutput;
        QMap<QString, QMap<int, int> > presets;

        QCheckBox * enabled;
        QComboBox * presetsList;
        QComboBox * presetTypesList;
        QScrollArea * scrollArea;
    };
}

#endif // EQUALIZER_VIEW
