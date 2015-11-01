#include "iplayer.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent), ITrackable(parent), prebuffering_level(0), muted(false), max_duration(0) {
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

void IPlayer::playPostprocessing() {
    if (isStopped())
        emit statusChanged(StartOfMedia);
    updateState(PlayingState);
}

void IPlayer::play(uint startMili, bool paused, uint maxDuration) {
    bool res;
    if (isPaused()) {
        if (!(res = resumeProcessing()))
            emit statusChanged(StalledMedia);
    } else {
        prebuffering_level = 0;
        emit statusChanged(media_url.isEmpty() ? NoMedia : LoadingMedia);
        duration(maxDuration);       
        res = playProcessing(startMili, paused);
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
        setPosition(qMin(max_duration, position() + max_duration / slidePercentage()));
}
void IPlayer::slidePosBackward() {
    if (seekable())
        setPosition(qMax(quint64(0), position() - max_duration / slidePercentage()));
}

void IPlayer::slideVolForward() {
    uint max = maxVolume();
    if (max == 0) return;
    setVolume(qMin(max, volume() + maxVolume() / slidePercentage()));
}
void IPlayer::slideVolBackward() {
    setVolume(qMax(uint(0), volume() - maxVolume() / slidePercentage()));
}

void IPlayer::setPosition(quint64 newPos) {
    newPosProcessing(newPos);
    ITrackable::setProgress(newPos);
    emit positionChanged(newPos);
}

void IPlayer::mute(bool enable) {
    if ((muted = enable)) {
        uint val = volumeVal;
        setVolume(0);
        volumeVal = val;
    }
    else setVolume(volumeVal);
}
