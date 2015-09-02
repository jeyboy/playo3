#ifndef EQUALIZER_VIEW
#define EQUALIZER_VIEW

#include <qwidget.h>
#include <qgridlayout.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>

#include "clickable_slider.h"
#include "media/player.h"
#include "dialogs/toolbardialog.h"

namespace Playo3 {
    #define DEFAULT_PRESET QStringLiteral("Manual")

    class EqualizerView : public QWidget {
        Q_OBJECT
    public:
        EqualizerView(QWidget * parent = 0);
        ~EqualizerView();

        QJsonObject settings();
        void setSettings(QJsonObject settings);

    protected slots:
        void createPreset();
        void removePreset();

        void presetChanged(QString name);
        void eqValueChanged(int);
        void reset();

    private:
        bool presetChanging;

        QList<QLabel *> dbOutput;
        QMap<QString, QMap<int, int> > presets;

        QCheckBox * enabled;
        QComboBox * presetsList;
    };
}

#endif // EQUALIZER_VIEW
