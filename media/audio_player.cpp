#include "audio_player.h"
#include "misc/settings.h"
#include <qdebug.h>

//Get the percentage downloaded of an internet file stream, or the buffer level when streaming in blocks.
//QWORD len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END); // file/buffer length
//QWORD buf=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_BUFFER); // buffer level
//float progress=buf*100.0/len; // percentage of buffer filled

//void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user)
void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    AudioPlayer * player = static_cast<AudioPlayer *>(user);
    player -> finishRemoteFileDownloading();
    emit player -> downloadEnded();
}

AudioPlayer::AudioPlayer(QObject * parent) : QObject(parent), duration(-1), notifyInterval(100),
    channelsCount(2), prevChannelsCount(0), volumeVal(1.0), spectrumHeight(0), defaultSpectrumLevel(0),
    currentState(StoppedState), _fxEQ(0), useEQ(false) {
    qRegisterMetaType<AudioPlayer::MediaStatus>("MediaStatus");
    qRegisterMetaType<AudioPlayer::MediaState>("MediaState");

    // cheat for cross treadhing
    connect(this, SIGNAL(playbackEnded()), this, SLOT(endOfPlayback()));

    setSpectrumBandsCount(28);

    if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS.DLL was loaded";
    }

    if (HIWORD(BASS_FX_GetVersion()) != BASSVERSION) {
        throw "An incorrect version of BASS_FX.DLL was loaded";
    }

    #ifdef Q_OS_WIN
    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";
    #else
    if (!BASS_Init(BASS_DEVICE_DEFAULT, 44100, 0, NULL, NULL))
        qDebug() << "Init error: " << BASS_ErrorGetCode();
//        throw "Cannot initialize device";
    #endif

    eqBands.insert(20, QStringLiteral("20"));
    eqBands.insert(32, QStringLiteral("32"));
    eqBands.insert(64, QStringLiteral("64"));
    eqBands.insert(90, QStringLiteral("90"));
    eqBands.insert(125, QStringLiteral("125"));
    eqBands.insert(160, QStringLiteral("160"));
    eqBands.insert(200, QStringLiteral("200"));

    eqBands.insert(250, QStringLiteral("250"));
    eqBands.insert(375, QStringLiteral("375"));
    eqBands.insert(500, QStringLiteral("500"));
    eqBands.insert(750, QStringLiteral("750"));
    eqBands.insert(1000, QStringLiteral("1k"));
    eqBands.insert(1500, QStringLiteral("1.5k"));

    eqBands.insert(2000, QStringLiteral("2k"));
    eqBands.insert(3000, QStringLiteral("3k"));
    eqBands.insert(4000, QStringLiteral("4k"));
    eqBands.insert(8000, QStringLiteral("8k"));
    eqBands.insert(12000, QStringLiteral("12k"));
    eqBands.insert(16000, QStringLiteral("16k"));

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
    BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, 15000);  // 15 sec
    //    BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, DWORD timeoutMili); // default is no timeout

    //BASS_ChannelSetAttribute(int handle, BASSAttribute attrib, float value))//    BASS_ATTRIB_PAN	The panning/balance position, -1 (full left) to +1 (full right), 0 = centre.

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    connect(notifyTimer, SIGNAL(started()), this, SLOT(started()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(stoped()));

    spectrumTimer = new NotifyTimer(this);
    connect(spectrumTimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(calcSpectrum()));
//    spectrumTimer -> start(Settings::instance() -> getSpectrumFreqRate()); // 25 //40 Hz
}

AudioPlayer::~AudioPlayer() {
    notifyTimer -> stop();
    delete notifyTimer;

    spectrumTimer -> stop();
    delete spectrumTimer;

    BASS_PluginFree(0);
    BASS_Free();
}

int AudioPlayer::getCalcSpectrumBandsCount() {
    if (channelsCount != prevChannelsCount) {
        prevChannelsCount = channelsCount;
        emit channelsCountChanged();
    }

    return _spectrumBandsCount / (channelsCount == 1 ? channelsCount : (channelsCount / 2));
}

void AudioPlayer::setNotifyInterval(signed int milis) {
    notifyInterval = milis;
    if (notifyTimer -> isActive())
        notifyTimer -> setInterval(notifyInterval);
}

