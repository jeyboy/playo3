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

    inline void setMedia(QUrl mediaPath, uint start_pos = 0, int length = -1) {
        mediaUri = mediaPath;
        currentState = InitState;
        startPos = start_pos;
        if ((duration = length) > 0) {
            initDuration();
            setStartPosition();
        }
    }
public slots:
    void play();
    void pause();
    void resume();
    void stop();
    void endOfPlayback();

protected:
    virtual inline void startProccessing() {}
private:
    NotifyTimer * notifyTimer;
    NotifyTimer * spectrumTimer;

    unsigned long chan;
};

#endif // AUDIO_PLAYER_H
