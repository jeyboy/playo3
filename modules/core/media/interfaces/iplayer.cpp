#include "iplayer.h"

void IPlayer::play(uint startMili) {
    if (isPaused())
        resumeProcessing();
    else
        playProcessing(startMili);
    updateState(PlayingState);
}
void IPlayer::pause() {
    pauseProcessing();
    updateState(PausedState);
}

void IPlayer::stop() {
    stopProcessing();
    updateState(StoppedState);
}

void IPlayer::slidePosForward() {
    if (seekable())
        position(qMin(max_duration, position() + max_duration / slidePercentage()));
}
void IPlayer::slidePosBackward() {
    if (seekable())
        position(qMax(uint(0), position() - max_duration / slidePercentage()));
}

void IPlayer::slideVolForward() {
    uint max = maxVolume();
    if (max == 0) return;
    volume(qMin(max, volume() + maxVolume() / slidePercentage()));
}
void IPlayer::slideVolBackward() {
    volume(qMax(uint(0), volume() - maxVolume() / slidePercentage()));
}
