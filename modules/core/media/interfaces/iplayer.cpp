#include "iplayer.h"
#include "settings.h"

IPlayer::IPlayer(QWidget * parent) : IEqualizable(parent),
    start_pos(0), size(0), volume_val(volumeDefault()), pan_val(panDefault()),
    tempo_val(tempoDefault()), downloading_level(0), muted(false), looped(false),
    play_pos(0), max_duration(0)
{
    qRegisterMetaType<PlayerState>("PlayerState");
    qRegisterMetaType<PlayerStatus>("PlayerStatus");

    itimer = new QTimer(parent);
    connect(itimer, SIGNAL(timeout()), this, SLOT(recalcPosition()));
    connect(this, SIGNAL(playbackEnding()), this, SLOT(endOfPlayback()));
    connect(this, SIGNAL(stateChanged(PlayerState)), this, SLOT(updateStatus(PlayerState)));
    itimer -> setInterval(300);

    setSpectrumFreq(Settings::obj().spectrumFreqRate());
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

    emit stateChanged(pstate);
}

void IPlayer::updatePosition(const qint64 & new_pos) {
    ITrackable::setProgress(new_pos);
    emit positionChanged(new_pos);
}

void IPlayer::playPostprocessing(const bool & paused) {
    if (isInitiating()) {
        if (paused) pause();

        if (max_duration == 0)
            setDuration(calcDuration());
        initFileSize();

        if (play_pos > 0 && media_url.isLocalFile()) {
            setPosition(play_pos);
            play_pos = startPosition();
        } else setPosition(0);

        setDuration(max_duration);

        emit videoOutputRequired(hasVideo());
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
        downloading_level = 0;

        if (media_url.isEmpty()) {
            emit statusChanged(media_title, NoMedia);
            return;
        } else {
            emit statusChanged(media_title, LoadingMedia);
            res = playProcessing(paused);
        }
    }

    if (res) playPostprocessing(paused);
}
void IPlayer::pause() {
   if (pauseProcessing())
        updateState(PausedState);
}

void IPlayer::stop() {
    if (stopProcessing())
        updateState(StoppedState);
}

qreal IPlayer::slidePercentage() const { return Settings::obj().slidePercent(); }

void IPlayer::slidePosForward() {
    if (isSeekable())
        setPosition(qMin(max_duration, (qint64)(position() + max_duration * slidePercentage())));
}
void IPlayer::slidePosBackward() {
    if (isSeekable())
        setPosition(qMax(qint64(0), (qint64)(position() - max_duration * slidePercentage())));
}

void IPlayer::slideVolForward() {
    setVolume(qMin(volumeMax(), (int)(volume() + volumeMax() * slidePercentage())));
}
void IPlayer::slideVolBackward() {
    setVolume(qMax(volumeMin(), (int)(volume() - volumeMax() * slidePercentage())));
}

void IPlayer::setPosition(const qint64 & new_pos) {
    newPosProcessing(new_pos);
    updatePosition(new_pos);
}

void IPlayer::mute(const bool & enable) {
    if ((muted = enable)) {
        int val = volume_val;
        setVolume(0);
        volume_val = val;
    }
    else setVolume(volume_val == volumeMin() ? volumeMax() : volume_val);
}
