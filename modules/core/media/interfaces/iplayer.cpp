#include "iplayer.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent), ITrackable(parent),
    play_pos(0), start_pos(0), size(0), volume_val(VOLUME_MULTIPLIER), pan_val(0), prebuffering_level(0), muted(false), looped(false), max_duration(0)
{
    qRegisterMetaType<PlayerState>("PlayerState");
    qRegisterMetaType<PlayerStatus>("PlayerStatus");

    itimer = new QTimer(parent);
    connect(itimer, SIGNAL(timeout()), this, SLOT(recalcPosition()));
    connect(this, SIGNAL(playbackEnding()), this, SLOT(endOfPlayback()));
    itimer -> setInterval(300);
}

void IPlayer::updateState(const PlayerState & new_state) {
    switch (pstate = new_state) {
        case InitState: {emit statusChanged(media_title, InitMedia); break;}
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
        default: channelsCount(2); //INFO: update spectrum if item opening is failed
    }
    ITrackable::updateState(isPlayed(), isPaused(), isStopped(), pstate != UnknowState);
    emit stateChanged(pstate);
}

void IPlayer::updatePosition(const qint64 & new_pos) {
    ITrackable::setProgress(new_pos);
    emit positionChanged(new_pos);
}

void IPlayer::playPostprocessing() {
    if (isInitiating()) {
        initFileSize();

        if (play_pos > 0 && media_url.isLocalFile()) {
            setPosition(play_pos);
            play_pos = startPosition();
        } else setPosition(0);

        setDuration(max_duration);
        emit statusChanged(media_title, PlaingMedia);
    }
    updateState(PlayingState);
}

void IPlayer::play(bool paused) {
    bool res;
    if (isPaused()) {
        if (!(res = resumeProcessing()))
            emit statusChanged(media_title, StalledMedia);
    } else {
        if (!isInitiating()) updateState(InitState);
        size = 0;
        prebuffering_level = 0;

        if (media_url.isEmpty()) {
            emit statusChanged(media_title, NoMedia);
            return;
        } else {
            emit statusChanged(media_title, LoadingMedia);
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
    newPosProcessing(newPos);
    updatePosition(newPos);
}

void IPlayer::mute(bool enable) {
    if ((muted = enable)) {
        int val = volume_val;
        setVolume(0);
        volume_val = val;
    }
    else setVolume(volume_val == 0 ? VOLUME_MULTIPLIER : volume_val);
}
