#include "bass_player.h"
#include "settings.h"

#include <qapplication.h>

void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    BassPlayer * player = static_cast<BassPlayer *>(user);
    emit player -> playbackEnding();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    BassPlayer * player = static_cast<BassPlayer *>(user);
    player -> setDownloadingLevel();
}

bool BassPlayer::proceedErrorState(int code) {
    updateState(UnknowState);

    if (code == BASS_OK)
        code = BASS_ErrorGetCode();

    qCritical() << "proceedErrorState" << media_url.toString() << code;

    switch(code) {
        case BASS_OK: {
            if (chan)
                return false;
            else { // hotfix for some stalled media
                emit statusChanged(media_title, StalledMedia);
            }
        break;}
        case BASS_ERROR_FILEFORM: { emit statusChanged(media_title, InvalidMedia); break; }
        case BASS_ERROR_FILEOPEN: { emit statusChanged(media_title, NoMedia); break; }
        // BASS_ERROR_TIMEOUT
        default:
            qDebug() << "UNKNOWN ERROR: " << BASS_ErrorGetCode();
            emit statusChanged(media_title, StalledMedia);
    }

    return true;
}

OpenCallbackData BassPlayer::openChannel(const QUrl & url, OpenCallbackData & channel_params) {
    if (url.isLocalFile()) {
        channel_params.channel_handle = open(url.toLocalFile(), LOCAL_PLAY_ATTRS);
        setDownloadingLevel(1);
    } else {
        //    "http://www.asite.com/afile.mp3\r\nCookie: mycookie=blah\r\n"
        channel_params.channel_handle = openRemote(
            url.toString()
                .replace(QStringLiteral("%0D%0A"), QStringLiteral("\r\n")) %
                    QStringLiteral("\r\n") % USER_AGENT_HEADER_NAME %
                    QStringLiteral(": ") % DEFAULT_AGENT % QStringLiteral("\r\n"),
            REMOTE_PLAY_ATTRS
        );
    }

    channel_params.error = BASS_ErrorGetCode();

//    if (!channel_params.second)
//        proceedErrorState();

    return channel_params;
}
void BassPlayer::closeChannel() {
    if (chan) {
        unregisterEQ();
        BASS_ChannelRemoveSync(chan, syncHandle);
        BASS_ChannelRemoveSync(chan, syncDownloadHandle);
        BASS_ChannelStop(chan);
        BASS_StreamFree(chan);
    }
}

void BassPlayer::afterSourceOpening() {
    QFutureWatcher<OpenCallbackData> * watcher = (QFutureWatcher<OpenCallbackData> *)sender();
    OpenCallbackData result = watcher -> result();

    if (proc_channel.uid != result.uid)
        BASS_StreamFree(result.channel_handle);
    else {
        closeChannel(); //INFO: close prev channel

        chan = result.channel_handle;

        if (!chan)
            proceedErrorState(result.error);
        else {
            emit statusChanged(media_title, LoadedMedia);
            if (chan) playPreproccessing();
        }
    }

    watcher -> deleteLater();
    if (watcher == openChannelWatcher)
        openChannelWatcher = 0;
}

void BassPlayer::playPreproccessing() {
    emit statusChanged(media_title, LoadedMedia);
    newVolumeProcessing(volume());
    newPanProcessing(pan());

    #ifdef BASS_USE_TEMPO
        applyTempoToChannel();
        newTempoProcessing(tempo());
    #endif

    setSampleRateQuality();

    BASS_CHANNELINFO info;
    if (BASS_ChannelGetInfo(chan, &info))
        channelsCount(info.chans);
    else
        channelsCount(2);

    if (eq_in_use) registerEQ();

    if (BASS_ChannelPlay(chan, true)) { // stalled with big sized video files
        playPostprocessing(is_paused);

        syncHandle = BASS_ChannelSetSync((HSYNC)chan, BASS_SYNC_END, 0, &endTrackSync, this);
        syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

//        BASS_SYNC_STALL
//        mixtime only	Sync when playback of the channel is stalled/resumed.
//        param : not used. data : 0 = stalled, 1 = resumed.
    } else {
        proceedErrorState();
        qCritical() << "IS NOT PLAYED";
    }
}

