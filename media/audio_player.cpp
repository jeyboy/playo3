#include "audio_player.h"
#include <qdebug.h>

//void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user)
void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    AudioPlayer::Base * player = static_cast<AudioPlayer::Base *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    AudioPlayer::Base * player = static_cast<AudioPlayer::Base *>(user);
    player -> finishRemoteFileDownloading();
    emit player -> downloadEnded();
}

using namespace AudioPlayer;

//Get the percentage downloaded of an internet file stream, or the buffer level when streaming in blocks.
//QWORD len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END); // file/buffer length
//QWORD buf=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_BUFFER); // buffer level
//float progress=buf*100.0/len; // percentage of buffer filled

void Base::init() {
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

Base::Base(QObject * parent) : Panel(parent) {
    init();

    // cheat for cross treadhing
    connect(this, SIGNAL(playbackEnded()), this, SLOT(endOfPlayback()));

    notifyTimer = new NotifyTimer(this);
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(signalUpdate()));

    spectrumTimer = new NotifyTimer(this);
    connect(spectrumTimer, SIGNAL(timeout()), this, SLOT(calcSpectrum()));
    connect(notifyTimer, SIGNAL(stoped()), this, SLOT(calcSpectrum()));
}

Base::~Base() {
    stopTimers();
    BASS_PluginFree(0);
    BASS_Free();
}

void Base::setMedia(const QUrl & mediaPath, uint start_pos, int media_duration) {
    mediaUri = mediaPath;
    currentState = InitState;
    bool is_local = mediaPath.isLocalFile();
    startPos = is_local ? start_pos : 0;
    if ((duration = media_duration) > 0) {
        initDuration();

        if (is_local) setStartPosition();
    }
}

////////////////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////////////////

int Base::openChannel(const QUrl & url) {
    if (url.isLocalFile()) {
        size = 0;
        prevDownloadPos = 1;
        chan = open(url.toLocalFile(), LOCAL_PLAY_ATTRS);
    } else {
        size = -1;
        //    "http://www.asite.com/afile.mp3\r\nCookie: mycookie=blah\r\n"
        chan = openRemote(url.toString(), REMOTE_PLAY_ATTRS);
    }

    return chan;
}

void Base::closeChannel() {
//    BASS_ChannelSlideAttribute(chan, BASS_ATTRIB_VOL, 0, 1000);
    unregisterEQ();
    BASS_ChannelStop(chan);
    BASS_ChannelRemoveSync(chan, syncHandle);
    BASS_ChannelRemoveSync(chan, syncDownloadHandle);
    BASS_StreamFree(chan);
    emit stateChanged(currentState = StoppedState);
    calcSpectrum();
}

void Base::play() {
    if (currentState == PausedState)
        resume();
    else {
        closeChannel();
        emit mediaStatusChanged(LoadingMedia);
        if (mediaUri.isEmpty())
            emit mediaStatusChanged(NoMedia);
        else {
            startProccessing();
            openChannel(mediaUri);

            if (chan) aroundProccessing();
            else proceedErrorState();
        }
    }
}

void Base::pause() {
    stopTimers(true);
    BASS_ChannelPause(chan);
}

void Base::resume() {
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

void Base::stop() {
    closeChannel();
    stopTimers();
}

void Base::endOfPlayback() {
    setPosition(0);
    pause();
    emit mediaStatusChanged(EndOfMedia);
}

void Base::startTimers() {
    notifyTimer -> start(notifyInterval);
    spectrumTimer -> start(Settings::instance() -> spectrumFreqRate()); // 25 //40 Hz
    emit stateChanged(currentState = PlayingState);
}

void Base::stopTimers(bool paused) {
    notifyTimer -> stop();
    spectrumTimer -> stop();

    if (paused)
        emit stateChanged(currentState = PausedState);
    else
        emit stateChanged(currentState = StoppedState);
}

void Base::aroundProccessing() {
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
    initDuration();

    BASS_CHANNELINFO info;
    if (BASS_ChannelGetInfo(chan, &info))
        channelsCount = info.chans;
    else
        channelsCount = 2;

    if (useEQ) registerEQ();

    emit mediaStatusChanged(LoadedMedia);

    startTimers();
    BASS_ChannelPlay(chan, true);

    syncHandle = BASS_ChannelSetSync((HSYNC)chan, BASS_SYNC_END, 0, &endTrackSync, this);
    syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

    setStartPosition();
}
