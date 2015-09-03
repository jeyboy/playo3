#include "audio_player_panel.h"

using namespace AudioPlayer;

void Panel::setNotifyInterval(signed int milis) {
    notifyInterval = milis;
    if (notifyTimer -> isActive())
        notifyTimer -> setInterval(notifyInterval);
}

void Panel::slidePosForward() {
    if (state() == PlayingState || state() == PausedState) {
        int dur = getDuration();
        int pos = getPosition() + dur / SLIDE_DURATION_PERCENT;
        if (pos < dur)
            setPosition(pos);
    }
}
void Panel::slidePosBackward() {
    if (state() == PlayingState || state() == PausedState) {
        int pos = getPosition() - (getDuration() / SLIDE_DURATION_PERCENT);
        if (pos < 0) pos = 0;
        setPosition(pos);
    }
}

void Panel::slideVolForward() {
    int newVolLevel = getVolume() + SLIDE_VOLUME_OFFSET;
    if (newVolLevel > VOLUME_MULTIPLIER) newVolLevel = VOLUME_MULTIPLIER;
    setChannelVolume(newVolLevel);
}
void Panel::slideVolBackward() {
    int newVolLevel = getVolume() - SLIDE_VOLUME_OFFSET;
    if (newVolLevel < 0) newVolLevel = 0;
    setChannelVolume(newVolLevel);
}

float Panel::getBpmValue(QUrl uri) {
    int cochan;

    if (uri.isLocalFile())
        cochan = BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN | BASS_SAMPLE_MONO);
    else
        cochan = BASS_StreamCreateURL(uri.toString().toStdString().c_str(), 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_SAMPLE_MONO, NULL, 0);

    if (cochan) {
        int playBackDuration = BASS_ChannelBytes2Seconds(cochan, BASS_ChannelGetLength(cochan, BASS_POS_BYTE));

        return BASS_FX_BPM_DecodeGet(cochan,
            0,
            playBackDuration,
            MAKEWORD(20, 180),
            BASS_FX_FREESOURCE, //BASS_FX_BPM_BKGRND // BASS_FX_BPM_MULT2
            NULL, NULL
        );
    } else return 0;
}
