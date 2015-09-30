#ifndef AUDIO_PLAYER_EQUALIZER
#define AUDIO_PLAYER_EQUALIZER

#include "bass.h"
#include "bass_fx.h"
//#include "bassmix.h"

#include "ispectrumable.h"

#include <qmap.h>
#include <qstringbuilder.h>

namespace AudioPlayer {
    class Equalizer : public Spectrum {
        Q_OBJECT
    public:
        virtual ~Equalizer() {}

        inline QMap<int, int> eqGains() const { return eqBandsGain; }
        inline void setEqGains(QMap<int, int> gains) { eqBandsGain = gains; }
        void setEQBand(int band, float gain);
        inline QMap<int, QString> bands() const { return eqBands; }
    public slots:
        void registerEQ(bool registrate);
    protected:
        Equalizer(QObject * parent = 0);

        virtual unsigned long chId() const = 0;

        void registerEQ();
        void unregisterEQ();

        HFX _fxEQ;
        bool useEQ;
        QMap<int, QString> eqBands;
        QMap<int, int> eqBandsGain;
    };
}

#endif // AUDIO_PLAYER_EQUALIZER
