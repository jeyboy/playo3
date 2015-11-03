#include "bass_player.h"

#include <qapplication.h>

void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    BassPlayer * player = static_cast<BassPlayer *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    BassPlayer * player = static_cast<BassPlayer *>(user);
    player -> prebufferingLevel();
}

void BassPlayer::proceedErrorState() {
    updateState(UnknowState);
    switch(BASS_ErrorGetCode()) {
        case BASS_ERROR_FILEFORM: { emit statusChanged(InvalidMedia); break; }
        case BASS_ERROR_FILEOPEN: { emit statusChanged(NoMedia); break; }
        // BASS_ERROR_TIMEOUT
        default: emit statusChanged(StalledMedia);
    }
}

int BassPlayer::openChannel(const QUrl & url, QFutureWatcher<int> * watcher) {
    int new_chan;

    if (url.isLocalFile()) {
//        size = 0;
        new_chan = open(url.toLocalFile(), LOCAL_PLAY_ATTRS);
        prebufferingChanged(1);
    } else {
//        size = -1;
        //    "http://www.asite.com/afile.mp3\r\nCookie: mycookie=blah\r\n"
        new_chan = openRemote(url.toString(), REMOTE_PLAY_ATTRS);
    }

    if (!new_chan)
        qDebug() << "OPEN ERROR" << url.toString() << BASS_ErrorGetCode();

    if (watcher -> isCanceled())
        BASS_StreamFree(new_chan);


    //TODO: need to realise proc of situation when timeout is to short - because now this is fired refresh many times

    return new_chan;
}

void BassPlayer::afterSourceOpening() {
    QFutureWatcher<int> * watcher = (QFutureWatcher<int> *)sender();

    if (!watcher -> isCanceled()) {
        chan = watcher -> result();
        if (chan) playPreproccessing();
        else proceedErrorState();
    } else emit statusChanged(LoadedMedia);

    watcher -> deleteLater();
    if (watcher == openChannelWatcher)
        openChannelWatcher = 0;
}

void BassPlayer::playPreproccessing() {
    emit statusChanged(LoadedMedia);
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volume());
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, pan());

    if (max_duration == 0)
        setDuration(round(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE))) * POSITION_MULTIPLIER);


    BASS_CHANNELINFO info;
    if (BASS_ChannelGetInfo(chan, &info))
        channelsCount(info.chans);
    else
        channelsCount(2);

    if (eqInUse) registerEQ();

    if (BASS_ChannelPlay(chan, true))
        playPostprocessing();

    syncHandle = BASS_ChannelSetSync((HSYNC)chan, BASS_SYNC_END, 0, &endTrackSync, this);
    syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

    if (startPos > 0) setPosition(startPos);
    if (is_paused) pause();
}

bool BassPlayer::playProcessing(int startMili, bool paused) {
    startPos = startMili;
    is_paused = paused;

    stop();

    if (!media_url.isEmpty()) {
        if (openChannelWatcher)
            openChannelWatcher -> cancel();

        openChannelWatcher = new QFutureWatcher<int>();
        connect(openChannelWatcher, SIGNAL(finished()), this, SLOT(afterSourceOpening()));
        openChannelWatcher -> setFuture(QtConcurrent::run(this, &BassPlayer::openChannel, media_url, openChannelWatcher));
    }

    return false; // skip inherited actions
}
bool BassPlayer::resumeProcessing() {
    if (!BASS_ChannelPlay(chan, false)) {
        qDebug() << "Error resuming";
        return false;
    }

    return true;
}
bool BassPlayer::pauseProcessing() { return BASS_ChannelPause(chan); }
bool BassPlayer::stopProcessing() {
    if (BASS_ChannelStop(chan)) {
        unregisterEQ();
        BASS_ChannelRemoveSync(chan, syncHandle);
        BASS_ChannelRemoveSync(chan, syncDownloadHandle);
        BASS_StreamFree(chan);
        return true;
    } else {
        qDebug() << "Error while stopping";
        return false;
    }
}

int BassPlayer::recalcCurrentPosProcessing() {
    return BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * POSITION_MULTIPLIER;
}
bool BassPlayer::newPosProcessing(int newPos) {
    return BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, newPos / POSITION_MULTIPLIER), BASS_POS_BYTE);
}
bool BassPlayer::newVolumeProcessing(int newVol) {
    int volumeVal = newVol > 0 ? (newVol / VOLUME_MULTIPLIER) : 0;
    return  BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
}
bool BassPlayer::newPanProcessing(int newPan) {
    int panVal = newPan > 0 ? (newPan / PAN_MULTIPLIER) : 0;
    return BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, panVal);
}


