#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <qwidget.h>
#include <qgridlayout.h>

#include "clickable_slider.h"
#include "media/player.h"
#include "dialogs/toolbardialog.h"

namespace Playo3 {
    class Equalizer : public QWidget {
        Q_OBJECT

    public:
        Equalizer(QWidget * parent = 0);
        ~Equalizer();

        QJsonObject settings();
        void setSettings(QJsonObject settings);

    protected slots:
        void createPreset();
        void removePreset();

        void presetChanged(QString name);
        void eqValueChanged(int);
        void reset();

    private:
        QMap<QString, QMap<int, int> > presets;

        QCheckBox * enabled;
        QComboBox * presetsList;
    };
}

#endif // EQUALIZER_H
