#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <qapplication.h>
#include <qurl.h>
#include <qdir.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

#include "math.h"

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

//#include "misc/settings.h"

#include "audio_player/audio_player_panel.h"

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

namespace AudioPlayer {
    class Base : public Panel {
        Q_OBJECT

        int openChannel(const QUrl & url, QFutureWatcher<int> * watcher);
        void closeChannel();
    public:
        explicit Base(QObject * parent = 0);
        virtual ~Base();

        void setMedia(const QUrl & mediaPath, uint start_pos = 0, int media_duration = -1);
        inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
        inline void setOpenTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(500, (int)(secLimit * 1000))); }
        inline void setReadTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(500, (int)(secLimit * 1000))); }
        inline void setUserAgent(const QString & userAgent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, userAgent.isEmpty() ? NULL : QSTRING_TO_STR(userAgent)); }
        inline void setProxy(const QString & proxyStr = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxyStr.isEmpty() ? NULL : QSTRING_TO_STR(proxyStr)); }
    signals:
        void playbackEnded();
        void downloadEnded();

    public slots:
        void play();
        void pause();
        void resume();
        void stop();
        void endOfPlayback();
    private slots:
        void postProccessing();

    protected:
        inline unsigned long chId() const { return chan; }

        inline int default_device() {
            #ifdef Q_OS_WIN
                return -1;
            #else
                return BASS_DEVICE_DEFAULT;
            #endif
        }

        void init();

        void startTimers();
        void stopTimers(bool paused = false);

        virtual inline void startProccessing() {}
        void aroundProccessing();
    private:
        unsigned long chan;
        QFutureWatcher<int> * openChannelWatcher;
    };
}

#endif // AUDIO_PLAYER_H
