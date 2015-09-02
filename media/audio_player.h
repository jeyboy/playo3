#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <qapplication.h>
#include <qurl.h>
#include <qdir.h>
#include <qvector.h>

#include "math.h"

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

#include "media/notify_timer.h"

#include "misc/settings.h"

#include "audio_player/audio_player_spectrum.h"
#include "audio_player/audio_player_equalizer.h"
#include "audio_player/audio_player_panel.h"
#include "audio_player/audio_player_state.h"

class AudioPlayer : public QObject, public AudioPlayerEqualizer, public AudioPlayerSpectrum,
        public AudioPlayerPanel, public AudioPlayerState {
    Q_OBJECT

    int openChannel(QString path);
    int openRemoteChannel(QString path);
    void closeChannel();
public:
    explicit AudioPlayer(QObject * parent = 0);
    ~AudioPlayer();

    void setMedia(QUrl mediaPath, uint start_pos = 0, int media_duration = -1);
public slots:
    void play();
    void pause();
    void resume();
    void stop();
    void endOfPlayback();

protected:
    inline int default_device() {
        #ifdef Q_OS_WIN
            return -1;
        #else
            return BASS_DEVICE_DEFAULT;
        #endif
    }

    void init();

    inline void startTimers() {
        notifyTimer -> start(notifyInterval);
        spectrumTimer -> start(Settings::instance() -> spectrumFreqRate()); // 25 //40 Hz
        emit stateChanged(currentState = PlayingState);
    }

    inline void stopTimers(bool paused = false) {
        notifyTimer -> stop();
        spectrumTimer -> stop();

        if (paused)
            emit stateChanged(currentState = PausedState);
        else
            emit stateChanged(currentState = StoppedState);
    }

    virtual inline void startProccessing() {}
private:
    NotifyTimer * notifyTimer;
    NotifyTimer * spectrumTimer;

    unsigned long chan;
};

#endif // AUDIO_PLAYER_H