bool BassPlayer::playProcessing(const bool & paused) {
    is_paused = paused;

    if (!media_url.isEmpty()) {
        proc_channel = OpenCallbackData {
            QString::number(QDateTime::currentMSecsSinceEpoch()) % media_title,
            0,
            0
        };

        openChannelWatcher = new QFutureWatcher<OpenCallbackData>();
        connect(openChannelWatcher, SIGNAL(finished()), this, SLOT(afterSourceOpening()));
        openChannelWatcher -> setFuture(QtConcurrent::run(this, &BassPlayer::openChannel, media_url, proc_channel));
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
bool BassPlayer::pauseProcessing() {
    return BASS_ChannelPause(chan);
}
bool BassPlayer::stopProcessing() {
    if (openChannelWatcher) {
        while(openChannelWatcher && !openChannelWatcher -> isFinished())
            QApplication::processEvents();
//        openChannelWatcher -> waitForFinished();
    }

    closeChannel();

    return true;
}

qint64 BassPlayer::position() const {
    return BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * BASS_POSITION_MULTIPLIER - startPosition();
}
bool BassPlayer::newPosProcessing(const qint64 & new_pos) {
    return BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, (new_pos + startPosition()) / BASS_POSITION_MULTIPLIER), BASS_POS_BYTE);
}
bool BassPlayer::newVolumeProcessing(const int & new_vol) {
    float volume_val = new_vol > 0 ? (new_vol / (float)volumeMax()) : 0;
    return BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volume_val);
}
bool BassPlayer::newPanProcessing(const int & new_pan) {
    float pan_val = new_pan == 0 ? 0 : (new_pan / (float)panMax());
    return BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, pan_val);
}


void BassPlayer::applyTempoToChannel() {
    quint64 tempo_chan = BASS_FX_TempoCreate(chan, BASS_FX_FREESOURCE);
    if (!tempo_chan) {
        //FIXME: block tempo func
        qCritical() << "e:" << BASS_ErrorGetCode();
    } else
        chan = tempo_chan;
}
bool BassPlayer::newTempoProcessing(const int & new_tempo) {
    if (!chan) return false;

    float tempo = new_tempo;
    if (tempo != 0) // -95%...0...+5000%
        tempo = (new_tempo / (float)tempoMax()) * (tempo < 0 ? 50 : 200); // take only 200 %

    bool res = BASS_ChannelSetAttribute(chan, BASS_ATTRIB_TEMPO, tempo);
    if (!res)
        qCritical() << "err:" << BASS_ErrorGetCode();

    return res;
}

float BassPlayer::downloadingLevelCalc() {
    if (fileSize() > 0)
        return ((BASS_StreamGetFilePosition(chan, BASS_FILEPOS_DOWNLOAD)) / (float)fileSize());
    else return 1;
}

qint64 BassPlayer::calcFileSize() {
    qint64 len = BASS_StreamGetFilePosition(chan, BASS_FILEPOS_END);
    return len + BASS_StreamGetFilePosition(chan, BASS_FILEPOS_START);
}


bool BassPlayer::registerEQ() {
    if (_fxEQ) unregisterEQ();

    if ((_fxEQ = BASS_ChannelSetFX(chan, BASS_FX_BFX_PEAKEQ, 0))) {
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
bool BassPlayer::processEqSetGain(const int & band, const float & gain) {
    BASS_BFX_PEAKEQ eq;
    eq.lBand = band;
    BASS_FXGetParameters(_fxEQ, &eq);
    eq.fGain = gain;
    return BASS_FXSetParameters(_fxEQ, &eq);
}

bool BassPlayer::calcSpectrum(QVector<float> & result) {
    int b0 = 0;
    float fft[1024];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048);

    for (int x = 0; x < sbands_count; x++) {
        float peak = 0;
        int b1 = spectrumPoints[x];
        for (; b0 < b1; b0++)
            if (peak < fft[1 + b0])
                peak = fft[1 + b0];

        result.append(sqrt(peak));
    }

    return true;
}
bool BassPlayer::calcSpectrum(QList<QVector<float> > & result) {
    float fft[1024 * channels_count];
    BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FFT_REMOVEDC);

    QVector<float> peaks;
    int b0 = 0, x, z, peakNum;

    for (x = 0; x < channels_count; x++)
        result.append(QVector<float>());

    for (x = 0; x < calcSpectrumBandsGroupCount(); x++) {
        peaks.fill(0, channels_count);

        int b1 = spectrumComplexPoints[x];

        for (; b0 < b1; b0++) {
            peakNum = b0 % channels_count;
            if (peaks[peakNum] < fft[b0])
                peaks[peakNum] = fft[b0];
        }

        for (z = 0; z < channels_count; z++)
            result[z].append(sqrt(peaks[z]));
    }

    return true;
}