bool BassPlayer::registerEQ() {
    if (_fxEQ) unregisterEQ();

    if (_fxEQ = BASS_ChannelSetFX(chan, BASS_FX_BFX_PEAKEQ, 0)) {
        BASS_BFX_PEAKEQ eq;
        eq.fQ = 0;
        eq.fBandwidth = currentPresetBase();
        eq.lChannel = BASS_BFX_CHANALL;

        QMap<float, QString> eqBands = bands();
        QMap<float, QString>::Iterator band = eqBands.begin();
        for(int num = 0; band != eqBands.end(); band++, num++) {
            eq.fGain = eqBandsGains.value(num, 0);
            eq.lBand = num; eq.fCenter = band.key(); BASS_FXSetParameters(_fxEQ, &eq);
        }
    }

    return _fxEQ != 0;
}
bool BassPlayer::unregisterEQ() {
    if (!_fxEQ) return false;
    bool res = BASS_ChannelRemoveFX(chan, _fxEQ);
    _fxEQ = 0;
    return res;
}
bool BassPlayer::processEqSetGain(int band, float gain) {
    BASS_BFX_PEAKEQ eq;
    eq.lBand = band;
    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    return BASS_FXSetParameters(_fxEQ, &eq);
}

bool BassPlayer::calcSpectrum(QVector<int> & result) {}
bool BassPlayer::calcSpectrum(QList<QVector<int> > & result) {}

bool BassPlayer::fileInfo(const QUrl & uri, IMediaInfo * info) {
    int chUID;

    if (uri.isLocalFile())
        chUID = open(uri.toLocalFile(), LOCAL_PLAY_ATTRS);
    else
        chUID = openRemote(uri.toString(), REMOTE_PLAY_ATTRS);

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

float BassPlayer::bpmCalc(const QUrl & uri) {
    int cochan;

    if (uri.isLocalFile())
        cochan = open(uri.toLocalFile(), LOCAL_BPM_ATTRS);// BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, LOCAL_BPM_ATTRS);
    else
        cochan = openRemote(uri.toString(), REMOTE_BPM_ATTRS); //BASS_StreamCreateURL(uri.toString().toStdString().c_str(), 0, REMOTE_BPM_ATTRS, NULL, 0);

    if (cochan) {
        int playBackDuration = BASS_ChannelBytes2Seconds(cochan, BASS_ChannelGetLength(cochan, BASS_POS_BYTE));

        return BASS_FX_BPM_DecodeGet(cochan,
            0,
            playBackDuration,
            MAKEWORD(20, 180),
            BASS_FX_FREESOURCE, //BASS_FX_BPM_BKGRND // BASS_FX_BPM_MULT2
            NULL, NULL
        );
    } else return 0;
}

BassPlayer::BassPlayer(QWidget * parent, float open_time_out_sec) : IPlayer(parent), openChannelWatcher(0) {
    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        throw "An incorrect version of BASS.DLL was loaded";

    if (HIWORD(BASS_FX_GetVersion()) != BASSVERSION)
        throw "An incorrect version of BASS_FX.DLL was loaded";

    if (!BASS_Init(default_device(), 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";

    ///////////////////////////////////////////////
    /// load plugins
    ///////////////////////////////////////////////
    QFileInfoList list = QDir(QCoreApplication::applicationDirPath() % QStringLiteral("/bass_plugins")).entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    QFileInfoList::Iterator it = list.begin();

    for(; it != list.end(); it++) {
        /*int res = */BASS_PluginLoad((*it).filePath().toLatin1(), 0);

//        if (res == 0)
//            qDebug() << file.filePath() << BASS_ErrorGetCode();
//        else
//            qDebug() << file.filePath() << res;
    }
    ///////////////////////////////////////////////

    BASS_SetConfig(BASS_CONFIG_FLOATDSP, TRUE);
    BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 15); // 15 percents prebuf

    openTimeOut(open_time_out_sec);
}

BassPlayer::~BassPlayer() {
    if (openChannelWatcher)
        openChannelWatcher -> cancel();
    openChannelWatcher -> deleteLater();

    stop();
    BASS_PluginFree(0);
    BASS_Free();
}

int BassPlayer::position() const { return BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * POSITION_MULTIPLIER; }