void AudioPlayer::setSpectrumBandsCount(int bandsCount) {
    _spectrumBandsCount = bandsCount;
    defaultSpectrum.clear();
    QVector<int> l;
    l.fill(defaultSpectrumLevel, _spectrumBandsCount);

    defaultSpectrum.append(l);
    defaultSpectrum.append(l);

//////////////////  calculate predefined points  ///////////////////////////

    spectrumPoints.clear();
    for (int x = 0, b0 = 0; x < _spectrumBandsCount; x++) { // 1024 nodes precalc
        int b1 = pow(2, x * 10.0 / (_spectrumBandsCount - 1));
        if (b1 > 1023) b1 = 1023;
        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin

        spectrumPoints.append(b1);

        b0 = b1;
    }

    int layerLimit = 1024, gLimit = layerLimit * channelsCount;
    int workSpectrumBandsCount = getCalcSpectrumBandsCount();

    spectrumComplexPoints.clear();
    for (int x = 0, b0 = 0; x < workSpectrumBandsCount; x++) {
        int b1 = pow(2, x * 10.0 / (workSpectrumBandsCount - 1)) * channelsCount;
        if (b1 - channelsCount <= b0) b1 = b0 + channelsCount * 2; // make sure it uses at least 2 FFT bin
        if (b1 > gLimit - 1) b1 = gLimit - 1; // prevent index overflow

        spectrumComplexPoints.append(b1);

        b0 = b1;
    }
}

void AudioPlayer::registerEQ() {
    if (_fxEQ) unregisterEQ();

    _fxEQ = BASS_ChannelSetFX(chan, BASS_FX_BFX_PEAKEQ, 0);

    BASS_BFX_PEAKEQ eq;
    eq.fQ = 0;
    eq.fBandwidth = 3; //2.5
    eq.lChannel = BASS_BFX_CHANALL;

    QMap<int, QString>::Iterator band = eqBands.begin();
    for(int num = 0; band != eqBands.end(); band++, num++) {
        eq.fGain = eqBandsGain.value(num, 0);
        eq.lBand = num; eq.fCenter = band.key(); BASS_FXSetParameters(_fxEQ, &eq);
    }
}

void AudioPlayer::unregisterEQ() {
    if (!_fxEQ) return;
    BASS_ChannelRemoveFX(chan, _fxEQ);
    _fxEQ = 0;
}

void AudioPlayer::setEQBand(int band, float gain) {
    BASS_BFX_PEAKEQ eq;
    eq.lBand = band;
    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    BASS_FXSetParameters(_fxEQ, &eq);
    eqBandsGain.insert(band, gain);
}

////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

int AudioPlayer::openRemoteChannel(QString path) {
    BASS_ChannelStop(chan);

    #ifdef Q_OS_WIN
        chan = BASS_StreamCreateURL(path.toStdWString().data(), 0, BASS_SAMPLE_FLOAT, NULL, 0);
    #else
        chan = BASS_StreamCreateURL(path.toStdString().c_str(), 0, BASS_SAMPLE_FLOAT, NULL, 0);
    #endif

    return chan;
}

int AudioPlayer::openChannel(QString path) {
    BASS_ChannelStop(chan);

    #ifdef Q_OS_WIN
        chan = BASS_StreamCreateFile(false, path.toStdWString().data(), 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE);
    #else
        chan = BASS_StreamCreateFile(false, path.toStdString().c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE);
    #endif

    if (!chan)
//    if (!(stream = BASS_StreamCreateFile(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP))
//        && !(chan = BASS_MusicLoad(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP | BASS_MUSIC_RAMP | BASS_MUSIC_POSRESET | BASS_MUSIC_STOPBACK | BASS_STREAM_PRESCAN | BASS_MUSIC_AUTOFREE, 1)))
        qDebug() << "Can't play file " <<  BASS_ErrorGetCode() << path.toUtf8().data();
    return chan;
}

//int AudioPlayer::openRemoteChannel(QString path) {
//    BASS_ChannelStop(chan);
//    chan = BASS_StreamCreateURL(path.toStdWString().data(), 0, BASS_SAMPLE_FLOAT, NULL, 0);

////    if (!chan) {
////        int status = BASS_ErrorGetCode();
////        if (chan == -1 || status == BASS_ERROR_FILEOPEN)// || status == BASS_ERROR_NONET)
////            emit remoteUnprocessed();
////        qDebug() << "Can't play stream" <<  BASS_ErrorGetCode() << path.toUtf8();
////    }
//    return chan;
//}

