#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <qtoolbar.h>
#include <qpainter.h>
#include <qbrush.h>
#include <qevent.h>
#include <qmenu.h>

#include "misc/settings.h"

//FIXME: on visibility changed need to stop/start spec calc in player

namespace Playo3  {
    class Spectrum : public QToolBar {
        Q_OBJECT
    public:
        Spectrum(const QString & objName, QWidget * parent = 0);
        ~Spectrum();

        inline SpectrumType viewType() const { return type; }
        void generateContextMenu(QMenu * parent);

    public slots:
        inline void setBarsView()      { changeType(Playo3::bars); }
        inline void setSplitBarsView() { changeType(Playo3::split_bars); }
        inline void setWavesView()     { changeType(Playo3::waves); }

        void updateColors();
        void changeType(SpectrumType);
        void changeBandCount();
        void changeHeight(int newHeight);

    protected slots:
        void dataUpdated(QList<QVector<int> >);
        void onMovableChanged(bool movable);
        void onOrientationChanged(Qt::Orientation orientation);
        void recalcAttrs();

    protected:
        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *event);

        int calcBarCount();
        int peakDimension();
        inline int verticalPadd() { return 5; }
        inline int paddWidth() { return 2; }
        inline int beetweenSpace() { return 6; }

        void paintCombo();
        void paintDuo();
        void paintWaves();

    private:
        QList<QVector<int> > peaks;

        int offset, last_pairs_count, pairs;
        double start_h_offset, start_v1_offset, start_v2_offset;
        float bar_width;
        QRect update_rect;

        SpectrumType type;

        QLinearGradient g, gg;
    };
}

#endif // SPECTRUM_H
