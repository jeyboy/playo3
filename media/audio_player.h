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

#include "misc/settings.h"

#include "audio_player/audio_player_spectrum.h"
#include "audio_player/audio_player_equalizer.h"
#include "audio_player/audio_player_panel.h"
#include "audio_player/audio_player_state.h"

class AudioPlayer : public QObject, public AudioPlayerEqualizer, public AudioPlayerSpectrum,
        public AudioPlayerPanel, public AudioPlayerState {
    Q_OBJECT

    int openChannel(const QUrl & url);
    void closeChannel();
public:
    explicit AudioPlayer(QObject * parent = 0);
    ~AudioPlayer();

    void setMedia(const QUrl & mediaPath, uint start_pos = 0, int media_duration = -1);
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
    NotifyTimer * notifyTimer;

    unsigned long chan;
};

#endif // AUDIO_PLAYER_H