//int AudioPlayer::openChannel(QString path) {
//    BASS_ChannelStop(chan);
//    if (!(chan = BASS_StreamCreateFile(false, path.toStdWString().data(), 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE)))
////    if (!(stream = BASS_StreamCreateFile(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP))
////        && !(chan = BASS_MusicLoad(false, path.toStdWString().c_str(), 0, 0, BASS_SAMPLE_LOOP | BASS_MUSIC_RAMP | BASS_MUSIC_POSRESET | BASS_MUSIC_STOPBACK | BASS_STREAM_PRESCAN | BASS_MUSIC_AUTOFREE, 1)))
//        qDebug() << "Can't play file " <<  BASS_ErrorGetCode() << path.toUtf8().data();
//    return chan;
//}

void AudioPlayer::closeChannel() {
//    BASS_ChannelSlideAttribute(chan, BASS_ATTRIB_VOL, 0, 1000);
    unregisterEQ();
    BASS_ChannelStop(chan);
    BASS_ChannelRemoveSync(chan, syncHandle);
    BASS_ChannelRemoveSync(chan, syncDownloadHandle);
    BASS_StreamFree(chan);
}

////////////////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////////////////

void AudioPlayer::started() {
    currentState = PlayingState;
    emit stateChanged(PlayingState);
}

void AudioPlayer::signalUpdate() {
    int curr_pos = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000;
    emit positionChanged(curr_pos);
}

void AudioPlayer::calcSpectrum() { 
    if (spectrumHeight > 0) {
        if (currentState == StoppedState) {
            emit spectrumChanged(defaultSpectrum);
        } else {
            if (Settings::instance() -> spectrumType() == Playo3::bars) {
                QList<QVector<int> > res;
                res.append(getSpectrum());
                emit spectrumChanged(res);
            }
            else emit spectrumChanged(getComplexSpectrum());
        }
    }
}

void AudioPlayer::slidePosForward() {
    if (currentState == PlayingState || currentState == PausedState) {
        int dur = getDuration();
        int pos = getPosition() + dur / 10;
        if (pos < dur)
            setPosition(pos);
    }
}
void AudioPlayer::slidePosBackward() {
    if (currentState == PlayingState || currentState == PausedState) {
        int pos = getPosition() - (getDuration() / 10);
        if (pos < 0) pos = 0;
        setPosition(pos);
    }
}

//0 to 10000
void AudioPlayer::setChannelVolume(int val) {
    volumeVal = val > 0 ? (val / 10000.0) : 0;
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
    emit volumeChanged(val);
}

void AudioPlayer::slideVolForward() {
    int newVolLevel = getVolume() + 1000;
    if (newVolLevel > 10000) newVolLevel = 10000;
    setChannelVolume(newVolLevel);
}
void AudioPlayer::slideVolBackward() {
    int newVolLevel = getVolume() - 1000;
    if (newVolLevel < 0) newVolLevel = 0;
    setChannelVolume(newVolLevel);
}

// 0 to 10000
void AudioPlayer::setVolume(int val) {
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, val);
    emit volumeChanged(val);
}

//from 0 to 1
float AudioPlayer::getRemoteFileDownloadPosition() {
    if (size == -1) {
        prevDownloadPos = 0;
        DWORD len = BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END);
        size = len + BASS_StreamGetFilePosition(chan, BASS_FILEPOS_START);
    }

    if (prevDownloadPos != 1) {
        prevDownloadPos = ((BASS_StreamGetFilePosition(chan, BASS_FILEPOS_DOWNLOAD)) / size);
    }
    return prevDownloadPos;
}

float AudioPlayer::getBpmValue(QUrl uri) {
    int cochan;

    if (uri.isLocalFile())
        cochan = BASS_StreamCreateFile(false, uri.toLocalFile().toStdWString().c_str(), 0, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN | BASS_SAMPLE_MONO);
    else
        cochan = BASS_StreamCreateURL(uri.toString().toStdString().c_str(), 0, BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_SAMPLE_MONO, NULL, 0);

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

float AudioPlayer::fastSqrt(float x) {
  unsigned int i = *(unsigned int*) &x;

  // adjust bias
  i  += 127 << 23;
  // approximation of square root
  i >>= 1;

  return *(float*) &i;
}

QVector<int> AudioPlayer::getSpectrum() {
    float fft[1024];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048);
    QVector<int> res;
    int spectrumMultiplicity = Settings::instance() -> spectrumMultiplier() * spectrumHeight;

    int b0 = 0, x, y;

    for (x = 0; x < _spectrumBandsCount; x++) {
        float peak = 0;
        int b1 = spectrumPoints[x];
        for (; b0 < b1; b0++)
            if (peak < fft[1 + b0])
                peak = fft[1 + b0];

        y = fastSqrt(peak) * spectrumMultiplicity + defaultSpectrumLevel; // 4 // scale it (sqrt to make low values more visible)
        if (y > spectrumHeight) y = spectrumHeight; // cap it

        res.append(y);
    }

    return res;
}

