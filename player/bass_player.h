#ifndef PLAYER
#define PLAYER

#include <qdir.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

#include "modules/core/media/interfaces/iplayer.h"

void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackDownloading(HSYNC, DWORD, DWORD, void * user);

#ifdef Q_OS_WIN
    #define QSTRING_TO_STR(str) str.toStdWString().data()
#else
    #define QSTRING_TO_STR(str) str.toStdString().c_str()
#endif

#define SLIDE_VOLUME_OFFSET 1000
#define VOLUME_MULTIPLIER 10000.0
#define POSITION_MULTIPLIER 1000.0
#define PAN_MULTIPLIER 1000.0

#define LOCAL_PLAY_ATTRS BASS_SAMPLE_FLOAT | BASS_ASYNCFILE
#define REMOTE_PLAY_ATTRS BASS_SAMPLE_FLOAT

#define LOCAL_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN | BASS_SAMPLE_MONO
#define REMOTE_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_SAMPLE_MONO

class BassPlayer : public IPlayer {
    Q_OBJECT

    HFX _fxEQ;
    HSYNC syncHandle, syncDownloadHandle;
    bool is_paused;
    int startPos;
    unsigned long chan;
    QFutureWatcher<int> * openChannelWatcher;

    inline int default_device() {
        #ifdef Q_OS_WIN
            return -1;
        #else
            return BASS_DEVICE_DEFAULT;
        #endif
    }

    void proceedErrorState();
    int openChannel(const QUrl & url, QFutureWatcher<int> * watcher);

    void playPreproccessing();
protected slots:
    void afterSourceOpening();

protected:
    inline unsigned long open(const QString & path, DWORD flags) {
        return BASS_StreamCreateFile(false, QSTRING_TO_STR(path), 0, 0, flags);
    }
    inline unsigned long openRemote(const QString & path, DWORD flags) {
        return BASS_StreamCreateURL(QSTRING_TO_STR(path), 0, flags, NULL, 0);
    }

    bool playProcessing(int startMili, bool paused = false);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    int recalcCurrentPosProcessing();
    bool newPosProcessing(int newPos);
    bool newVolumeProcessing(int newVol);
    bool newPanProcessing(int newPan);
    float prebufferingLevelCalc();
    int calcFileSize();

    inline int maxVolume() const { return 10000; }

    bool registerEQ();
    bool unregisterEQ();
    bool processEqSetGain(int band, float gain);

    bool calcSpectrum(QVector<int> & result);
    bool calcSpectrum(QList<QVector<int> > & result);
public:
    explicit BassPlayer(QWidget * parent, float open_time_out_sec = 10);
    ~BassPlayer();

    int position() const;

    bool fileInfo(const QUrl & uri, IMediaInfo * info);
    float bpmCalc(const QUrl & uri);

    inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
    inline void openTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
    inline void proxy(const QString & proxyStr = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxyStr.isEmpty() ? NULL : QSTRING_TO_STR(proxyStr)); }
//    inline void readTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
//    inline void userAgent(const QString & userAgent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, userAgent.isEmpty() ? NULL : QSTRING_TO_STR(userAgent)); }
};

#endif // PLAYER
