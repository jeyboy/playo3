#include "player.h"

Player * Player::self = 0;

Player * Player::instance(QObject * parent) {
    if(!self)
        self = new Player(parent);
    return self;
}

//ModelItem * Player::playedItem() const {
//    return played;
//}
//View * Player::currentPlaylist() const {
//    return playlist;
//}
//View * Player::currentActivePlaylist() const {
//    return playlist;
//}


//void Player::setActivePlaylist(View * newActivePlaylist) {
//    activePlaylist = newActivePlaylist;
//}

void Player::setPlayButton(QAction * playAction) {
    playButton = playAction;
    playButton -> setVisible(true);
//    connect((QObject *)playAction, SIGNAL(triggered(bool)), instance(), SLOT(play()));
    connect((QObject *)playAction, SIGNAL(triggered(bool)), this, SLOT(start()));
}
void Player::setPauseButton(QAction * pauseAction) {
    pauseButton = pauseAction;
    pauseButton -> setVisible(false);
    connect((QObject *)pauseAction, SIGNAL(triggered(bool)), this, SLOT(pause()));
}
void Player::setStopButton(QAction * stopAction) {
    stopButton = stopAction;
    stopButton -> setVisible(false);
    connect((QObject *)stopAction, SIGNAL(triggered(bool)), this, SLOT(stop()));
}

void Player::setLikeButton(QAction * likeAction) {
    likeButton = likeAction;
    likeButton -> setVisible(false);
    connect((QObject *)likeAction, SIGNAL(triggered(bool)), this, SLOT(like()));
}

void Player::setMuteButton(QAction * muteAction) {
    muteButton = muteAction;
    connect((QObject *)muteButton, SIGNAL(triggered(bool)), this, SLOT(mute()));
}

void Player::updateItemState(bool isPlayed) {
    if (currentItem.isValid()) {
        QAbstractItemModel * mdl = const_cast<QAbstractItemModel *>(currentItem.model());

        if (isPlayed) {
            mdl -> setData(currentItem, ItemState::listened | ItemState::played, STATEID);
//            played -> setState(STATE_LISTENED | STATE_PLAYED);
        } else {
            mdl -> setData(currentItem, -ItemState::played, STATEID);
//            played -> setState(-STATE_PLAYED, false);
        }
//        currentItem.data(UPDATEID);
    }
}

void Player::playItem(QModelIndex item, bool paused) {
    switch(state()) {
        case StoppedState: { break; }

        case PausedState:
        case PlayingState: {
            stop();
            break;
        }
    }

    currentItem = item;
    setMedia(url);
    play();

    if (paused)
        pause();
}

void Player::setStartPosition(int position) {
    setPosition(position);
    emit positionChanged(position);
}

void Player::setTrackBar(QSlider * trackBar) {
    slider = trackBar;
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

//void Player::initFormat(int millis) {
//    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
//    extended_format = h > 0;
//}

//QString Player::intToStr(int millis) {
//    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
//    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;

//    if (extended_format) {
//        int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
//        return QString().sprintf("%02d:%02d:%02d", h, m, s);
//    } else {
//        return QString().sprintf("%02d:%02d", m, s);
//    }
//}

void Player::updateControls(bool played, bool paused, bool stopped) {
    playButton -> setVisible(played);
    pauseButton -> setVisible(paused);
    stopButton -> setVisible(stopped);
    likeButton -> setVisible(!(played && !stopped && !paused));
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
    play();

//    if ((played == 0 || playlist != activePlaylist) && activePlaylist != 0) {
//        activePlaylist -> proceedNext();
//    } else play();
}

void Player::like() {
//    played -> setState(STATE_LIKED);
}

void Player::mute() {
    int curr = getVolume();
    setChannelVolume(prevVolumeVal);

    if (muteButton == 0) return;
    prevVolumeVal = curr;
}

void Player::unmuteCheck(int) {
//    muteButton -> setChecked(false);
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

//            updateItemState(true);
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
        break; }

        case EndOfMedia:
        case InvalidMedia: {
//            if (playlist) {
//                if (playlist -> isPlaylist()) {
//                    playlist -> proceedNext();
//                }
//            }
            break;
        }
        default: {  }
    }
}
