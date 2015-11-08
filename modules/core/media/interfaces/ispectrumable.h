#ifndef ISPECTRUMABLE
#define ISPECTRUMABLE

#include <qobject.h>
#include <qvector.h>
#include <qtimer.h>

#include "player_states.h"

class ISpectrumable : public QObject {
    Q_OBJECT

    QTimer * itimer;
    QList<QVector<float> > sdefault;
protected:
    inline ISpectrumable(QObject * parent) : QObject(parent), channels_count(2) {
        itimer = new QTimer(parent);
        spectrumBandsCount(12, false);
        connect(itimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    }

    inline void spectrumCalcStart() { if (spectrumable()) itimer -> start(); }
    inline void spectrumCalcStop() {
        if (!spectrumable()) return;
        itimer -> stop();
        calcSpectrum(); // emit default spectrum level
    }

    void channelsCount(int newChannelsCount);

    virtual PlayerState state() const = 0;
    virtual bool calcSpectrum(QVector<float> & result) = 0;
    virtual bool calcSpectrum(QList<QVector<float> > & result) = 0;
    virtual bool spectrumable() { return true; }

    virtual bool respondToMultichannelSpectrumCalc() { return channels_count != 1; }

//    inline float fastSqrt(float x) {
//      unsigned int i = *(unsigned int*) &x;
//      // adjust bias
//      i  += 127 << 23;
//      // approximation of square root
//      i >>= 1;
//      return *(float*) &i;
//    }

    QVector<int> spectrumPoints;
    QVector<int> spectrumComplexPoints;

    int sbands_count;
    int channels_count;
public:
    virtual ~ISpectrumable() {}

    inline QList<QVector<float> > defaultSpectrum() const { return sdefault; }

    void spectrumBandsCount(int bandsCount, bool update = true);
    inline int spectrumBandsCount() const { return sbands_count; }

    inline void spectrumFreq(int millis) { itimer -> setInterval(millis); }

    inline int channelsCount() const { return channels_count; }
    inline int calcSpectrumBandsGroupCount() { return sbands_count / (respondToMultichannelSpectrumCalc() ? (channels_count / 2) : channels_count); }
signals:
    void spectrumChanged(const QList<QVector<float> > &);
    void channelsCountChanged();
protected slots:
    void calcSpectrum();
};

#endif // ISPECTRUMABLE
