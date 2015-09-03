#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <qapplication.h>
#include <qurl.h>
#include <qdir.h>

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

        int openChannel(const QUrl & url);
        void closeChannel();
    public:
        explicit Base(QObject * parent = 0);
        virtual ~Base();

        void setMedia(const QUrl & mediaPath, uint start_pos = 0, int media_duration = -1);

    signals:
        void playbackEnded();
        void downloadEnded();

    public slots:
        void play();
        void pause();
        void resume();
        void stop();
        void endOfPlayback();

    protected:
        inline unsigned long chId() const { return chan; }

        inline int default_device() {
            #ifdef Q_OS_WIN
                return -1;
            #else
                return BASS_DEVICE_DEFAULT;
            #endif
        }

        inline unsigned long open(const QString & path) {
            return BASS_StreamCreateFile(false,
                    #ifdef Q_OS_WIN
                        path.toStdWString().data()
                    #else
                        path.toStdString().c_str()
                    #endif
                , 0, 0, BASS_SAMPLE_FLOAT | BASS_ASYNCFILE);
        }
        inline unsigned long openRemote(const QString & path) {
            return BASS_StreamCreateURL(
                    #ifdef Q_OS_WIN
                        path.toStdWString().data()
                    #else
                        path.toStdString().c_str()
                    #endif
                , 0, BASS_SAMPLE_FLOAT, NULL, 0);
        }

        void init();

        void startTimers();
        void stopTimers(bool paused = false);

        virtual inline void startProccessing() {}
        void aroundProccessing();
    private:
        unsigned long chan;
    };
}

#endif // AUDIO_PLAYER_H
