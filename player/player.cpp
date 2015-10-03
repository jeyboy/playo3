#include "player.h"

void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    BassPlayer * player = static_cast<BassPlayer *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    BassPlayer * player = static_cast<BassPlayer *>(user);
    player -> finishRemoteFileDownloading();
}

void BassPlayer::proceedErrorState() {
    updateState(UnknowState);
    switch(BASS_ErrorGetCode()) {
        case BASS_ERROR_FILEFORM: { emit statusChanged(InvalidMedia); break; }
        case BASS_ERROR_FILEOPEN: { emit statusChanged(NoMedia); break; }
        // BASS_ERROR_TIMEOUT
        default: emit mediaStatusChanged(StalledMedia);
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
        qDebug() << "OPEN ERROR" << BASS_ErrorGetCode();

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
    } else emit mediaStatusChanged(LoadedMedia);

    watcher -> deleteLater();
    if (watcher == openChannelWatcher)
        openChannelWatcher = 0;
}

void BassPlayer::playPreproccessing() {
    emit mediaStatusChanged(LoadedMedia);
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volumeVal);
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_PAN, panVal);

    initDuration();

    BASS_CHANNELINFO info;
    if (BASS_ChannelGetInfo(chan, &info))
        channelsCount = info.chans;
    else
        channelsCount = 2;

    if (useEQ) registerEQ();

    startTimers();
    if (BASS_ChannelPlay(chan, true))
        emit mediaStatusChanged(StartPlayingMedia);

    syncHandle = BASS_ChannelSetSync((HSYNC)chan, BASS_SYNC_END, 0, &endTrackSync, this);
    syncDownloadHandle = BASS_ChannelSetSync(chan, BASS_SYNC_DOWNLOAD, 0, &endTrackDownloading, this);

    setStartPosition();
}



bool BassPlayer::playProcessing(uint startMili) {
    startPos = startMili;

    stop();

    if (media_url.isEmpty())
        emit statusChanged(NoMedia);
    else {
        emit statusChanged(LoadingMedia);

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
        emit mediaStatusChanged(StalledMedia);
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

uint BassPlayer::recalcCurrentPosProcessing() {}
bool BassPlayer::newPosProcessing(uint newPos) {}
bool BassPlayer::newVolumeProcessing(uint newVol) {}
bool BassPlayer::newPanProcessing(int newPan) {}


bool BassPlayer::registerEQ() {}
bool BassPlayer::unregisterEQ() {}

void BassPlayer::calcSpectrum(QVector<int> & result) {}
void BassPlayer::calcSpectrum(QList<QVector<int> > & result) {}

BassPlayer::BassPlayer(QWidget * parent, uint open_time_out_sec) : IPlayer(parent) {
    #include <qapplication.h>

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

uint BassPlayer::position() const {}
uint BassPlayer::volume() const {}
int BassPlayer::pan() const {}
