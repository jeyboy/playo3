#include "audio_player.h"
#include <qdebug.h>

//Get the percentage downloaded of an internet file stream, or the buffer level when streaming in blocks.
//QWORD len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END); // file/buffer length
//QWORD buf=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_BUFFER); // buffer level
//float progress=buf*100.0/len; // percentage of buffer filled

void AudioPlayer::init() {
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
    BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, 15000);  // 15 sec
    //    BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, DWORD timeoutMili); // default is no timeout

    // TODO: add proxy realization
    //if (MESS(41,BM_GETCHECK,0,0))
    //    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,NULL); // disable proxy
    //else
    //    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,proxy); // enable proxy

    //BASS_ChannelSetAttribute(int handle, BASSAttribute attrib, float value))//    BASS_ATTRIB_PAN	The panning/balance position, -1 (full left) to +1 (full right), 0 = centre.
}

AudioPlayer::AudioPlayer(QObject * parent) : QObject(parent), AudioPlayerEqualizer(this), AudioPlayerSpectrum(this), notifyInterval(100) {
    init();

    // cheat for cross treadhing
    connect(this, SIGNAL(playbackEnded()), this, SLOT(endOfPlayback()));

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));

    spectrumTimer = new NotifyTimer(this);
    connect(spectrumTimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(calcSpectrum()));
}

AudioPlayer::~AudioPlayer() {
    stopTimers();
    delete notifyTimer;
    delete spectrumTimer;

    BASS_PluginFree(0);
    BASS_Free();
}

void AudioPlayer::setMedia(QUrl mediaPath, uint start_pos = 0, int media_duration = -1) {
    mediaUri = mediaPath;
    currentState = InitState;
    startPos = start_pos;
    if ((duration = media_duration) > 0) {
        initDuration();
        setStartPosition();
    }
}

void AudioPlayer::setNotifyInterval(signed int milis) {
    notifyInterval = milis;
    if (notifyTimer -> isActive())
        notifyTimer -> setInterval(notifyInterval);
}

////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

int AudioPlayer::openRemoteChannel(QString path) {
    BASS_ChannelStop(chan);

    size = -1;

//    "http://www.asite.com/afile.mp3\r\nCookie: mycookie=blah\r\n"
    chan = BASS_StreamCreateURL(
            #ifdef Q_OS_WIN
                path.toStdWString().data()
            #else
                path.toStdString().c_str()
            #endif
        , 0, BASS_SAMPLE_FLOAT, NULL, 0);
    return chan;
}

int AudioPlayer::openChannel(QString path) {
    BASS_ChannelStop(chan);

    size = 0;
    prevDownloadPos = 1;

    chan = BASS_StreamCreateFile(false,
            #ifdef Q_OS_WIN
                 path.toStdWString().data()
            #else
                path.toStdString().c_str()
            #endif
        , 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE);

    if (!chan)
        qDebug() << "Can't play file " <<  BASS_ErrorGetCode() << path.toUtf8().data();
    return chan;
}

void AudioPlayer::closeChannel() {
//    BASS_ChannelSlideAttribute(chan, BASS_ATTRIB_VOL, 0, 1000);
    unregisterEQ();
    BASS_ChannelStop(chan);
    BASS_ChannelRemoveSync(chan, syncHandle);
    BASS_ChannelRemoveSync(chan, syncDownloadHandle);
    BASS_StreamFree(chan);
    channelsCount = 2;
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

            if (mediaUri.isLocalFile())
                openChannel(mediaUri.toLocalFile());
            else
                openRemoteChannel(mediaUri.toString());

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
                startTimers();

                syncHandle = BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &endTrackSync, this);
                syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

                setStartPosition();
            }
            else proceedErrorState();
        }
    }
}

void AudioPlayer::pause() {
    stopTimers(true);
    BASS_ChannelPause(chan);
}

void AudioPlayer::resume() {
    if (currentState == InitState)
        play();
    else {
        if (!BASS_ChannelPlay(chan, false)) {
            emit mediaStatusChanged(StalledMedia);
            qDebug() << "Error resuming";
        }
        else startTimers();
    }
}

void AudioPlayer::stop() {
    closeChannel();
    stopTimers();
}

void AudioPlayer::endOfPlayback() {
    setPosition(0);
    pause();
    emit mediaStatusChanged(EndOfMedia);
}
