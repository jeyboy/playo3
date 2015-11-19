#include "iplayer.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent), ITrackable(parent), startPos(0), volumeVal(VOLUME_MULTIPLIER), size(0), prebuffering_level(0), muted(false), looped(false), max_duration(0) {
    qRegisterMetaType<PlayerState>("PlayerState");
    qRegisterMetaType<PlayerStatus>("PlayerStatus");

    itimer = new QTimer(parent);
    connect(itimer, SIGNAL(timeout()), this, SLOT(recalcPosition()));
    connect(this, SIGNAL(playbackEnding()), this, SLOT(endOfPlayback()));
    itimer -> setInterval(500);
}

void IPlayer::updateState(PlayerState new_state) {
    switch (pstate = new_state) {
        case InitState: {emit statusChanged(InitMedia); break;}
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
    ITrackable::updateState(isPlayed(), isPaused(), isStopped(), pstate != UnknowState);
    emit stateChanged(pstate);
}

void IPlayer::updatePosition(int newPos) {
    ITrackable::setProgress(newPos);
    emit positionChanged(newPos);
}

void IPlayer::playPostprocessing() {
    if (isInitiating()) {
        initFileSize();
        if (startPos > 0 && media_url.isLocalFile()) setPosition(startPos); // sets start offset only for local files
        setDuration(max_duration);
        emit statusChanged(PlaingMedia);
    }
    updateState(PlayingState);
}

void IPlayer::play(bool paused) {
    bool res;
    if (isPaused()) {
        if (!(res = resumeProcessing()))
            emit statusChanged(StalledMedia);
    } else {
        if (!isInitiating()) updateState(InitState);
        size = 0;
        prebuffering_level = 0;

        if (media_url.isEmpty()) {
            emit statusChanged(NoMedia);
            return;
        } else {
            emit statusChanged(LoadingMedia);
            res = playProcessing(paused);
        }
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
        setPosition(qMax(int(0), position() - max_duration / slidePercentage()));
}

void IPlayer::slideVolForward() {
    setVolume(qMin(VOLUME_MULTIPLIER, volume() + VOLUME_MULTIPLIER / slidePercentage()));
}
void IPlayer::slideVolBackward() {
    setVolume(qMax(0.0, volume() - VOLUME_MULTIPLIER / slidePercentage()));
}

void IPlayer::setPosition(int newPos) {
    newPosProcessing(newPos);
    updatePosition(newPos);
}

void IPlayer::mute(bool enable) {
    if ((muted = enable)) {
        int val = volumeVal;
        setVolume(0);
        volumeVal = val;
    }
    else setVolume(volumeVal);
}