QList<QVector<int> > AudioPlayer::getComplexSpectrum() {
    QList<QVector<int> > res;

    if (channelsCount == 1) {
        res.append(getSpectrum());
        return res;
    }

    int layerLimit = 1024, gLimit = layerLimit * channelsCount;
    int spectrumMultiplicity = Settings::instance() -> spectrumMultiplier() * spectrumHeight;
    int workSpectrumBandsCount = getCalcSpectrumBandsCount();
    float fft[gLimit];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FFT_REMOVEDC);

    QVector<float> peaks;
    int b0 = 0, x, y, z, peakNum;

    for (x = 0; x < channelsCount; x++)
        res.append(QVector<int>());

    for (x = 0; x < workSpectrumBandsCount; x++) {
        peaks.fill(0, channelsCount);

        int b1 = spectrumComplexPoints[x];

        for (; b0 < b1; b0++) {
            peakNum = b0 % channelsCount;
            if (peaks[peakNum] < fft[b0])
                peaks[peakNum] = fft[b0];
        }

        for (z = 0; z < channelsCount; z++) {
            y = fastSqrt(peaks[z]) * spectrumMultiplicity + defaultSpectrumLevel; // 4 // scale it (sqrt to make low values more visible)
            if (y > spectrumHeight) y = spectrumHeight; // cap it

            res[z].append(y);
        }
    }

    return res;
}

////////////////////////////////////////////////////////////////////////

void AudioPlayer::registerEQ(bool registrate) {
    useEQ = registrate;
    if (registrate) registerEQ();
    else unregisterEQ();
}

void AudioPlayer::play() {
    if (currentState == PausedState) {
        resume();
    } else {
        emit mediaStatusChanged(LoadingMedia);
        closeChannel();
        if (mediaUri.isEmpty()) {
            emit mediaStatusChanged(NoMedia);
        } else {
            startProccessing();
            qDebug() << mediaUri.toString();

            if (mediaUri.isLocalFile()) {
                openChannel(mediaUri.toLocalFile());
                size = 0;
                prevDownloadPos = 1;
            } else {
                openRemoteChannel(mediaUri.toString());
                size = -1;
            }

            if (chan) {
                BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
                initDuration();

                BASS_CHANNELINFO info;
                if (BASS_ChannelGetInfo(chan, &info))
                    channelsCount = info.chans;
                else
                    channelsCount = 2;

                if (useEQ) registerEQ();

                emit mediaStatusChanged(LoadedMedia);

                BASS_ChannelPlay(chan, true);
                spectrumTimer -> start(Settings::instance() -> spectrumFreqRate()); // 25 //40 Hz
                notifyTimer -> start(notifyInterval);

                syncHandle = BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &endTrackSync, this);
                syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

                setStartPosition();
            } else {
                currentState = UnknowState;
                switch(BASS_ErrorGetCode()) {
                    case BASS_ERROR_FILEFORM: {
                        emit mediaStatusChanged(InvalidMedia);
                    break;}
                    case BASS_ERROR_FILEOPEN: {
                        emit mediaStatusChanged(NoMedia);
                    break;}
                    default: emit mediaStatusChanged(StalledMedia);
                }
            }
        }
    }
}

void AudioPlayer::pause() {
    notifyTimer -> stop();
    spectrumTimer -> stop();
    BASS_ChannelPause(chan);
    emit stateChanged(PausedState);
    currentState = PausedState;
}

void AudioPlayer::resume() {
    if (currentState == InitState)
        play();
    else {
        if (!BASS_ChannelPlay(chan, false)) {
            emit mediaStatusChanged(StalledMedia);
            qDebug() << "Error resuming";
        } else {
            notifyTimer -> start(notifyInterval);
            spectrumTimer -> start(Settings::instance() -> spectrumFreqRate()); // 25 //40 Hz
        }
    }
}

void AudioPlayer::stop() {
    closeChannel();
    channelsCount = 2;
    spectrumTimer -> stop();
    notifyTimer -> stop();
    emit stateChanged(StoppedState);
}

void AudioPlayer::endOfPlayback() {
    setPosition(0);
    pause();
    emit mediaStatusChanged(EndOfMedia);
}
