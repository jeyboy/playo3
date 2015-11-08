#ifndef SPECTRUM_VIEW
#define SPECTRUM_VIEW

#include <qtoolbar.h>
#include <qpainter.h>
#include <qbrush.h>
#include <qevent.h>
#include <qmenu.h>

#include "settings.h"

//FIXME: on visibility changed need to stop/start spec calc in player

namespace Controls  {
    class SpectrumView : public QToolBar {
        Q_OBJECT
    public:
        SpectrumView(const QString & objName, QWidget * parent = 0);
        ~SpectrumView();

        inline SpectrumType viewType() const { return type; }
        void generateContextMenu(QMenu * parent);

    public slots:
        inline void setBarsView()      { changeType(bars); }
        inline void setSplitBarsView() { changeType(split_bars); }

        void updateColors();
        void changeType(SpectrumType);
        void changeBandCount();
        void changeHeight(int newHeight);

    protected slots:
        void dataUpdated(const QList<QVector<float> > &);
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
        void paintPairs();

    private:
        QList<QVector<float> > peaks;

        int offset, last_pairs_count, pairs, mult_bar_height;
        double start_h_offset, start_v1_offset, start_v2_offset;
        float bar_width, bar_height;
        QRect update_rect;

        SpectrumType type;

        QLinearGradient g, gg;
    };
}

#endif // SPECTRUM_VIEW
