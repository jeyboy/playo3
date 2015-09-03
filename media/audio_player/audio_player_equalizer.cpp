#include "audio_player_equalizer.h"

using namespace AudioPlayer;

Equalizer::Equalizer(QObject * parent) : Spectrum(parent), _fxEQ(0), useEQ(false) {
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

void Equalizer::registerEQ(bool registrate) {
    if ((useEQ = registrate)) registerEQ();
    else unregisterEQ();
}

void Equalizer::registerEQ() {
    if (_fxEQ) unregisterEQ();

    _fxEQ = BASS_ChannelSetFX(chId(), BASS_FX_BFX_PEAKEQ, 0);

    BASS_BFX_PEAKEQ eq;
    eq.fQ = 0;
    eq.fBandwidth = 3; //2.5
    eq.lChannel = BASS_BFX_CHANALL;

    QMap<int, QString>::Iterator band = eqBands.begin();
    for(int num = 0; band != eqBands.end(); band++, num++) {
        eq.fGain = eqBandsGain.value(num, 0);
        eq.lBand = num; eq.fCenter = band.key(); BASS_FXSetParameters(_fxEQ, &eq);
    }
}

void Equalizer::unregisterEQ() {
    if (!_fxEQ) return;
    BASS_ChannelRemoveFX(chId(), _fxEQ);
    _fxEQ = 0;
}

void Equalizer::setEQBand(int band, float gain) {
    BASS_BFX_PEAKEQ eq;
    eq.lBand = band;
    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    BASS_FXSetParameters(_fxEQ, &eq);
    eqBandsGain.insert(band, gain);
}