bool BassPlayer::fileInfo(const QUrl & uri, IMediaInfo * info) {
    bool currently_played = media_url.isValid() && uri == media_url;

    int chUID;

    if (currently_played)
        chUID = chan;
    else {
        if (uri.isLocalFile())
            chUID = open(uri.toLocalFile(), LOCAL_PLAY_ATTRS);
        else
            chUID = openRemote(uri.toString(), REMOTE_PLAY_ATTRS);

        if (!chUID) return false;
    }

    float time = BASS_ChannelBytes2Seconds(chUID, BASS_ChannelGetLength(chUID, BASS_POS_BYTE)); // playback duration
    qint64 len = BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_END); // file length

    info -> setDuration(time);
    info -> setBitrate((len / (125 * time) + 0.5));

    BASS_CHANNELINFO media_info;
    if (BASS_ChannelGetInfo(chUID, &media_info)) {
        info -> setSize(len + BASS_StreamGetFilePosition(chUID, BASS_FILEPOS_START));
        info -> setSampleRate(media_info.freq);
        info -> setChannels(media_info.chans);
    }

    if (!currently_played)
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

void BassPlayer::loadPlugins() {
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
}

BassPlayer::BassPlayer(QWidget * parent) : IPlayer(parent), chan(0), default_device_index(DEFAULT_BASS_DEVICE), openChannelWatcher(0) {
    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        throw "An incorrect version of BASS.DLL was loaded";

    if (HIWORD(BASS_FX_GetVersion()) != BASSVERSION)
        throw "An incorrect version of BASS_FX.DLL was loaded";

//    BASS_SetVolume(1);
    initOutputDevice(identifyOutputDevice());
    loadPlugins();
    setUserAgent(DEFAULT_AGENT);
}
BassPlayer::~BassPlayer() {
    if (openChannelWatcher)
        openChannelWatcher -> cancel();
    openChannelWatcher -> deleteLater();

    stop();
    BASS_Stop();
    BASS_PluginFree(0);
    if (!closeAllDevices())
        qDebug() << "Some device is not closed";
}

QHash<QString, QVariant> BassPlayer::outputDeviceList() {
    QHash<QString, QVariant> res;

    res.insert(defaultDeviceName(), DEFAULT_BASS_DEVICE);

    BASS_DEVICEINFO info;
    for (int a = 1; BASS_GetDeviceInfo(a, &info); a++) // 0 - no sound
        if (info.flags & BASS_DEVICE_ENABLED) {
            res.insert(QString(info.name), a);

            if (info.flags & BASS_DEVICE_DEFAULT)
                default_device_index = a;
        }

    #ifdef Q_OS_MAC
        for (int a = 0; BASS_GetDeviceInfo(a | BASS_DEVICES_AIRPLAY, &info); a++)
            res.insert(QString(info.name), a | BASS_DEVICES_AIRPLAY);
    #endif

    return res;
}
int BassPlayer::identifyOutputDevice() {
    QString device_name = Settings::obj().outputDevice();

    return
        device_name == defaultDeviceName() ?
            DEFAULT_BASS_DEVICE
              :
            outputDeviceList().value(device_name, DEFAULT_BASS_DEVICE).toInt();
}
QVariant BassPlayer::currOutputDevice() { return QVariant::fromValue(BASS_GetDevice()); }
bool BassPlayer::setOutputDevice(const QString & device_name) {
    int curr_device = BASS_GetDevice();

    auto new_device_functor = [this](const QString & device_name) {
        if (device_name == defaultDeviceName())
            return default_device_index;
        else
            return outputDeviceList().value(device_name).toInt();
    };

    int new_device = new_device_functor(device_name);

    if (curr_device == new_device)
        return true;

    bool res = false;

    if ((res = initOutputDevice(new_device))) {
        //INFO: we cant close here old device // we must close it later
        res = BASS_SetDevice(new_device);
        res &= BASS_ChannelSetDevice(chan, new_device);
    }

    return res;
}
bool BassPlayer::initOutputDevice(const int & new_device, const int & frequency) {
    bool res = BASS_Init(new_device, frequency, 0, NULL, NULL);

    if (!res) {
        if (BASS_ErrorGetCode() == BASS_ERROR_ALREADY) {
            res = true;
        } else {
            qDebug() << "Init error: " << BASS_ErrorGetCode();
            // throw "Cannot initialize device";
        }
    }

    if (res) {
        opened_devices.insert(new_device, true);

        BASS_SetConfig(BASS_CONFIG_FLOATDSP, TRUE);
    //    BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 15); // 15 percents prebuf

        setOpenTimeOut(Settings::obj().openTimeOut());
    }

    return res;
}

bool BassPlayer::closeAllDevices() {
    bool ret = true;

    for(QHash<int, bool>::Iterator it = opened_devices.begin(); it != opened_devices.end(); it++) {
        ret &= BASS_SetDevice(it.key()) && BASS_Free();
    }

    opened_devices.clear();

    return ret;
}
