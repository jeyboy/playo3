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

#include <qjsonobject.h>

#include "settings.h"
#include "dialogs/user_action_dialog.h"
#include "clickable_slider.h"

namespace Controls {
    #define DEFAULT_PRESET QStringLiteral("Manual")
    #define JSON_KEY QStringLiteral("presets")

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
    };
}

#endif // EQUALIZER_VIEW
