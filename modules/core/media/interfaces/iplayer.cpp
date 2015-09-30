#include "iplayer.h"

void IPlayer::play() {
    if (isPaused())
        resumeProcessing();
    else
        playProcessing();
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

void IPlayer::setProgress(uint pos) {
    setProgressProcessing(pos);
    ITrackable::setProgress(pos);
}

void IPlayer::setMaxProgress(uint maxPos) {
    setMaxProgressProcessing(maxPos);
    ITrackable::setMaxProgress(maxPos);
}
