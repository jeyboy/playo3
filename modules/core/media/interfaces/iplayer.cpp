#include "iplayer.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent), ITrackable(parent), max_duration(0) {
//        qRegisterMetaType<PlayerState>("PlayerState");
//        qRegisterMetaType<PlayerStatus>("PlayerStatus");

    itimer = new QTimer(parent);
    connect(itimer, SIGNAL(timeout()), this, SLOT(recalcPosition()));
    itimer -> setInterval(500);
}

void IPlayer::updateState(PlayerState new_state) {
    switch (pstate = new_state) {
        case PlayingState: {
            itimer -> start();
            spectrumCalcStart();
        break;}
        case PausedState:
        case StoppedState: {
            spectrumCalcStop();
            itimer -> stop();
        break;}
        default: ;
    }
    ITrackable::updateState(isPlayed(), isPaused(), isStopped());
    emit stateChanged(pstate);
}

void IPlayer::play(uint startMili, uint maxDuration) {
    bool res;
    if (isPaused())
        res = resumeProcessing();
    else {
        duration(maxDuration);
        res = playProcessing(startMili);
    }

    if (res) playPostprocessing();
}
void IPlayer::pause() {
   if (pauseProcessing())
        updateState(PausedState);
}

void IPlayer::stop() {
    if (stopProcessing())
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

void IPlayer::position(uint newPos) {
    newPosProcessing(newPos);
    ITrackable::setProgress(newPos);
    emit positionChanged(newPos);
}
