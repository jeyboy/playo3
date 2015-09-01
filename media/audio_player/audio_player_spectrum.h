#ifndef AUDIO_PLAYER_SPECTRUM
#define AUDIO_PLAYER_SPECTRUM

#include <qobject.h>

#include "bass.h"

class AudioPlayerSpectrum : public QObject {
    Q_OBJECT
public:
    virtual ~AudioPlayerSpectrum() {}
public:
    inline QList<QVector<int> > & getDefaultSpectrum() { return defaultSpectrum; }
    int getCalcSpectrumBandsCount();

    void setSpectrumBandsCount(int bandsCount);
    inline int spectrumBandsCount() const { return _spectrumBandsCount; }
    inline void setSpectrumHeight(int newHeight) { spectrumHeight = newHeight; }
    inline void setSpectrumFreq(int millis) { spectrumTimer -> setInterval(millis); }
signals:
    void spectrumChanged(QList<QVector<int> >);
protected slots:
    void calcSpectrum();
protected:
    inline AudioPlayerSpectrum(QObject * parent = 0) : QObject(parent), spectrumHeight(0), defaultSpectrumLevel(0) {
        setSpectrumBandsCount(28);
    }

    float fastSqrt(float x);

    QVector<int> spectrumPoints;
    QVector<int> spectrumComplexPoints;

    QVector<int> getSpectrum();
    QList<QVector<int> > getComplexSpectrum();

    int _spectrumBandsCount;
    int spectrumHeight;
    int defaultSpectrumLevel;
    QList<QVector<int> > defaultSpectrum;
};

#endif // AUDIO_PLAYER_SPECTRUM
