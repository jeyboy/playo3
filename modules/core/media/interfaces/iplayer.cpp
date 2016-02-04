#include "iplayer.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent), ITrackable(parent),
    playPos(0), startPos(0), size(0), volumeVal(VOLUME_MULTIPLIER), panVal(0), prebuffering_level(0), muted(false), looped(false), max_duration(0)
{
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
        case StoppedState:
            updatePosition(0);
        case PausedState: {
            spectrumCalcStop();
            itimer -> stop();
        break;}
        default: channelsCount(2); // update spectrum if item opening is failed
    }
    ITrackable::updateState(isPlayed(), isPaused(), isStopped(), pstate != UnknowState);
    emit stateChanged(pstate);
}

void IPlayer::updatePosition(qint64 newPos) {
    ITrackable::setProgress(newPos);
    emit positionChanged(newPos - startPos);
}

void IPlayer::playPostprocessing() {
    if (isInitiating()) {
        initFileSize();

        if (playPos > 0 && media_url.isLocalFile())
            setPosition(playPos);
        else setPosition(startPos);

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
        setPosition(qMax(qint64(0), position() - max_duration / slidePercentage()));
}

void IPlayer::slideVolForward() {
    setVolume(qMin(VOLUME_MULTIPLIER, volume() + VOLUME_MULTIPLIER / slidePercentage()));
}
void IPlayer::slideVolBackward() {
    setVolume(qMax(0.0, volume() - VOLUME_MULTIPLIER / slidePercentage()));
}

void IPlayer::setPosition(qint64 newPos) {
    if (isInitiating()) startPos = newPos;
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
