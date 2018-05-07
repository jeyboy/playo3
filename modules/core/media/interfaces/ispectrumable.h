#ifndef ISPECTRUMABLE
#define ISPECTRUMABLE

#include <qvector.h>
#include <qtimer.h>

#include "itrackable.h"
//#include "player_states.h"

class ISpectrumable : public ITrackable {
    Q_OBJECT

    QTimer * itimer;
    QList<QVector<float> > sdefault;
    bool block_multichannel;
protected:
    inline ISpectrumable(QWidget * parent) : ITrackable(parent), block_multichannel(false), channels_count(2) {
        qRegisterMetaType<QList<QVector<float> > >("QList<QVector<float> >");
        itimer = new QTimer(parent);
        spectrumBandsCount(12, false);
        connect(itimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    }

    inline void spectrumCalcStart() { if (isSupportSpectrumCalc()) itimer -> start(); }
    inline void spectrumCalcStop() {
        if (!isSupportSpectrumCalc()) return;
        itimer -> stop();
        calcSpectrum(); // emit default spectrum level
    }

    void channelsCount(int newChannelsCount);

    virtual PlayerState state() const = 0;
    virtual bool calcSpectrum(QVector<float> & /*result*/) { return false; }
    virtual bool calcSpectrum(QList<QVector<float> > & /*result*/) { return false; }
    virtual bool isSupportSpectrumCalc() { return false; }

    virtual bool respondToMultichannelSpectrumCalc() { return !block_multichannel && channels_count != 1; }

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

    inline void setSpectrumFreq(const int & millis) { itimer -> setInterval(millis); }

    inline int channelsCount() const { return channels_count; }
    inline int calcSpectrumBandsGroupCount() { return sbands_count / (respondToMultichannelSpectrumCalc() ? (channels_count / 2) : channels_count); }
signals:
    void spectrumChanged(const QList<QVector<float> > &);
    void channelsCountChanged();
public slots:
    inline void setMultichannelRendering(bool render) {
        bool updateRequired = block_multichannel == render;
        block_multichannel = !render;
        if (updateRequired)
            spectrumBandsCount(sbands_count, true);
    }
protected slots:
    void calcSpectrum();
};

#endif // ISPECTRUMABLE
