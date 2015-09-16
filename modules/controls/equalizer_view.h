#ifndef EQUALIZER_VIEW
#define EQUALIZER_VIEW

#include <qwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qscrollarea.h>

#include <qgridlayout.h>
#include <qboxlayout.h>

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

        Json::Obj settings();
        void setSettings(const Json::Obj & json);

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
