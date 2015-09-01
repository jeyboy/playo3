#include "player.h"

Player * Player::self = 0;

Player * Player::instance(QWidget * parent) {
    if(!self)
        self = new Player(parent);
    return self;
}

Player::Player(QWidget * parent) : AudioPlayer(parent), slider(0), volumeSlider(0), timePanel(0), playButton(0), pauseButton(0),
    stopButton(0), likeButton(0), muteButton(0), prevVolumeVal(0), time_forward(true), extended_format(true), current_model(0), current_item(0)
{
    #ifdef Q_OS_WIN
        stateButton = new QWinTaskbarButton(parent);
        parent -> winId(); // generate native object for windowHandle()
        stateButton -> setWindow(parent -> windowHandle());
        stateProgress = stateButton -> progress();
        stateProgress -> setMinimum(0);
    #endif

    setNotifyInterval(500);
    connect(this, SIGNAL(stateChanged(MediaState)), this, SLOT(onStateChanged(MediaState)));
    connect(this, SIGNAL(mediaStatusChanged(MediaStatus)), this, SLOT(onMediaStatusChanged(MediaStatus)));
    connect(this, SIGNAL(volumeChanged(int)), this, SLOT(unmuteCheck(int)));
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
void Player::setCyclingButton(QAction * cycleAction) {
    cycleButton = cycleAction;
    cycleButton -> setCheckable(true);
//    connect(cycleButton, SIGNAL(triggered(bool)), this, SLOT(stop()));
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
    QModelIndex ind = playedIndex();
    if (ind.isValid()) {
        QAbstractItemModel * mdl = const_cast<QAbstractItemModel *>(ind.model());
        mdl -> setData(ind, state, ISTATE);
    }
}

void Player::updateItemState(bool isPlayed) {
    setItemState(isPlayed ? (ItemState::listened | ItemState::played) : -ItemState::played);
}

void Player::eject(bool updateState) {
    if (!updateState) {
        current_model = 0;
        current_item = 0;
    }
    playIndex(QModelIndex());
}

bool Player::playIndex(QModelIndex item, bool paused, uint start, int duration) {
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

    if (item.isValid()) {
        current_model = (IModel *)item.model();
        current_item = current_model -> item(item);

        likeButton -> setChecked(current_item -> is(ItemState::liked));

        setMedia(current_item -> toUrl(), start, duration);

        if (!paused)
            play();
        else onStateChanged(PausedState);

        if (isPlayed()) {
            updateItemState(true);
        } else {
            retVal = paused || false;
            setItemState(ItemState::played);
        }
    } else {
        current_model = 0;
        current_item = 0;

        duration = 0;
        setTimePanelVal(0);
    }

    return retVal;
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

QModelIndex Player::playedIndex() {
    return current_model ? current_model -> index(current_item) : QModelIndex();
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
    #ifdef Q_OS_WIN
        stateProgress -> setVisible(stopped);

        if (!stopped)
            stateButton -> setOverlayIcon(QIcon());
        else
            stateButton -> setOverlayIcon(QIcon(
                !played ? ":task_play" : ":task_pause"
            ));

        if (!played) {
            stateProgress -> resume();
            stateButton -> setOverlayAccessibleDescription(current_item -> title().toString());
        }

        if (!paused)
            stateProgress -> pause();
    #endif


    playButton -> setVisible(played);
    pauseButton -> setVisible(paused);
    stopButton -> setVisible(stopped);
    likeButton -> setVisible(!(played && !stopped && !paused));
}

bool Player::getFileInfo(QUrl uri, MediaInfo * info) {
    int chUID;

    if (uri.isLocalFile()) {
        chUID = BASS_StreamCreateFile(false,
            uri.toLocalFile()
              #ifdef Q_OS_WIN
                .toStdWString().data()
              #else
                .toStdString().c_str()
              #endif
            , 0, 0, 0);
    } else {
        chUID = BASS_StreamCreateURL(
            uri.toString()
                #ifdef Q_OS_WIN
                    .toStdWString().data()
                #else
                    .toStdString().c_str()
                #endif
            , 0, 0, NULL, 0);
    }

//    if (uri.isLocalFile())
//        chUID = BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, 0);
//    else
//        chUID = BASS_StreamCreateURL(uri.toString().toStdWString().data(), 0, 0, NULL, 0);

    if (!chUID) return false;

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
    return true;
}

void Player::playedIndexIsInvalid() {
    if (current_model)
        current_model -> itemError(playedIndex());
}

//////////////////////SLOTS/////////////////////////

void Player::invertTimeCountdown() {
    time_forward = !time_forward;
}

void Player::setTrackbarValue(int pos) {
    #ifdef Q_OS_WIN
        stateProgress -> setValue(pos);
    #endif

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
        positionChanged(startPos);

        #ifdef Q_OS_WIN
            stateProgress -> setVisible(true);
            stateProgress -> setMaximum(duration);
        #endif
    }
}

void Player::playPause() {
    if (state() == PlayingState)
        pause();
    else
        start();
}

void Player::start() {
    if (!current_item)
        emit nextItemNeeded(init);
    else play();
}

void Player::like() {
    if (current_item -> is(ItemState::liked))
        setItemState(-ItemState::liked);
    else
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
    #ifdef Q_OS_WIN
        stateProgress -> setValue(pos);
    #endif

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

            if (current_item && current_item -> is(ItemState::proccessing))
                emit mediaStatusChanged(LoadedMedia);
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
//    connect(Player::instance(), SIGNAL(itemExecError(QModelIndex)), this, SLOT(itemError(QModelIndex)));
//    connect(Player::instance(), SIGNAL(itemNotSupported(QModelIndex)), this, SLOT(itemNotSupported(QModelIndex)));
//    connect(Player::instance(), SIGNAL(itemNotAccessable(QModelIndex)), this, SLOT(itemNotExist(QModelIndex)));
//    connect(Player::instance(), SIGNAL(itemNotExisted(QModelIndex)), this, SLOT(itemNotExist(QModelIndex)));


    switch (status) {
        case UnknownMediaStatus: {
//            qDebug() << "PLAYER: " << "UNKNOWN";
            endProccessing();
            emit nextItemNeeded(error);
        break; }

        case StalledMedia: {
//            qDebug() << "PLAYER: " << "STALLED";
            if (current_item -> isRemote()) {
                emit nextItemNeeded(refreshNeed);
            } else {
                endProccessing();
                playedIndexIsInvalid();
                emit nextItemNeeded(stalled);
            }
        break; }

        case EndOfMedia: {
            setTrackbarValue(0);
//            qDebug() << "PLAYER: " << "END";
            if (cycleButton -> isChecked())
                play();
            else
                emit nextItemNeeded(endMedia);
        break;}

        case LoadedMedia: {
            endProccessing();
        break;}

        case InvalidMedia: {
//            qDebug() << "PLAYER: " << "INVALID";
//            emit itemNotSupported(playedIndex());
            current_model -> itemNotSupported(playedIndex());
            endProccessing();
            emit nextItemNeeded(error);
        break;}

        case NoMedia: {
//            qDebug() << "PLAYER: " << "NO MEDIA";
//            emit itemNotExisted(playedIndex());

            if (current_item -> isRemote()) {
                emit nextItemNeeded(refreshNeed);
            } else {
                endProccessing();
                current_model -> itemNotExist(playedIndex());
                emit nextItemNeeded(noMedia);
            }
        break;}
        default: {  }
    }
}
