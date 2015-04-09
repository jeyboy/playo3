#include "player.h"
#include <qdebug.h>

Player * Player::self = 0;

Player * Player::instance(QObject * parent) {
    if(!self)
        self = new Player(parent);
    return self;
}

void Player::setPlayButton(QAction * playAction) {
    playButton = playAction;
    playButton -> setVisible(true);
    connect(playAction, SIGNAL(triggered(bool)), this, SLOT(start()));
}
void Player::setPauseButton(QAction * pauseAction) {
    pauseButton = pauseAction;
    pauseButton -> setVisible(false);
    connect(pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause()));
}
void Player::setStopButton(QAction * stopAction) {
    stopButton = stopAction;
    stopButton -> setVisible(false);
    connect(stopAction, SIGNAL(triggered(bool)), this, SLOT(stop()));
}

void Player::setLikeButton(QAction * likeAction) {
    likeButton = likeAction;
    likeButton -> setVisible(false);
    connect(likeAction, SIGNAL(triggered(bool)), this, SLOT(like()));
}

void Player::setMuteButton(QAction * muteAction) {
    muteButton = muteAction;
    connect(muteButton, SIGNAL(triggered(bool)), this, SLOT(mute()));
}

void Player::setItemState(int state) {
    if (currentIndex.isValid()) {
        QAbstractItemModel * mdl = const_cast<QAbstractItemModel *>(currentIndex.model());

        mdl -> setData(
            currentIndex,
            state,
            ISTATE
        );
    }
}

void Player::updateItemState(bool isPlayed) {
    setItemState(isPlayed ? (ItemState::listened | ItemState::played) : -ItemState::played);
}

bool Player::playIndex(QModelIndex item, bool paused, uint start) {
    bool retVal = true;
    switch(state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
    }

    updateItemState(false);
    currentIndex = item;
    if (item.isValid()) {
        setMedia(item.data(IURL).toUrl());
        play();

        if (start > 0)
            setStartPosition(start);

        if (isPlayed())
            updateItemState(true);
        else
            retVal = false;
            setItemState(ItemState::played);

        if (paused)
            pause();
    } else {
        duration = 0;
        setTimePanelVal(0);
    }

    return retVal;
}

void Player::setStartPosition(int position) {
    setPosition(position);
    emit positionChanged(position);
}

void Player::setTrackBar(QSlider * trackBar) {
    slider = qobject_cast<Playo3::MetricSlider *>(trackBar);
    slider -> setMinimum(0);
    slider -> setMaximum(0);

    connect(this, SIGNAL(positionChanged(int)), this, SLOT(setTrackbarValue(int)));
    connect(this, SIGNAL(durationChanged(int)), this, SLOT(setTrackbarMax(int)));

    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(changeTrackbarValue(int)));
}

void Player::setVolumeTrackBar(QSlider * trackBar) {
    volumeSlider = trackBar;
    connect(trackBar, SIGNAL(valueChanged(int)), this, SLOT(setChannelVolume(int)));
    connect(this, SIGNAL(volumeChanged(int)), this, SLOT(setVolTrackbarValue(int)));

    volumeSlider -> setMaximum(10000);
    volumeSlider -> setValue(10000);
}

void Player::setTimePanel(ClickableLabel * newTimePanel) {
    timePanel = newTimePanel;
    connect(timePanel, SIGNAL(clicked()), this, SLOT(invertTimeCountdown()));
}

//void Player::setVideoOutput(QVideoWidget * container) {
//    setVideoOutput(container);
//}

void Player::setTimePanelVal(int millis) {
    if (timePanel) {
        QString val, total;
        total = Duration::fromMillis(getDuration(), extended_format);

        if (time_forward) {
            val = Duration::fromMillis(millis, extended_format);
            timePanel -> setText(val + "\n" + total);
        } else {
            val = Duration::fromMillis(getDuration() - millis, extended_format);
            timePanel -> setText(total + "\n" + val);
        }
    }
}

void Player::updateControls(bool played, bool paused, bool stopped) {
    playButton -> setVisible(played);
    pauseButton -> setVisible(paused);
    stopButton -> setVisible(stopped);
    likeButton -> setVisible(!(played && !stopped && !paused));
}

void Player::getFileInfo(QUrl uri, MediaInfo * info) {
    int chUID;

    if (uri.isLocalFile())
        chUID = BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, 0);
    else
        chUID = BASS_StreamCreateURL(uri.toString().toStdString().c_str(), 0, 0, NULL, 0);

    if (!chUID) return;

    float time = BASS_ChannelBytes2Seconds(chUID, BASS_ChannelGetLength(chUID, BASS_POS_BYTE)); // playback duration
    DWORD len = BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_END); // file length

    info -> setDuration(time);
    info -> setBitrate((len / (125 * time) + 0.5));

    BASS_CHANNELINFO media_info;
    if (BASS_ChannelGetInfo(chUID, &media_info)) {
        info -> setSize(len + BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_START));
//        ret.insert("info", Format::toInfo(Format::toUnits(size), bitrate, info.freq, info.chans));
        info -> setSampleRate(media_info.freq);
        info -> setChannels(media_info.chans);
    }

    BASS_StreamFree(chUID);
}

//////////////////////SLOTS/////////////////////////

void Player::invertTimeCountdown() {
    time_forward = !time_forward;
}

void Player::setTrackbarValue(int pos) {
    setTimePanelVal(pos);

    slider -> blockSignals(true);
    slider -> setValue(pos);
    slider -> blockSignals(false);
}

void Player::setVolTrackbarValue(int pos) {
    volumeSlider -> blockSignals(true);
    volumeSlider -> setValue(pos);
    volumeSlider -> blockSignals(false);
}

void Player::setTrackbarMax(int duration) {
    if (slider) {
        extended_format = Duration::hasHours(duration);
//        slider -> setDisabled(!isSeekable());
        slider -> setMaximum(duration);
    }
}

void Player::playPause() {
    if (state() == PlayingState)
        pause();
    else
        start();
}

void Player::start() {
    if (!currentIndex.isValid())
        emit nextItemNeeded(init);
    else play();
}

void Player::like() {
    setItemState(ItemState::liked);
}

void Player::mute() {
    int curr = getVolume();
    setChannelVolume(prevVolumeVal);

    if (muteButton == 0) return;
    prevVolumeVal = curr;
}

void Player::unmuteCheck(int val) {
    if (val != 0)
        muteButton -> setChecked(false);
    prevVolumeVal = 0;
}

void Player::changeTrackbarValue(int pos) {
    emit setPosition(pos);
}

void Player::onStateChanged(MediaState newState) {
    switch(newState) {
        case StoppedState: {
            slider -> blockSignals(true);
            slider -> setMaximum(0);
            setTimePanelVal(0);
            slider -> blockSignals(false);
            updateControls(true, false, false);
            break;
        }

        case PlayingState: {
            if (getDuration() != -1)
                slider -> setMaximum(getDuration());

            updateControls(false, true, true);
            break;
        }

        case PausedState: {
            updateControls(true, false, true);
            break;
        }
    }
}

void Player::onMediaStatusChanged(MediaStatus status) {
    switch (status) {
        case UnknownMediaStatus: {
        break; }

        case StalledMedia: {
            emit itemExecError(currentIndex);
            emit nextItemNeeded(error);
        break; }

        case EndOfMedia: {
            emit nextItemNeeded(endMedia);
            break;
        }
        case InvalidMedia: {
            emit itemNotSupported(currentIndex);
            emit nextItemNeeded(error);
        break;}
        default: {  }
    }
}
