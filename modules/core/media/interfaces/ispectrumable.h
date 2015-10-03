#ifndef ISPECTRUMABLE
#define ISPECTRUMABLE

#include <qobject.h>
#include <qvector.h>
#include <qtimer.h>

#include "player_states.h"

class ISpectrumable : public QObject {
    Q_OBJECT

    QTimer * itimer;
    QList<QVector<int> > sdefault;
protected:
    inline ISpectrumable(QObject * parent) : QObject(parent), sheight(0), sdefault_level(0), channels_count(2) {
        itimer = new QTimer(parent);
        spectrumBandsCount(12);
        connect(this, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    }

    inline void spectrumCalcStart() { if (spectrumable()) itimer -> start(); }
    inline void spectrumCalcStop() {
        if (!spectrumable()) return;
        itimer -> stop();
        calcSpectrum(); // emit default spectrum level
    }

    void channelsCount(int newChannelsCount);

    virtual PlayerState state() const = 0;
    virtual bool calcSpectrum(QVector<int> & result) = 0;
    virtual bool calcSpectrum(QList<QVector<int> > & result) = 0;
    virtual bool spectrumable() { return true; }

    virtual bool respondToMultichannelSpectrumCalc() { return channels_count != 1; }

    inline float fastSqrt(float x) {
      unsigned int i = *(unsigned int*) &x;
      // adjust bias
      i  += 127 << 23;
      // approximation of square root
      i >>= 1;
      return *(float*) &i;
    }

    QVector<int> spectrumPoints;
    QVector<int> spectrumComplexPoints;

    int sbands_count;
    int sheight;
    int sdefault_level;
    int channels_count;
public:
    virtual ~ISpectrumable() {}

    inline QList<QVector<int> > defaultSpectrum() const { return sdefault; }

    void spectrumBandsCount(int bandsCount);
    inline int spectrumBandsCount() const { return sbands_count; }

    inline void spectrumHeight(int newHeight) { sheight = newHeight; }
    inline void spectrumFreq(int millis) { itimer -> setInterval(millis); }

    inline int channelsCount() const { return channels_count; }
    inline int calcSpectrumBandsGroupCount() { return sbands_count / (respondToMultichannelSpectrumCalc() ? (channels_count / 2) : channels_count); }
signals:
    void spectrumChanged(const QList<QVector<int> > &);
    void channelsCountChanged();
protected slots:
    void calcSpectrum();
};

#endif // ISPECTRUMABLE
