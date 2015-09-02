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

#include "audio_player/audio_player_spectrum.h"
#include "audio_player/audio_player_equalizer.h"
#include "audio_player/audio_player_panel.h"
#include "audio_player/audio_player_state.h"

namespace AudioPlayer {
    class Base : public Equalizer, public Spectrum,
            public Panel, public State {
        Q_OBJECT

        int openChannel(const QUrl & url);
        void closeChannel();
    public:
        explicit Base(QObject * parent = 0);
        virtual ~Base();

        void setMedia(const QUrl & mediaPath, uint start_pos = 0, int media_duration = -1);

        inline MediaState state() const { return currentState; }
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
