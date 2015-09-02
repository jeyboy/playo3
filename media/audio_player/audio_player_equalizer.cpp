#include "audio_player_equalizer.h"

using namespace AudioPlayer;

void Equalizer::registerEQ(bool registrate) {
    if (useEQ = registrate) registerEQ();
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
//    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    BASS_FXSetParameters(_fxEQ, &eq);
    eqBandsGain.insert(band, gain);
}
