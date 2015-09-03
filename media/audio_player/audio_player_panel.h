#ifndef AUDIO_PLAYER_PANEL
#define AUDIO_PLAYER_PANEL

#include "bass.h"
#include "bass_fx.h"

#include "media/format.h"
#include "media/duration.h"

#include "media/notify_timer.h"

#include "audio_player_equalizer.h"

#include <qurl.h>

#define SLIDE_VOLUME_OFFSET 1000
#define VOLUME_MULTIPLIER 10000.0
#define POSITION_MULTIPLIER 1000.0
#define SLIDE_DURATION_PERCENT 10

namespace AudioPlayer {
    class Panel : public Equalizer {
        Q_OBJECT
    public:
        virtual ~Panel() {}

        inline int getPosition() const { return BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetPosition(chId(), BASS_POS_BYTE)) * POSITION_MULTIPLIER; }
        inline int getFilePosition() const { return BASS_ChannelGetPosition(chId(), BASS_POS_BYTE); } // need to check
        inline int getDuration() const { return duration; }
        inline int getVolume() const { return volumeVal * VOLUME_MULTIPLIER; }

        float getBpmValue(QUrl uri);

        inline int getNotifyInterval() { return notifyInterval; }
        void setNotifyInterval(signed int milis);

    signals:
        void volumeChanged(int);
        void positionChanged(int);
        void durationChanged(int);

    public slots:
        // 0 to 10000
        void setVolume(int val) {
            BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, val);
            emit volumeChanged(val);
        }

        //0 to 10000
        inline void setChannelVolume(int val) {
            volumeVal = val > 0 ? (val / VOLUME_MULTIPLIER) : 0;
            BASS_ChannelSetAttribute(chId(), BASS_ATTRIB_VOL, volumeVal);
            emit volumeChanged(val);
        }

        void slidePosForward();
        void slidePosBackward();
        inline bool setPosition(int position) { return BASS_ChannelSetPosition(chId(), BASS_ChannelSeconds2Bytes(chId(), position / POSITION_MULTIPLIER), BASS_POS_BYTE); }

        void slideVolForward();
        void slideVolBackward();

    protected slots:
        void signalUpdate() { emit positionChanged(BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetPosition(chId(), BASS_POS_BYTE)) * POSITION_MULTIPLIER); }

    protected:
        inline Panel(QObject * parent = 0) : Equalizer(parent), notifyInterval(100), duration(-1), volumeVal(1.0) {}

        virtual unsigned long chId() const = 0;

        inline void initDuration() {
            if (duration == -1)
                duration = round(BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetLength(chId(), BASS_POS_BYTE))) * POSITION_MULTIPLIER;
            durationChanged(duration);
        }

        inline void setStartPosition() {
            if (startPos == 0) return;
            if (setPosition(startPos))
                emit positionChanged(startPos);
        }

        int notifyInterval;
        int duration;
        uint startPos;

        QUrl mediaUri;

        float volumeVal;

        NotifyTimer * notifyTimer;
    };
}

#endif // AUDIO_PLAYER_PANEL
