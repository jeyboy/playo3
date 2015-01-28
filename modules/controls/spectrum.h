#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QToolBar>
#include <QPainter>
#include <QLinearGradient>
#include <qevent.h>
#include <qmenu.h>

namespace Playo3  {
    class Spectrum : public QToolBar {
        Q_OBJECT
    public:
        enum SpectrumType {
            bars,
            split_bars,
            waves
        };

        Spectrum(QWidget * parent = 0);
        ~Spectrum();

        inline SpectrumType viewType() const { return type; }
        void generateContextMenu(QMenu * parent);

    public slots:
        void setBarsView()      { changeType(Spectrum::bars); }
        void setSplitBarsView() { changeType(Spectrum::split_bars); }
        void setWavesView()     { changeType(Spectrum::waves); }

        void updateColors();
        void changeType(SpectrumType);
        void changeBandCount(int newCount);
        void changeHeight(int newHeight);

    protected slots:
        void dataUpdated(QList<QVector<int> >);
        void onMovableChanged(bool movable);
        void onOrientationChanged(Qt::Orientation orientation);

    protected:
        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *event);

        void recalcAttrs();

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

        SpectrumType type;

        QLinearGradient g, gg;
    };
}

#endif // SPECTRUM_H
