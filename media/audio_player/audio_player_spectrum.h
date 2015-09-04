#ifndef AUDIO_PLAYER_SPECTRUM
#define AUDIO_PLAYER_SPECTRUM

#include "misc/settings.h"

#include "media/notify_timer.h"
#include "audio_player_state.h"
#include "bass.h"

#include <qvector.h>

#define DEFAULT_CHANNELS_COUNT 2

namespace AudioPlayer {
    class Spectrum : public State {
        Q_OBJECT
    public:
        virtual ~Spectrum() {}
    public:
        inline QList<QVector<int> > & getDefaultSpectrum() { return defaultSpectrum; }

        void setSpectrumBandsCount(int bandsCount);
        inline int spectrumBandsCount() const { return _spectrumBandsCount; }
        inline void setSpectrumHeight(int newHeight) { spectrumHeight = newHeight; }
        inline void setSpectrumFreq(int millis) { spectrumTimer -> setInterval(millis); }

        inline int getChannelsCount() const { return channelsCount; }

        int getCalcSpectrumBandsCount() {
            if (channelsCount != prevChannelsCount) {
                prevChannelsCount = channelsCount;
                emit channelsCountChanged();
            }

            return _spectrumBandsCount / (channelsCount == 1 ? channelsCount : (channelsCount / 2));
        }
    signals:
        void spectrumChanged(const QList<QVector<int> > &);
        void channelsCountChanged();
    protected slots:
        void calcSpectrum();
    protected:
        inline Spectrum(QObject * parent) : State(parent), spectrumHeight(0), defaultSpectrumLevel(0), channelsCount(DEFAULT_CHANNELS_COUNT), prevChannelsCount(0) {
            setSpectrumBandsCount(28);
        }

        virtual unsigned long chId() const = 0;

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
