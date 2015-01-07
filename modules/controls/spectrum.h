#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QToolBar>
#include <QPainter>

namespace Playo3  {
    class Spectrum : public QToolBar {
        Q_OBJECT

    public:
        Spectrum(QWidget *parent = 0);
        ~Spectrum();

        int workHeight();

    public slots:
        void bandCountChanged(int newCount);
        void heightChanged(int newHeight);

    protected slots:
        void dataUpdated(QList<QVector<int> >);

    protected:
        inline int verticalPadd() { return 5; }
        inline int paddWidth() { return 2; }

        void paintEvent(QPaintEvent *event);
        void paintCombo();
        void paintDuo();

    private:
        QList<QVector<int> > peaks;
        int bars_count;
        bool isWave;
    };
}

#endif // SPECTRUM_H
