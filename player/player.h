#ifndef PLAYER
#define PLAYER

#include <qdir.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

//#include "math.h"
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

class BassPlayer : public IPlayer {
    unsigned long startPos;
    unsigned long chan;
    QFutureWatcher<int> * openChannelWatcher;

    inline int default_device() {
        #ifdef Q_OS_WIN
            return -1;
        #else
            return BASS_DEVICE_DEFAULT;
        #endif
    }
    int openChannel(const QUrl & url, QFutureWatcher<int> * watcher);

    void afterSourceOpening();
    void playPreproccessing();
protected:
    bool playProcessing(uint startMili);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    uint recalcCurrentPosProcessing();
    bool newPosProcessing(uint newPos);
    bool newVolumeProcessing(uint newVol);
    bool newPanProcessing(int newPan);

    inline uint maxVolume() const { return 10000; }

    bool registerEQ();
    bool unregisterEQ();

    void calcSpectrum(QVector<int> & result);
    void calcSpectrum(QList<QVector<int> > & result);
public:
    explicit BassPlayer(QWidget * parent, uint open_time_out_sec = 10);
    ~BassPlayer();

    uint position() const;
    uint volume() const;
    int pan() const;

    inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
    inline void openTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
    inline void proxy(const QString & proxyStr = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxyStr.isEmpty() ? NULL : QSTRING_TO_STR(proxyStr)); }
//    inline void readTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
//    inline void userAgent(const QString & userAgent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, userAgent.isEmpty() ? NULL : QSTRING_TO_STR(userAgent)); }
};

#endif // PLAYER
