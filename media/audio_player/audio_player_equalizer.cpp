#include "audio_player_equalizer.h"

void AudioPlayerEqualizer::registerEQ(bool registrate) {
    if (useEQ = registrate) registerEQ();
    else unregisterEQ();
}

void AudioPlayerEqualizer::registerEQ() {
    if (_fxEQ) unregisterEQ();

    _fxEQ = BASS_ChannelSetFX(chan, BASS_FX_BFX_PEAKEQ, 0);

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

void AudioPlayerEqualizer::unregisterEQ() {
    if (!_fxEQ) return;
    BASS_ChannelRemoveFX(chan, _fxEQ);
    _fxEQ = 0;
}

void AudioPlayerEqualizer::setEQBand(int band, float gain) {
    BASS_BFX_PEAKEQ eq;
    eq.lBand = band;
//    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    BASS_FXSetParameters(_fxEQ, &eq);
    eqBandsGain.insert(band, gain);
}
