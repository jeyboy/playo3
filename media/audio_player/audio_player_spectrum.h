#ifndef AUDIO_PLAYER_SPECTRUM
#define AUDIO_PLAYER_SPECTRUM

#include "misc/settings.h"

#include "media/notify_timer.h"
#include "audio_player_states.h"
#include "bass.h"

#include <qobject.h>
#include <qvector.h>

namespace AudioPlayer {
    class Spectrum : virtual public QObject {
    public:
        virtual ~Spectrum() {}
    public:
        inline QList<QVector<int> > & getDefaultSpectrum() { return defaultSpectrum; }
        int getCalcSpectrumBandsCount();

        void setSpectrumBandsCount(int bandsCount);
        inline int spectrumBandsCount() const { return _spectrumBandsCount; }
        inline void setSpectrumHeight(int newHeight) { spectrumHeight = newHeight; }
        inline void setSpectrumFreq(int millis) { spectrumTimer -> setInterval(millis); }

        inline int getChannelsCount() const { return channelsCount; }
    signals:
        void spectrumChanged(QList<QVector<int> >);
        void channelsCountChanged();
    protected slots:
        void calcSpectrum();
    protected:
        inline Spectrum() : spectrumHeight(0), defaultSpectrumLevel(0), channelsCount(2), prevChannelsCount(0) {
            setSpectrumBandsCount(28);
        }

        virtual unsigned long chId() const = 0;
        virtual MediaState state() const = 0;

        float fastSqrt(float x);

        QVector<int> spectrumPoints;
        QVector<int> spectrumComplexPoints;

        QVector<int> getSpectrum();
        QList<QVector<int> > getComplexSpectrum();

        int _spectrumBandsCount;
        int spectrumHeight;
        int defaultSpectrumLevel;
        QList<QVector<int> > defaultSpectrum;

        int channelsCount, prevChannelsCount;

        NotifyTimer * spectrumTimer;
    };
}

#endif // AUDIO_PLAYER_SPECTRUM
