#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QToolBar>
#include <QPainter>
#include <qevent.h>

namespace Playo3  {
    class Spectrum : public QToolBar {
        Q_OBJECT

    public:
        Spectrum(QWidget *parent = 0);
        ~Spectrum();

    public slots:
        void bandCountChanged(int newCount);
        void heightChanged(int newHeight);

    protected slots:
        void dataUpdated(QList<QVector<int> >);
        void onMovableChanged(bool movable);
        void onOrientationChanged(Qt::Orientation orientation);
        //void onTopLevelChanged(bool topLevel);
        void onVisibilityChanged(bool visible);

    protected:
        void resizeEvent(QResizeEvent *);
        void paintEvent(QPaintEvent *event);

        int workHeight();
        inline int verticalPadd() { return 5; }
        inline int paddWidth() { return 2; }

        void paintCombo();
        void paintDuo();

    private:
        QList<QVector<int> > peaks;
        bool isWave;
    };
}

#endif // SPECTRUM_H
