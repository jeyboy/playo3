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

#define BASS_VOLUME_MULTIPLIER VOLUME_MULTIPLIER
#define BASS_POSITION_MULTIPLIER POSITION_MULTIPLIER
#define BASS_PAN_MULTIPLIER PAN_MULTIPLIER

#define LOCAL_PLAY_ATTRS BASS_SAMPLE_FLOAT | BASS_ASYNCFILE
#define REMOTE_PLAY_ATTRS BASS_SAMPLE_FLOAT

#define LOCAL_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN | BASS_SAMPLE_MONO
#define REMOTE_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_SAMPLE_MONO

class BassPlayer : public IPlayer {
    Q_OBJECT

    HFX _fxEQ;
    HSYNC syncHandle, syncDownloadHandle;
    bool is_paused;
    unsigned long chan;
    QFutureWatcher<int> * openChannelWatcher;
//    QList<int> openedDevices;

    int default_device();

    bool proceedErrorState();
    int openChannel(const QUrl & url, QFutureWatcher<int> * watcher);

    void playPreproccessing();
protected slots:
    void afterSourceOpening();

protected:
    bool initDevice(int newDevice, int frequency = 44100);
    bool closeDevice(int device);
    void loadPlugins();

    inline unsigned long open(const QString & path, DWORD flags) {
        return BASS_StreamCreateFile(false, QSTRING_TO_STR(path), 0, 0, flags);
    }
    inline unsigned long openRemote(const QString & path, DWORD flags) {
        return BASS_StreamCreateURL(QSTRING_TO_STR(path), 0, flags, NULL, 0);
    }

    bool playProcessing(bool paused = false);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    bool newPosProcessing(qint64 newPos);
    bool newVolumeProcessing(int newVol);
    bool newPanProcessing(int newPan);
    float prebufferingLevelCalc();
    qint64 calcFileSize();

    bool registerEQ();
    bool unregisterEQ();
    bool processEqSetGain(int band, float gain);

    bool calcSpectrum(QVector<float> & result);
    bool calcSpectrum(QList<QVector<float> > & result);
public:
    explicit BassPlayer(QWidget * parent);
    ~BassPlayer();

    QHash<QString, QVariant> deviceList();
    QVariant currDevice();
    bool setDevice(const QVariant & device);

    qint64 position() const;

    bool fileInfo(const QUrl & uri, IMediaInfo * info);
    float bpmCalc(const QUrl & uri);

    inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
    inline void openTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
    inline void proxy(const QString & proxyStr = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxyStr.isEmpty() ? NULL : QSTRING_TO_STR(proxyStr)); }
//    inline void readTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
//    inline void userAgent(const QString & userAgent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, userAgent.isEmpty() ? NULL : QSTRING_TO_STR(userAgent)); }
};

#endif // PLAYER
