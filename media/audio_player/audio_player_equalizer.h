#ifndef AUDIO_PLAYER_EQUALIZER
#define AUDIO_PLAYER_EQUALIZER

#include "bass.h"
#include "bass_fx.h"
//#include "bassmix.h"

#include <qobject.h>
#include <qmap.h>
#include <qstringbuilder.h>

namespace AudioPlayer {
    class Equalizer : public virtual QObject {
    public:
        virtual ~Equalizer() {}

        inline QMap<int, int> eqGains() const { return eqBandsGain; }
        inline void setEqGains(QMap<int, int> gains) { eqBandsGain = gains; }
        void setEQBand(int band, float gain);
        inline QMap<int, QString> bands() const { return eqBands; }
    public slots:
        void registerEQ(bool registrate);
    protected:
        Equalizer() : _fxEQ(0), useEQ(false) {
            eqBands.insert(20, QStringLiteral("20"));
            eqBands.insert(32, QStringLiteral("32"));
            eqBands.insert(64, QStringLiteral("64"));
            eqBands.insert(90, QStringLiteral("90"));
            eqBands.insert(125, QStringLiteral("125"));
            eqBands.insert(160, QStringLiteral("160"));
            eqBands.insert(200, QStringLiteral("200"));

            eqBands.insert(250, QStringLiteral("250"));
            eqBands.insert(375, QStringLiteral("375"));
            eqBands.insert(500, QStringLiteral("500"));
            eqBands.insert(750, QStringLiteral("750"));
            eqBands.insert(1000, QStringLiteral("1k"));
            eqBands.insert(1500, QStringLiteral("1.5k"));

            eqBands.insert(2000, QStringLiteral("2k"));
            eqBands.insert(3000, QStringLiteral("3k"));
            eqBands.insert(4000, QStringLiteral("4k"));
            eqBands.insert(8000, QStringLiteral("8k"));
            eqBands.insert(12000, QStringLiteral("12k"));
            eqBands.insert(16000, QStringLiteral("16k"));
        }

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
